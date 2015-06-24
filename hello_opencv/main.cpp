//
//  main.cpp
//  hello_opencv
//
//  Created by LiLingyu on 15/6/24.
//  Copyright (c) 2015年 LiLingyu. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include <fstream>
using namespace std;

#define BYTE unsigned char

int main(int argc, const char * argv[])

{
    // insert code here...
#if 1
    //get the image from the directed path
    IplImage* img = cvLoadImage("/Users/lilingyu1/Documents/sample.jpg", 1);
    //NSLog(img);
    //create a window to display the image
    cvNamedWindow("picture", 1);
    //show the image in the window
    cvShowImage("picture", img);
    //wait for the user to hit a key
    cvWaitKey(0);
    //delete the image and window
    cvReleaseImage(&img);
    cvDestroyWindow("picture");
#endif
    //return
    return 0;
}

