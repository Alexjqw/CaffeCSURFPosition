/*
工程：基于卷积神经网络的双目视觉物体识别与定位
单位：哈尔滨工程大学
时间：2017年4月13日
作者：蒋强卫
文件功能：自定义匹配点类头文件
*/
#ifndef IPOINT_H
#define IPOINT_H

#include <vector>
#include <math.h>

//-------------------------------------------------------

class Ipoint; // Pre-declaration
typedef std::vector<Ipoint> IpVec;
typedef std::vector<std::pair<Ipoint, Ipoint> > IpPairVec;

//-------------------------------------------------------

//! Ipoint operations
void getMatches(IpVec &ipts1, IpVec &ipts2, IpPairVec &matches);
int translateCorners(IpPairVec &matches, const CvPoint src_corners[4], CvPoint dst_corners[4]);

//-------------------------------------------------------

class Ipoint {

public:

  //! Destructor
  ~Ipoint() {};

  //! Constructor
  Ipoint() : orientation(0) {};

  //! Gets the distance in descriptor space between Ipoints
  float operator-(const Ipoint &rhs)
  {
    float sum=0.f;
    for(int i=0; i < 64; ++i)
      sum += (this->descriptor[i] - rhs.descriptor[i])*(this->descriptor[i] - rhs.descriptor[i]);
    return sqrt(sum);
  };

  //! Coordinates of the detected interest point
  float x, y;

  //! Detected scale
  float scale;

  //! Orientation measured anti-clockwise from +ve x-axis
  float orientation;

  //! Sign of laplacian for fast matching purposes
  int laplacian;

  //! Vector of descriptor components
  float descriptor[64];

  //! Placeholds for point motion (can be used for frame to frame motion analysis)
  float dx, dy;

  //! Used to store cluster index
  int clusterIndex;
};

//-------------------------------------------------------


#endif
