#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.

    VideoCapture cap2;

    if(!cap.open(1)||!cap2.open(0))
        return 0;

    for(;;)
    {
          Mat frame, frame2;
          cap >> frame;
          cap2 >> frame2;

          cout<<frame.cols<<" x "<<frame.rows<<endl;

          if( frame.empty() ) break; // end of video stream
          imshow("this is you, smile! :)", frame);


          imshow("2",frame2);
         // if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC


          char c=(char)cv::waitKey(25);
          if(c==27)
              break;

    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}
