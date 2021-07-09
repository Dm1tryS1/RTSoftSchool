#include "opencv2/opencv.hpp"
using namespace cv;

void recogniseStickersByThreshold(Mat &frame)
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
        Canny(tmp,tmp,180,200);
        findContours(tmp, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
               
        if (contours.size() == 1)
        {
            Rect br = boundingRect(contours[0]);           
            rectangle(frame,br,Scalar(0,250,0),2);
            drawContours(frame,contours,-1, (0,0,255), 3, LINE_AA);
        }       
    }

}

int main() 
{
    VideoCapture cap("sample.MOV"); // open the video file for reading
    if ( !cap.isOpened() ) return -1; //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms
    
    namedWindow("MyVideo",WINDOW_AUTOSIZE); //create a window called "MyVideo"
    while(1) 
    {        
        Mat frame;
        bool flag =cap.read(frame);
        if (!flag) break;

        recogniseStickersByThreshold(frame);
        
        resize(frame,frame,Size(800,600));
        imshow("MyVideo", frame); //show the frame in "MyVideo" window
        if(waitKey(30) == 27)  break;
    }
}
