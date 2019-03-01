/*
工程：基于卷积神经网络的双目视觉物体识别与定位
单位：哈尔滨工程大学
时间：2017年4月13日
作者：蒋强卫
文件功能：边缘检测算法头文件
*/

#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <vector>
#include <memory>
#include "opencv2/imgproc/imgproc.hpp"
#include "ipoint.h"

const std::vector< std::vector<int>> sobel_h_kernel =            {{ 1,    0,   -1}, { 2,    0,   -2}, { 1,    0,   -1}};
const std::vector< std::vector<int>> sobel_v_kernel =            {{ 1,    2,    1}, { 0,    0,    0}, {-1,   -2,   -1}};
const std::vector< std::vector<int>> sharpen_kernel =            {{ 0,   -1,    0}, {-1,    5,   -1}, { 0,   -1,    0}};
const std::vector< std::vector<int>> edge_kernel_weak =          {{ 1,    0,   -1}, { 0,    0,    0}, {-1,    0,    0}};
const std::vector< std::vector<int>> edge_kernel_normal =        {{ 0,    1,    0}, { 1,   -4,    1}, { 0,    1,    0}};
const std::vector< std::vector<int>> edge_kernel_strong =        {{-1,   -1,   -1}, {-1,    8,   -1}, {-1,   -1,   -1}};
const std::vector< std::vector<int>> gaussian_blur =             {{ 1,    2,    1}, { 2,    4,    2}, { 1,    2,    1}};
const std::vector< std::vector<int>> roberts_h_kernel =          {{ 1,    0}, { 0,   -1}};
const std::vector< std::vector<int>> roberts_v_kernel =          {{ 0,    1}, {-1,    0}};

const std::vector< std::vector<int>> prewitt_h_kernel =            {{ -1,    0,   1}, { -1,    0,   1}, { -1,    0,   1}};
const std::vector< std::vector<int>> prewitt_v_kernel =            {{ -1,    -1,    -1}, { 0,    0,    0}, {1,   1,   1}};

std::unique_ptr< cv::Mat > ImageConvolute8U(cv::Mat& src, const std::vector< std::vector< int > >& kernel, const double multiplier = 1.0);
std::unique_ptr< cv::Mat > ApplyEdgeDetection(cv::Mat& src, const std::vector<std::vector<int>>& hKernel, const std::vector<std::vector<int>>& vKernel);
std::unique_ptr<cv::Mat> ApplyRobertsEdgeDetection(cv::Mat& src);
std::unique_ptr<cv::Mat> ImageConvolute32S(cv::Mat& src, const std::vector<std::vector<int>>& kernel);

std::unique_ptr<cv::Mat> ThresholdImage(cv::Mat& src, int threshold);

std::unique_ptr<cv::Mat> FindZeroCrossings(cv::Mat& src, int threshold);

std::vector<std::vector<int>> CalculateLaplacianOfGaussianKernel(int size, double sigma = 1.4);
std::vector<std::vector<int>> CalculateGaussianKernel(int size, double sigma = 1.0);

std::vector< std::vector<int>> RotateKernel(const std::vector<std::vector<int>>& kernel, int degree);
std::unique_ptr<cv::Mat> FindHighestResponseValues(std::vector<cv::Mat*> images);
std::unique_ptr<cv::Mat> TraceEdgesHysteresis(cv::Mat& src, int thresholdMax, int thresholdMin,IpVec &ipts);

void ApplyCanny(cv::Mat& src, int thresholdMax, int thresholdMin,IpVec &ipts);
std::unique_ptr<cv::Mat> NonMaximaSuppression(cv::Mat& src, cv::Mat& Gh, cv::Mat& Gv);

//If a kernel overlaps the border we assume a black pixel
template<typename T>
T CheckBorderGetPixel(cv::Mat src, int x, int y) {
    if ((x < 0) || (x >= src.cols) || (y < 0) || (y >= src.rows)) {
        return 0;
    } else {
        return src.at<T>(y, x);
    }
}

#endif //IMAGE_PROCESSING_H
