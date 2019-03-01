/*
单位：哈尔滨工程大学
时间：2017年4月13日
作者：蒋强卫
文件功能：一些共用的头文件
*/


#ifndef COMMON_JQW_H
#define COMMON_JQW_H


#include <fstream>
#include <string>
#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include <cv.h>
#include <cxmisc.h>
#include <highgui.h>
#include <cvaux.h>
#include <iostream>
#include <ctype.h>
//#include <unistd.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include <stdio.h>


#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>



#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include <cv.h>
#include <cxmisc.h>
#include <highgui.h>
#include <cvaux.h>
#include <iostream>
#include <ctype.h>
//#include <unistd.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>

#include <cstdio>
#include <string>
#include "time.h"

using namespace cv;
using namespace std;

class Mypoint;
typedef std::vector<std::pair<Mypoint, Mypoint> >  Match_Point;

//定义目标信息结构体
struct de_class
 {
   int  class_s;
   int  xmin;
   int  ymin;
   int  xmax;
   int  ymax;
  };
struct objects
 {
   de_class  object[100];
   int  de_n;
  };
//定义坐标类
class Mypoint
{
  public:
  int  x,y;

  ~Mypoint(){};
  void Setpoint(int x_i,int y_i)
  {
    x=x_i;
    y=y_i;
  }
  Mypoint()
  {
    x=0;
    y=0;
  }
};


#endif
