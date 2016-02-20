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
#include "homo_filter.h"
using namespace std;

#define BYTE unsigned char

int main(int argc, const char * argv[])

{
    // insert code here...
    int width = 4;
    int height = 4;
    int size = width*height*3/2;
    
#if 0
    int frames = 100/1;
    
    char fp_str[200] = "/Users/lilingyu1/Documents/collection_noise_estimation/noise_sequence/ipod5_368x640_f100.yuv";
    width = 368;
    height = 640;
#endif
    
    
#if 0
    int frames = 240/1;
    
    char fp_str[200] = "/Users/lilingyu1/Documents/collection_noise_estimation/noise_sequence/test2_1280x720.yuv";
    
    width = 1280;
    height = 720;
    
#endif
    
#if 0
    int frames = 240/1;
    
    char fp_str[200] = "/Users/lilingyu1/Documents/collection_noise_estimation/noise_sequence/jianchuan_rd_night_720x1280.yuv";
    
    width = 720;
    height = 1280;
    
#endif
    
#if 1
    int frames = 1;
    
    char fp_str[200] = "/Users/lilingyu1/Documents/dark_1136x1136.yuv";
    
    width = 1136;
    height = 1136;
    
#endif
    
    
#if 0
    char fpo_str[200];
    
    
    strcpy(fpo_str, fp_str);
    //strcat(fpo_str, ".hdr_tone_map.yuv");
    strcat(fpo_str, ".homo.yuv");
    
    
    size = width*height*3/2;
    
    FILE* fp = fopen(fp_str, "rb");
    assert(NULL != fp);
    
    FILE* fpo = fopen(fpo_str, "wb");
    assert(NULL != fpo);
    
    uint8_t* src = (uint8_t*)malloc(size);
    assert(NULL != src);
    
    //homo_filt_context* homo_ctx_p = malloc(sizeof(*homo_ctx_p));
    //assert(NULL != homo_ctx_p);
    float delta = 4.0/20;
    int filter_radius = 3;
    //homo_filter_init((void*)homo_ctx_p, width, height, delta, filter_radius);
    //cv::Mat src_mat = Mat::zeros(height, width, CV_8UC1);
    
    cv::Mat src_mat, dst_mat;
    src_mat.data = src;
    src_mat.rows = height;
    src_mat.cols = width;
    
    dst_mat = Mat::zeros(src_mat.rows, src_mat.cols, CV_8UC1);
    
    long period = 0;
    float seconds_per_frame=0.0;
    
    for (int frame=0; frame<frames; frame++) {
        fseek(fp, frame*width*height*3/2, 0);
        fread(src, 1, width*height*3/2, fp);
        memcpy(src_mat.data, src, width*height);
        
        long tic = clock();
        //hdr_tone_map_1ch(src, width, height);
        //homo_filter((void*)homo_ctx_p, src, width, height);
        //HomoFilter(src_mat, dst_mat);
        my_HomoFilter(src_mat, dst_mat);

        //test();
        
        long toc = clock();
        period += toc-tic;
        float seconds = (toc-tic+0.0)/(CLOCKS_PER_SEC+0.0);
        seconds_per_frame += seconds;
        fwrite(dst_mat.data, 1, width*height, fpo);
        fwrite(src+(width*height), 1, width*height/2, fpo);
        
        printf("frame %3d: %8d\t%f\n", frame, (int)(toc-tic), seconds);
        
        
    }
    period /= frames;
    seconds_per_frame /= frames;
    printf("avg:\nclocks: %4d \tseconds: %8f\n", (int)period, seconds_per_frame);
    
    
    
    fclose(fp);
    fclose(fpo);
    free(src);
    //free(homo_ctx_p);
    //homo_filter_close((void*)homo_ctx_p);
#endif
    
#if 0 //array test
    uint8_t src[4*4] = {1, 1, 1, 1,
                        1, 1, 1, 1,
                        1, 1, 1, 1,
                        1, 1, 1, 1};
    width = 4;
    height = 4;
    cv::Mat src_mat = Mat::ones(height, width, CV_8UC1);
    cout<<src_mat<<endl;

    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            src_mat.data[y*width+x] = 0x80;
        }
    }
    
    cv::Mat dst_mat;
    //src_mat.data = src;
    //src_mat.rows = height;
    //src_mat.cols = width;
    
    HomoFilter(src_mat, dst_mat);

#endif
    
#if 0
    //get the image from the directed path
    IplImage* img = cvLoadImage("/Users/lilingyu1/Documents/video_enhance/data/sample.jpg", 1);
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
    
#if 1
    char fpo_str[200];
    
    
    strcpy(fpo_str, fp_str);
    //strcat(fpo_str, ".hdr_tone_map.yuv");
    strcat(fpo_str, ".homo.yuv");
    
    
    size = width*height*3/2;
    
    FILE* fp = fopen(fp_str, "rb");
    assert(NULL != fp);
    
    FILE* fpo = fopen(fpo_str, "wb");
    assert(NULL != fpo);
    
    uint8_t* src = (uint8_t*)malloc(size);
    assert(NULL != src);
    fread(src, 1, width*height*3/2, fp);

    
    
    Mat src_mat = imread("/Users/lilingyu1/Documents/dark_1136x1136.jpg", 0);//
    imshow("src:", src_mat);
    Mat dst_mat(src_mat.rows, src_mat.cols, src_mat.type());
    //HomoFilter(src_mat, dst_mat);
    my_HomoFilter(src_mat, dst_mat);
    
    fwrite(dst_mat.data, 1, width*height, fpo);
    fwrite(src+(width*height), 1, width*height/2, fpo);
    
    fclose(fp);
    fclose(fpo);
    free(src);
    
    imshow("dst:", dst_mat);
    cvWaitKey(0);

    
#endif
    
    return 0;

    
}

