#include "opencv2/opencv.hpp"
#include "json.hpp"
#include "mosquitto.h"

using namespace cv;
using json = nlohmann::json;

#define mqtt_host "localhost"
#define mqtt_port 1883

struct mosquitto *mosquit;

struct Box {
    int x1, y1;
    int x2, y2;
};

void to_json(json &j, Box &b)
{
    j = json{
        { "vector1", {
            { "x", b.x1},
            { "y", b.y1}
            }
        },
        { "vector2", {
            { "x", b.x2 },
            { "y", b.y2 }
            }
        }
    };
}

void recogniseStickersByThreshold(Mat &frame ,std::vector<std::vector<Point>> &vLine, std::vector<std::vector<Point>> &fLine)
{
    Mat edges;
    std::vector<std::vector<Point>> contours; 
    cvtColor(frame, edges, COLOR_BGR2HSV);    
    Mat tmp(frame.size(),CV_8U);            
    Scalar colors[4] = { Scalar(12,50,220), Scalar(16,255,255),Scalar(25,0,100), Scalar(60,80,255) };

    for (int i=0; i<2; i++)
    {
        inRange(edges,colors[2*i],colors[2*i+1],tmp);
        dilate(tmp,tmp,Mat(),Point(-1,-1),3); 
        erode(tmp,tmp,Mat(),Point(-1,-1),1);
        Canny(tmp,tmp,40,80);
        findContours(tmp, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
               
        if (contours.size() == 1)
        {
            Rect br = boundingRect(contours[0]);
            vLine[i].push_back(Point((2*br.x+br.width)/2,(2*br.y+br.height)/2));         
            
         ///////////////////////////////////////////////////////////////////////
            int size = 3;
            if (vLine[i].size()>size)
            {
                std::vector <int> mas;
                for (int j=0;j<size;j++)
                    mas.push_back(0);

                int sumx = 0;
                int index = 0;
                
                for (int j=0; j<vLine[i].size();j++)
                {
                    sumx -= mas[index];
                    mas[index] = vLine[i][j].y;
                    sumx += mas[index];
                    index++;
                    if (index >= size)
                        index = 0;                    
                }
                fLine[i].push_back(Point(vLine[i].back().x,sumx/size));

                for (int j=0;j<fLine[i].size()-1;j++)
                    line(frame,fLine[i][j+1],fLine[i][j],Scalar(0,250,255),3);                
            }
            else
                fLine[i].push_back(Point(vLine[i].back().x,vLine[i].back().y));
         ////////////////////////////////////////////////////////////////////////////////
            for (int j=0;j<vLine[i].size()-1;j++)
                line(frame,vLine[i][j+1],vLine[i][j],Scalar(255,250,0),3);
          
            rectangle(frame,br,Scalar(0,250,0),2);
            drawContours(frame,contours,-1, (0,0,255), 3, LINE_AA);

            
            if (vLine[0].size()>size)
            {
                Box box;
                box.x1 = vLine[0].back().x;
                box.y1 = vLine[0].back().y;  
                box.x2 = fLine[0].back().x;
                box.y2 = fLine[0].back().y;
                json j;
                to_json(j,box);  
                std::stringstream ss;
                std::string str;
                ss << j.dump();
                ss >> str;
                mosquitto_publish(mosquit, NULL, "test", str.size(), str.c_str(), 0, false);                 
            }
                 
        }
     
    }

}

int main() 
{
    int connection;

    mosquit = mosquitto_new("test", true, NULL);    
    connection = mosquitto_connect(mosquit, mqtt_host, mqtt_port, 60);
    if (connection != 0)
    {
        printf(" Error\n");
        mosquitto_destroy(mosquit);
        return -1;
    }

    VideoCapture cap("sample.MOV"); // open the video file for reading
    if ( !cap.isOpened() ) return -1;
    std::vector<std::vector<Point>> vLine;
    std::vector<std::vector<Point>> fLine;
    std::vector<Point> x;
    for (int i=0;i<2;i++)
    {
        vLine.push_back(x);
        fLine.push_back(x);
    }

    namedWindow("MyVideo",WINDOW_AUTOSIZE); //create a window called "MyVideo"
    while(1) 
    {        
        Mat frame;
        bool flag =cap.read(frame);
        if (!flag) break;

        recogniseStickersByThreshold(frame,vLine,fLine);
        
        resize(frame,frame,Size(600,600));
        imshow("MyVideo", frame); //show the frame in "MyVideo" window
        if(waitKey(30) == 27)  break;
    }

    mosquitto_disconnect(mosquit);
    mosquitto_destroy(mosquit);
    return 0;
}
