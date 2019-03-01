/*
工程：基于卷积神经网络的双目视觉物体识别与定位
单位：哈尔滨工程大学
时间：2017年4月13日
作者：蒋强卫
文件功能：矫正和裁剪图像算法实现
*/
#include "common_jqw.h"
//#include "correct_jqw.h"
#include <cstdio>
#include <string>


#include "time.h"

using namespace cv;
using namespace std;

extern "C" {
void correct_jqw(int flag=0) {

	Mat img1, img_res1, img2, img_res2;
    char* calib_file="cam_stereo.yml";

    //cout<<"hello call me"<<endl;
	Mat R1, R2, P1, P2, Q;
	Mat K1, K2, R;
	Vec3d T;
	Mat D1, D2;

	cv::FileStorage fs1(calib_file, cv::FileStorage::READ);
	fs1["K1"] >> K1;
	fs1["K2"] >> K2;
	fs1["D1"] >> D1;
	fs1["D2"] >> D2;
	fs1["R"] >> R;
	fs1["T"] >> T;

	fs1["R1"] >> R1;
	fs1["R2"] >> R2;
	fs1["P1"] >> P1;
	fs1["P2"] >> P2;
	fs1["Q"] >> Q;

	cv::Mat lmapx, lmapy, rmapx, rmapy;
	cv::Mat imgU1, imgU2;
	CvMat part_p;



	img1 = imread("left1.jpg", CV_LOAD_IMAGE_COLOR);
	img2 = imread("right1.jpg", CV_LOAD_IMAGE_COLOR);


//矫正图像
	cv::initUndistortRectifyMap(K1, D1, R1, P1, img1.size(), CV_32F, lmapx, lmapy);
	cv::initUndistortRectifyMap(K2, D2, R2, P2, img2.size(), CV_32F, rmapx, rmapy);
	cv::remap(img1, imgU1, lmapx, lmapy, cv::INTER_LINEAR);
	cv::remap(img2, imgU2, rmapx, rmapy, cv::INTER_LINEAR);




	imwrite("left.jpg", imgU1);
	imwrite("right.jpg", imgU2);


	 IplImage* c_left=cvLoadImage("left.jpg",0);
	 IplImage* c_right=cvLoadImage("right.jpg",0);


   //设置裁剪区域
	 //设置裁剪区域
	 int w_start=90;
	 int h_start=60;
	 int w_end=1100;
	 int h_end=660;
	 int c_width=w_end-w_start;
	 int c_hight=h_end-h_start;


	 cvSetImageROI(c_left,cvRect(w_start,h_start,c_width,c_hight));
 	 cvSetImageROI(c_right,cvRect(w_start,h_start,c_width,c_hight));


	 //CvSize c_size = cvGetSize(c_left);
	 //std::cout << "c_left:::c_size =" <<c_size.width<<"  "<<c_size.height<< '\n';

 	IplImage*  dst_left = cvCreateImage(cvSize(c_width,c_hight), IPL_DEPTH_8U,  c_left->nChannels);
 	IplImage*  dst_right = cvCreateImage(cvSize(c_width,c_hight), IPL_DEPTH_8U,  c_right->nChannels);

	 //c_size = cvGetSize(dst_left);
	//std::cout << "dst_left:::c_size =" <<c_size.width<<"  "<<c_size.height<< '\n';


 	 cvCopy(c_left,dst_left,0);
   cvResetImageROI(c_left);

 	 cvCopy(c_right,dst_right,0);
   cvResetImageROI(c_right);


 	cvSaveImage("c_d_left.jpg", dst_left);
 	cvSaveImage("c_d_right.jpg", dst_right);



	 CvMat* left_mat = cvCreateMat(dst_left->height, dst_left->width,CV_8U);
	 cvConvert (dst_left , left_mat );

	 CvMat* right_mat = cvCreateMat(dst_left->height, dst_left->width,CV_8U);
	 cvConvert (dst_right, right_mat );

	 CvSize c_size = cvGetSize(dst_left);
	 //std::cout << "c_size =" <<c_size.width<<"  "<<c_size.height<< '\n';

	 CvMat* pair_p;
	 pair_p = cvCreateMat( c_size.height, c_size.width*2,CV_8U);


	 cvGetCols( pair_p, &part_p, 0, c_size.width );

	 cvCopy( left_mat, &part_p, 0);

	 cvGetCols( pair_p, &part_p, c_size.width,c_size.width*2 );
	 cvCopy( right_mat, &part_p, 0 );

if (flag ==1)
	{
		for( int j = 0; j < c_size.height; j += 16 )
			 cvLine( pair_p, cvPoint(0,j),cvPoint(c_size.width*2,j),CV_RGB(0,255,0));
		cvShowImage( "矫正剪裁后的图像", pair_p );
		cvSaveImage("1.jpg", pair_p);
	}






	cvReleaseImage(&c_left);
	cvReleaseImage(&c_right);
	cvReleaseMat(&pair_p);

	cvReleaseMat(&left_mat);
	cvReleaseMat(&right_mat);
	//cvReleaseMat(&part_p);
	img1.release();
	img2.release();

 }
}
