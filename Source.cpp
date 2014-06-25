#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <iostream>
#include <stdio.h>

using namespace cv;

int main(int argc, char** argv)
{
	int comp=0;
    CvCapture* capture=0;
    capture=cvCaptureFromCAM(0);
    if(!capture)
    {
        printf("Capture failure\n");
        return -1;
    }

    int dp=1,mindist=100,param1=300,param2=60,minradius=0,maxradius=0;
    IplImage* frame;
    while(true)
    {
        frame=cvQueryFrame(capture);
        if(!frame) break;
        Mat src(frame);
        IplImage* framegray=cvCreateImage(cvGetSize(frame),8,3);
        Mat srcgray(framegray);
        cvtColor(src,srcgray,CV_BGR2GRAY);

        Mat sx,sy,edges;
		int k=0;
        Sobel(srcgray,sx,CV_32FC1,1,0);
        Sobel(srcgray,sy,CV_32FC1,0,1);
        magnitude(sx,sy,edges);
        normalize(edges,edges,255,0,NORM_MINMAX);
        convertScaleAbs( edges, edges );
        //imshow( "edges", edges );
        vector<Vec3f> circles;
		int countapp=0;
		int countret=0;
        HoughCircles( edges, circles, CV_HOUGH_GRADIENT, dp,mindist, param1, param2, minradius, maxradius);
        for(size_t i = 0; i < circles.size(); i++ )
        {
			
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
			if(radius>comp)
			{
				std::cout<<"Approacing "<<countapp<<std::endl;
			}
			else
			{
				std::cout<<"Retreating "<<countret<<std::endl;
			}
            circle( src, center, radius, Scalar(0,255,0), 3, 8, 0 );
			comp = cvRound(circles[i][2]);
        }
        namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
        imshow( "Hough Circle Transform Demo", src );
        cvReleaseImage(&framegray);
        int c=cvWaitKey(1);
        if(char(c)==27)
            break;
    }
    return 0;
}
