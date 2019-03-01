/*
工程：基于卷积神经网络的双目视觉物体识别与定位
单位：哈尔滨工程大学
时间：2017年4月13日
作者：蒋强卫
文件功能：积分图像计算
*/

#include "utils.h"

#include "integral.h"

//! Computes the integral image of image img.  Assumes source image to be a
//! 32-bit floating point.  Returns IplImage of 32-bit float form.
IplImage *Integral1(IplImage *source)
{
  // convert the image to single channel 32f
  IplImage *img = getGray(source);
  IplImage *int_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

  // set up variables for data access
  int height = img->height;
  int width = img->width;
  int step = img->widthStep/sizeof(float);
  float *data   = (float *) img->imageData;
  float *i_data = (float *) int_img->imageData;

  // first row only
  float rs = 0.0f;
  for(int j=0; j<width; j++)
  {
    rs += data[j];
    i_data[j] = rs;
  }

  // remaining cells are sum above and to the left
  for(int i=1; i<height; ++i)
  {
    rs = 0.0f;
    for(int j=0; j<width; ++j)
    {
      rs += data[i*step+j];
      i_data[i*step+j] = rs + i_data[(i-1)*step+j];
    }
  }

  // release the gray image
  cvReleaseImage(&img);

  // return the integral image
  return int_img;
}
