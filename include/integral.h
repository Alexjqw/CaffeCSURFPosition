/*
工程：基于卷积神经网络的双目视觉物体识别与定位
单位：哈尔滨工程大学
时间：2017年4月13日
作者：蒋强卫
文件功能：积分图像计算的头文件
*/

#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <algorithm>  // req'd for std::min/max

// undefine VS macros
#ifdef min
  #undef min
#endif

#ifdef max
  #undef max
#endif

#include <cv.h>

//! Computes the integral image of image img.  Assumes source image to be a
//! 32-bit floating point.  Returns IplImage in 32-bit float form.
IplImage *Integral1(IplImage *img);


//! Computes the sum of pixels within the rectangle specified by the top-left start计算由左上角指定的矩形中像素的总和。
//! co-ordinate and size
inline float BoxIntegral(IplImage *img, int row, int col, int rows, int cols)
{
  float *data = (float *) img->imageData;
  int step = img->widthStep/sizeof(float);

  // The subtraction by one for row/col is because row/col is inclusive.
  int r1 = std::min(row,          img->height) - 1;
  int c1 = std::min(col,          img->width)  - 1;
  int r2 = std::min(row + rows,   img->height) - 1;
  int c2 = std::min(col + cols,   img->width)  - 1;

  float A(0.0f), B(0.0f), C(0.0f), D(0.0f);
  if (r1 >= 0 && c1 >= 0) A = data[r1 * step + c1];
  if (r1 >= 0 && c2 >= 0) B = data[r1 * step + c2];
  if (r2 >= 0 && c1 >= 0) C = data[r2 * step + c1];
  if (r2 >= 0 && c2 >= 0) D = data[r2 * step + c2];

  return std::max(0.f, A - B - C + D);
}

#endif
