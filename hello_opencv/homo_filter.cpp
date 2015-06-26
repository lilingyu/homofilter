//
//  homo_filter.cpp
//  hello_opencv
//
//  Created by LiLingyu on 15/6/24.
//  Copyright (c) 2015年 LiLingyu. All rights reserved.
//

#include "homo_filter.h"

void test()
{
    printf("hello\n");
}

void HomoFilter(Mat srcImg, Mat &dst)
{
    srcImg.convertTo(srcImg, CV_64FC1);
    dst = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
    
    // 构造滤波矩阵
    Mat H_u_v;
    double gammaH = 1.5;
    double gammaL = 0.5;
    double C = 1;
    double d0 = (srcImg.rows/2)*(srcImg.rows/2) + (srcImg.cols/2)*(srcImg.cols/2);
    double d2 = 0;
    H_u_v = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
    
    double totalWeight = 0.0;
    for (int i = 0; i < srcImg.rows; i++)
    {
        double * dataH_u_v = H_u_v.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            d2 = pow((i - srcImg.rows/2), 2.0) + pow((j - srcImg.cols/2), 2.0);
            dataH_u_v[j] = 	(gammaH - gammaL)*(1 - exp(-C*d2/d0)) + gammaL;
            totalWeight += dataH_u_v[j];
        }
    }
    
    double gain = (srcImg.rows*srcImg.cols)/totalWeight;
    //double gain = 2;
    for (int i = 0; i < srcImg.rows; i++)
    {
        double * dataH_u_v = H_u_v.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            //d2 = pow((i - srcImg.rows/2), 2.0) + pow((j - srcImg.cols/2), 2.0);
            dataH_u_v[j] *= gain;
            //totalWeight += dataH_u_v[j];
        }
    }
    
    imshow("H_u_v", H_u_v);
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* srcdata = srcImg.ptr<double>(i);
        //uint8_t* srcdata = srcImg.ptr<uint8_t>(i);
        double* logdata = dst.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            logdata[j] = log(srcdata[j]+0.0001);
        }
    }
    
    //%%%%%%%%%%%%%%%%%%%%%%%傅里叶变换、滤波、傅里叶反变换%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    Mat planes[] = {dst, Mat::zeros(dst.size(), CV_64F)};
    Mat complexI;
    merge(planes, 2, complexI); // Add to the expanded another plane with zeros
    dft(complexI, complexI);    // this way the result may fit in the source matrix
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);
    //imshow("real: ", planes[0]);
    //imshow("notreal:", planes[1]);
    Mat IDFT[] = {Mat::zeros(dst.size(), CV_64F), Mat::zeros(dst.size(), CV_64F)};

#if 1
    //IDFT[0] = H_u_v.mul(planes[0]);//planes[0].mul(H_u_v);
    //IDFT[1] = H_u_v.mul(planes[1]);//planes[1].mul(H_u_v);
    planes[0].copyTo(IDFT[0]);
    planes[1].copyTo(IDFT[1]);
    
#else
    IDFT[0] = planes[0].mul(H_u_v);
    IDFT[1] = planes[1].mul(H_u_v);
#endif
    //imshow("fft real", <#InputArray mat#>)
    
    merge(IDFT, 2, complexI);
    idft(complexI, complexI);
    split(complexI, IDFT);
    
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* dataRe = IDFT[0].ptr<double>(i);
        double* dataIm = IDFT[1].ptr<double>(i);
        double* logdata = dst.ptr<double>(i);
        
        for (int j = 0; j < srcImg.cols; j++)
        {
            if (dataIm[j] < 0)
            {
                logdata[j]  = dataRe[j]*dataRe[j] - dataIm[j]*dataIm[j];
            }
            else
            {
                logdata[j]  = dataRe[j]*dataRe[j] + dataIm[j]*dataIm[j];
            }
        }
    }
    
    
    normalize(dst, dst, 0, 5.545, CV_MINMAX); 	
    
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* logdata = dst.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {	
            logdata[j] = pow(2.718281828, logdata[j])+10;
        }
    }
    dst.convertTo(dst, CV_8UC1);
    
}

void my_HomoFilter(Mat srcImg, Mat &dst)
{
    
    srcImg.convertTo(srcImg, CV_64FC1);
    dst.convertTo(dst, CV_64FC1);
    //1. ln
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* srcdata = srcImg.ptr<double>(i);
        double* logdata = srcImg.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            logdata[j] = log(srcdata[j]+0.0001);
        }
    }
    
#if 1
    //spectrum
    //2. dct
    Mat mat_dct = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
    dct(srcImg, mat_dct);
    imshow("dct", mat_dct);
    
    //3. linear filter
    Mat H_u_v;
    double gammaH = 1.5;
    double gammaL = 0.5;
    double C = 1;
    double d0 = (srcImg.rows/2)*(srcImg.rows/2) + (srcImg.cols/2)*(srcImg.cols/2);
    double d2 = 0;
    H_u_v = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
    
    double totalWeight = 0.0;
    for (int i = 0; i < srcImg.rows; i++)
    {
        double * dataH_u_v = H_u_v.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            d2 = pow((i), 2.0) + pow((j), 2.0);
            dataH_u_v[j] = 	(gammaH - gammaL)*(1 - exp(-C*d2/d0)) + gammaL;
            totalWeight += dataH_u_v[j];
        }
    }
    H_u_v.ptr<double>(0)[0] = 1.1;
    
    //H_u_v = Mat::ones(srcImg.rows, srcImg.cols, CV_64FC1);
    imshow("H_u_v", H_u_v);


    //imshow("before filter", mat_dct);

    /*
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* srcdata = mat_dct.ptr<double>(i);
        double* filtdata = H_u_v.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            srcdata[j] = srcdata[j]*filtdata[j];
            //srcdata[j] = srcdata[j]*1;

        }
    }*/
    mat_dct = mat_dct.mul(H_u_v);
    //Mat tmp = mat_dct.mul(H_u_v);
    //tmp.copyTo(mat_dct);
    //4. idct
    idct(mat_dct, dst);
#endif
    
#if 0
    //spatial high high pass filter
    Mat tmp = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
    GaussianBlur(srcImg, tmp, Size(9, 9), 1.5, 1.5);
    const double alpha = 0.5;
    
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* srcdata = srcImg.ptr<double>(i);
        double* blurdata = tmp.ptr<double>(i);
        double* dstdata = dst.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            dstdata[j] = (1+alpha)*srcdata[j] - alpha*blurdata[j];
            //dstdata[j] = blurdata[j];
            
        }
    }
     
    
    
#endif
    //5. exp
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* srcdata = dst.ptr<double>(i);
        double* dstdata = dst.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            dstdata[j] = exp(srcdata[j]);
        }
    }
    
    //imshow("dst", dst);
    dst.convertTo(dst, CV_8UC1);

}

/*void  homomorphicfiltering(IplImage* src, IplImage* dst,
                           const double & gammaH, const double& gammaL, const double& C, const double & d0)
{
    if ( gammaH < 1 || gammaL > 1 )
    {
        cerr<< "gammaH > 1 && gammaL < 1时高频增强，低频减小!" <<endl;
        return;
    }
    if (src->nChannels != 2 || dst->nChannels != 2 )
    {
        cerr<< "通道数y必须为a1！！" <<endl;
        return;
    }
    if (src->width != dst->width || src->height != dst->height)
    {
        cvError(CV_StsUnmatchedSizes, "homomorphicfiltering", "图像的长和宽必须相等" ,  __FILE__, __LINE__ );
    }
    //图像大小
    CvSize sz = cvSize(src->width, src->height);
    CvMat* temp = cvCreateMat(src->height, src->width, CV_64FC1);
    double P = src->width/2;
    double Q = src->height/2;
    IplImage* imgRe = cvCreateImage(sz, src->depth, 1);
    IplImage* imgIm = cvCreateImage(sz, src->depth, 1);
    IplImage* srcTemp = cvCloneImage(src);
    IplImage* dstTemp = cvCloneImage(src);
    fftshift(src, srcTemp);
    cvSplit(srcTemp, imgRe, imgIm, NULL, NULL);
    for (int y = 0; y != src->height; ++y)
    {
        for (int x = 0; x != src->width; ++x)
        {
            double d2 = pow (pow(x - P, 2.0) + pow(y - Q, 2.0), 0.5);
            *(( double*)CV_MAT_ELEM_PTR(*temp, y, x)) = (gammaH - gammaL)*(1 - exp(-C*d2/pow (d0, 2))) + gammaL;
        }
    }
    cvMul(imgRe, temp, imgRe);
    cvMul(imgIm, temp, imgIm);
    cvMerge(imgRe, imgIm, NULL, NULL, dst);
    fftshift(dst, dst);
}*/
