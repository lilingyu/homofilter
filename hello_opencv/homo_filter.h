//
//  homo_filter.h
//  hello_opencv
//
//  Created by LiLingyu on 15/6/24.
//  Copyright (c) 2015年 LiLingyu. All rights reserved.
//

#ifndef __hello_opencv__homo_filter__
#define __hello_opencv__homo_filter__

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include <fstream>
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

void test();

void HomoFilter(Mat srcImg, Mat &dst);
void my_HomoFilter(Mat srcImg, Mat &dst);

#endif /* defined(__hello_opencv__homo_filter__) */
