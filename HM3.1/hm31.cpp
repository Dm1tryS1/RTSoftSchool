#include "opencv2/opencv.hpp"
using namespace cv;

void recogniseStickersByThreshold(Mat &frame ,std::vector<std::vector<Point>> &vLine, std::vector<std::vector<Point>> &fLine)
{
    Mat edges;
    std::vector<std::vector<Point> > contours; 
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
            int x1 = br.x,
                y1 = br.y;

            int w = br.width,
                h = br.height;

            int x4,y2;
            y2 = y1 + h;            
            x4 = x1 + w;

            int xc = (x1+x4)/2;
            int yc = (y1+y2)/2;              

            std::cout<<"Centre:"<<xc<<";"<<yc<<"\n"; 
 
            vLine[i].push_back(Point(xc,yc));


        /////////////////////////////////////////////////////////////////////////
            int size = 5;
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
        /////////////////////////////////////////////////////////////////////////////////
            for (int j=0;j<vLine[i].size()-1;j++)
                line(frame,vLine[i][j+1],vLine[i][j],Scalar(255,250,0),3);
          
            rectangle(frame,br,Scalar(0,250,0),2);
            drawContours(frame,contours,-1, (0,0,255), 3, LINE_AA);
        }       
    }

}

int main() 
{
    VideoCapture cap("sample.MOV"); // open the video file for reading
    if ( !cap.isOpened() ) return -1; //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms
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
}
