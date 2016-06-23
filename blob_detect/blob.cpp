/**
 * OpenCV SimpleBlobDetector Example
 *
 * Copyright 2015 by Satya Mallick <spmallick@gmail.com>
 *
 */

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void r_subtract_gb(const Mat & bgr,Mat & r);

int main( int argc, char** argv )
{
	VideoCapture cap(1); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	namedWindow("camera",1);
	namedWindow("red",1);
	namedWindow("bw",1);
	Mat frame;
	vector<Mat> channels;
	Mat img_r,img_gray,img_bw,img_detect;
	//Mat morph_element = getStructuringElement(MORPH_RECT,cvSize(17,17));
	//morphologyEx(src,dst,MORPH_OPEN,morph_element);
	vector<Mat> contours;
	for(;;)
	{
		cap>>frame;
		//imwrite("frame.jpg",frame);
		//return 0;
		//split(frame,channels);
		//img_r = channels.at(2);
		r_subtract_gb(frame,img_r);
		cvtColor(frame,img_gray,CV_BGR2GRAY);
		threshold(img_r,img_bw,150,255,THRESH_BINARY);
		findContours(img_bw,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
		drawContours(frame,contours,-1,Scalar(0,255,0),2);
		// assume only one contour
		if(contours.size()==1)
		{
			vector<Moments> mu(contours.size());
			mu[0] = moments(contours[0],false);
			vector<Point2d> mc(contours.size());
			mc[0] = Point2d(mu[0].m10/mu[0].m00,mu[0].m01/mu[0].m00);
			cout<<"center: "<<mc[0].x<<" "<<mc[0].y<<endl;
			circle(frame,mc[0],4,Scalar(255,0,0),-1,8,0);
		}
		else
			cout<<"contours.size: "<<contours.size()<<endl;

		imshow("camera",frame);
		imshow("red",img_r);
		imshow("bw",img_bw);
		
		//imshow("detect",img_detect);

		if(waitKey(30)>=0)
			break;
	}
	return 0;

}
void r_subtract_gb(const Mat & bgr,Mat & r)
{
	if(!r.data)
		r = Mat(bgr.rows,bgr.cols,CV_8UC1);
	uchar * data1 = bgr.data;
	uchar * data2 = r.data;
	int step1 = bgr.step /sizeof(uchar);
	int step2 = r.step /sizeof(uchar);
	//cout<<"step: "<<step1<<" "<<step2<<endl;
	int idx1,idx2;
	int _r,_g,_b;
	int tmp;
	int max_tmp = 0;
	for(int i=0;i<bgr.rows;++i)
		for(int j=0;j<bgr.cols;++j)
		{
			idx1 = i*step1 + j*3; // NOTE: j*3;
			idx2 = i*step2 + j;
			_b = data1[idx1];
			_g = data1[idx1+1];
			_r = data1[idx1+2];
			tmp = _r - (_b+_g)/2;

			if(tmp > 0)
				data2[idx2] = tmp;
			else
				data2[idx2] = 0;
		}

}
