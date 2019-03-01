/*
工程：基于卷积神经网络的双目视觉物体识别与定位
单位：哈尔滨工程大学
时间：2017年4月13日
作者：蒋强卫
文件功能：定位算法实现
*/


#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ImageProcessing.h"

#include <chrono>
#include"ipoint.h"
#include "common_jqw.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "integral.h"
#include "surf.h"
#include "utils.h"

extern "C" {

  objects myobject;
  string class_name[21]={"background","aeroplane","bicycle","bird",
                         "boat","bottle","bus","car", "cat",
                         "chair","cow","diningtable", "dog",
                         "horse", "motorbike","person","pottedplant" ,
                         "sheep","sofa","train", "tvmonitor"};
//解析目标信息文本
void GetObject()
{

 ifstream fs("detection_object.txt");
 string filename;
 string line;
 if(fs) // 判断文件是否存在
 {
     int i=0;
     while (getline (fs, line)) // line中不包括每行的换行符
     {
         //cout << line << endl;
         //获取目标类别序号
         int s_start=line.find("class_s:")+8;
         //std::cout << s_start<< '\n';
         int s_end=line.find(" ,",s_start);
         int w=s_end-s_start;
         //std::cout << s_end<< '\n';
         string buff=line.substr(s_start,w);
         //std::cout <<buff << '\n';
         int num=atoi(buff.c_str());
         myobject.object[i].class_s=num;
         //std::cout << " myobject.object[i].class_s="<<myobject.object[i].class_s<< '\n';


         //获取目标xmin
         s_start=line.find("xmin:")+5;
         //std::cout << s_start<< '\n';
         s_end=line.find(" ,",s_start);
          w=s_end-s_start;
         //std::cout << s_end<< '\n';
         buff=line.substr(s_start,w);
         //std::cout <<buff << '\n';
         num=atoi(buff.c_str());
         myobject.object[i].xmin=num;
         //std::cout << " myobject.object[i].xmin="<<myobject.object[i].xmin<<'\n';

         //获取目标xmax
         s_start=line.find("xmax:")+5;
         //std::cout << s_start<< '\n';
         s_end=line.find(" ,",s_start);
          w=s_end-s_start;
         //std::cout << s_end<< '\n';
         buff=line.substr(s_start,w);
         //std::cout <<buff << '\n';
         num=atoi(buff.c_str());
         myobject.object[i].xmax=num;
         //std::cout << " myobject.object[i].xmax="<<myobject.object[i].xmax<<'\n';

         //获取目标ymin
         s_start=line.find("ymin:")+5;
         //std::cout << s_start<< '\n';
         s_end=line.find(" ,",s_start);
          w=s_end-s_start;
         //std::cout << s_end<< '\n';
         buff=line.substr(s_start,w);
         //std::cout <<buff << '\n';
         num=atoi(buff.c_str());
         myobject.object[i].ymin=num;
         //std::cout << " myobject.object[i].ymin="<<myobject.object[i].ymin<<'\n';

         //获取目标xmax
         s_start=line.find("ymax:")+5;
         //std::cout << s_start<< '\n';
         s_end=line.find(" ,",s_start);
          w=s_end-s_start;
         //std::cout << s_end<< '\n';
         buff=line.substr(s_start,w);
         //std::cout <<buff << '\n';
         num=atoi(buff.c_str());
         myobject.object[i].ymax=num;
         //std::cout << " myobject.object[i].ymax="<<myobject.object[i].ymax<<'\n';
         i=i+1;
     }
     myobject.de_n=i;
     std::cout << "/***************************************************************/" << '\n';
     std::cout <<"识别目标的个数"<< myobject.de_n << '\n';
 }
 else // 没有该文件
 {
     cout <<"no such file" << endl;
 }
}


int EdgeDetectionPosition()
{
   int rio=4;
   cv::Mat src_l1=cv::imread("c_d_left.jpg", CV_LOAD_IMAGE_GRAYSCALE);
   cv::Mat src_r1=cv::imread("c_d_right.jpg", CV_LOAD_IMAGE_GRAYSCALE);
   //auto src_l1 = cv::imread("l.jpg", CV_LOAD_IMAGE_GRAYSCALE);

   //auto src_r1 = cv::imread("r.jpg", CV_LOAD_IMAGE_GRAYSCALE);
   cv::Mat src_l,src_r;
   resize(src_l1,src_l,Size(src_r1.cols/rio,src_r1.rows/rio),0,0,INTER_LINEAR);
   resize(src_r1,src_r,Size(src_r1.cols/rio,src_r1.rows/rio),0,0,INTER_LINEAR);



    IpVec ipts_l,ipts_r;
    IpPairVec matches;
    std::vector<KeyPoint> keypoints_l,keypoints_r;

    //measure time start
    clock_t start, finish;
    double  duration;
    start = clock();
    //*************

     ApplyCanny(src_l, 45,50,ipts_l);
     ApplyCanny(src_r, 45,50,ipts_r);



    IplImage *img_l1 = cvLoadImage("c_d_left.jpg");
    IplImage *img_r1 = cvLoadImage("c_d_right.jpg");
    //sift_dec(img1,ipts_l);
    IplImage *int_img_l = Integral1(img_l1);
    IplImage *int_img_r = Integral1(img_r1);


    Surf des_l(int_img_l, ipts_l);
    Surf des_r(int_img_r, ipts_r);
    des_l.getDescriptors(true);
    des_r.getDescriptors(true);



   std::cout << "/* ipts_l.size() */" <<ipts_l.size()<< '\n';
   getMatches(ipts_l,ipts_r,matches);
   std::cout << "/* matches.size() */" <<matches.size()<< '\n';

   //maesure time end
   finish = clock();
   duration = (double)(finish - start) / CLOCKS_PER_SEC;
   printf( "%f seconds\n", duration );
  //*************

  //拼接图片
  CvMat* pair_img;
  CvSize c_size = cvGetSize(img_l1);
  pair_img= cvCreateMat( c_size.height, c_size.width*2,CV_8UC3);


  StitcImage(img_l1,img_r1,pair_img); //拼接图片
  Mypoint mypoint_l[matches.size()],mypoint_r[matches.size()];
  for (unsigned int i = 0; i <matches.size(); ++i)
  {
    drawPoint_Line_jqw(pair_img,matches[i].first,matches[i].second, c_size.width,i);
    mypoint_l[i].Setpoint(fRound(matches[i].first.x),fRound(matches[i].first.y));
    mypoint_r[i].Setpoint(fRound(matches[i].second.x),fRound(matches[i].second.y));
    //mypoint_p[1].Setpoint(1,3);
  }

  cvShowImage("匹配的点", pair_img);

  std::cout << "ipts_l" <<ipts_l.size()<< '\n';
  cout<<"ipts_l.size()"<<ipts_l.size()<<endl;

  //获取识别信息
  GetObject();

  //筛选深度值
  int objects_n=myobject.de_n; //识别目标数量
  Match_Point Mymatch[objects_n];
  int num;
  float depth_o[objects_n];
  float depth,sum,meam;
  if(objects_n>0)
  {
    for(int i=0;i<matches.size();i++)
      {

        for(int j=0;j<myobject.de_n;j++)
          {

           if (matches[i].first.x>myobject.object[j].xmin && matches[i].first.x<myobject.object[j].xmax && matches[i].first.y>myobject.object[j].ymin &&matches[i].first.y<myobject.object[j].ymax)
            {
                 Mymatch[j].push_back(std::make_pair(mypoint_l[i], mypoint_r[i]));
            }
          }
       }

       for(int j=0;j<objects_n;j++)
       {
         sum=0;
         depth=0;
         depth_o[j]=0;
         std::cout << "/* 到哪一个目标 */" << j<< '\n';
         std::cout << "/*未优化的匹配为数 */"<< Mymatch[j].size()<< '\n';
         int jqw_sun=0;
         for(int i=0;i<Mymatch[j].size();i++)
         {
          //cvCircle(img1, cvPoint(Mymatch[0][i].first.x,Mymatch[0][i].first.y),3,cvScalar(0, 0, 0), -1);
          depth=Mymatch[j][i].first.x-Mymatch[j][i].second.x;
          if(depth<100&&depth>0)
          {
              sum=sum+depth;
              jqw_sun=jqw_sun+1;
          }

          //std::cout << "/* depth*/" << depth<<'\n';

         }

         //meam=sum/(Mymatch[j].size());
         meam=sum/jqw_sun;

         num=0;
         sum=0;
         std::cout << "/* 未优化的均值meam */"<<meam << '\n';
         //剔除偏离平均值比较远的值
         for(int i=0;i<Mymatch[j].size();i++)
         {
            //cvCircle(img1, cvPoint(Mymatch[0][i].first.x,Mymatch[0][i].first.y),3,cvScalar(0, 0, 0), -1);
            depth=Mymatch[j][i].first.x-Mymatch[j][i].second.x;
            std::cout << "/* depth :*/" <<depth<< '\n';
            if((depth-meam)<3&&(depth-meam)>-3)
            {
              //std::cout << "/*   depth111 */"<<  depth << '\n';

              sum=sum+depth;
              num=num+1;
            }
          }
       std::cout << "/*优化后的匹配为数num */"<< num<< '\n';
       if(num>0)
       {
         meam=sum/num;
         depth_o[j]=meam;
         std::cout << "/*优化后的均值meam */"<<meam << '\n';
       }
     }

     //计算坐标值
     float x[objects_n],y[objects_n],z[objects_n];
     float img_x[objects_n],img_y[objects_n];
     float tempx,tempy,temp,tempd;

     float Q[4][4]={{1.0, 0.0, 0.0, -3.9482571220397949e+02 } ,
                    {0.0, 1.0, 0.0,  -2.5152133750915527e+02} ,
                    {0.0, 0.0, 0.0, 7.9647500902944898e+02  } ,
                    {0.0, 0.0, 9.9490794902812052e+00, 0.0  }
                  };
      int fontface = cv::FONT_HERSHEY_SIMPLEX;
      double scale = 0.8;
      int thickness = 2;
      int baseline = 0;
      cv::Size text = cv::getTextSize(class_name[0], fontface, scale,
                                      thickness,&baseline);
      //std::cout << "/* text.width */" <<text.width<< '\n';
      cv::Mat Result_img,Src_img;
      IplImage *buff_ipl= cvLoadImage("c_d_left.jpg");
      Src_img = cv::Mat(buff_ipl);
      float rio=1.0;
      resize(Src_img,Result_img,Size(Src_img.cols*rio,Src_img.rows*rio),0,0,INTER_LINEAR);
      int class_x;//识别目标序号
      char x_s[20],y_s[20],z_s[20];
      int l=10;


     for(int i=0;i<objects_n;i++)
      {
       if(depth_o[i]!=0)
       {
        img_x[i]=(myobject.object[i].xmin+myobject.object[i].xmax)/2.0;
        img_y[i]=(myobject.object[i].ymin+myobject.object[i].ymax)/2.0;
        tempx=Q[0][0]*img_x[i]+Q[0][3];
        tempy=Q[1][1]*img_y[i]+Q[1][3];
        tempd=Q[2][3];
        temp=Q[3][2]*depth_o[i]+ Q[3][3];
        x[i]=tempx/temp;
        y[i]=tempy/temp;
        z[i]=tempd/temp;


        class_x=myobject.object[i].class_s;
        //打印坐标
        sprintf(x_s, "x=%f", x[i]);
        sprintf(y_s, "y=%f", y[i]);
        sprintf(z_s, "z=%f", z[i]);
        std::cout << "/*第"<<i<<"个目标*/"<<"是"<<class_name[class_x]<<"坐标为:"<<"("<<x[i]<<","<<y[i]<<","<<z[i]<<")" << '\n';
        //框出目标的图像的位置
        cv::rectangle(Result_img, cv::Rect(myobject.object[i].xmin*rio, myobject.object[i].ymin*rio,
                      (myobject.object[i].xmax-myobject.object[i].xmin)*rio,
                      (myobject.object[i].ymax-myobject.object[i].ymin)*rio ),
                      CV_RGB(0,255,0),3,8,0);
       //填充矩形，为了显示目标信息内容
       cv::rectangle(Result_img, cv::Point(myobject.object[i].xmin*rio, myobject.object[i].ymin*rio),
                                    cv::Point(myobject.object[i].xmin*rio+180,  myobject.object[i].ymin*rio+(text.height)*4+10),
                                    CV_RGB(255, 255, 255), CV_FILLED);
      //显示目标类型
      cv::putText(Result_img, class_name[class_x], cv::Point(myobject.object[i].xmin*rio,
                  myobject.object[i].ymin*rio+text.height ),
                  fontface, scale, CV_RGB(0, 255, 0), thickness, 8);
      //显示目标的X轴的值
      cv::putText(Result_img,x_s, cv::Point(myobject.object[i].xmin*rio,
         myobject.object[i].ymin*rio+text.height*2 ),
         fontface, scale, CV_RGB(0, 255, 0), thickness, 8);
         //显示目标的y轴的值
         cv::putText(Result_img,y_s, cv::Point(myobject.object[i].xmin*rio,
            myobject.object[i].ymin*rio+text.height*3.),
            fontface, scale, CV_RGB(0, 255, 0), thickness, 8);
            //显示目标的z轴的值
          cv::putText(Result_img,z_s, cv::Point(myobject.object[i].xmin*rio,
               myobject.object[i].ymin*rio+text.height*4 + baseline / 2.),
               fontface, scale, CV_RGB(0, 255, 0), thickness, 8);
          cv::line(Result_img,cvPoint(img_x[i]*rio-l,img_y[i]*rio),
          cvPoint(img_x[i]*rio+l,img_y[i]*rio),CV_RGB(255,0,0),3,CV_AA,0);
          cv::line(Result_img,cvPoint(img_x[i]*rio,img_y[i]*rio-l),
          cvPoint(img_x[i]*rio,img_y[i]*rio+l),CV_RGB(255,0,0),3,CV_AA,0);
       }
        }

       cv::imshow( "Result of position and recognition", Result_img);
       cv::imwrite("Result_img.jpg",Result_img);


       //cvShowImage( "Result of position and recognition", img1 );
        cv::Mat Map_img= imread("map.jpg", CV_LOAD_IMAGE_UNCHANGED);
        for(int i=0;i<objects_n;i++)
        {

              if (fRound(z[i]*100)>10)
              {
                class_x=myobject.object[i].class_s;
                circle(Map_img,cv::Point(fRound(x[i]*100+300),800-20-fRound(z[i]*100)),3,Scalar(0,0,0),-1);
                circle(Map_img,cv::Point(fRound(x[i]*100+300),800-20-fRound(z[i]*100)),5,Scalar(0,0,255),2);
                cv::putText(Map_img, class_name[class_x],cv::Point(fRound(x[i]*100+300),800-20-fRound(z[i]*100)),
                            fontface, 1, CV_RGB(0, 0, 255), 2, 8);
              }

        }

        cv::imshow( "Orientation map", Map_img);
       for(int i=0;i<objects_n;i++)
       {
        Mymatch[i].clear();

      }//if objects_n>0
   }




  return 0;

}

}
