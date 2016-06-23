/**
 * OpenCV SimpleBlobDetector Example
 *
 * Copyright 2015 by Satya Mallick <spmallick@gmail.com>
 *
 */

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void inverse_density(const Mat & src, Mat & dst);
int main( int argc, char** argv )
{
	VideoCapture cap(1); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 150;
	params.maxThreshold = 255;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 1500;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.2;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;

	// Storage for blobs
	vector<KeyPoint> keypoints;

#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2
	// Set up detector with params
	SimpleBlobDetector detector(params);
#else 
	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);   
#endif 

	namedWindow("camera",1);
	namedWindow("red",1);
	namedWindow("bw",1);
	Mat frame;
	vector<Mat> channels;
	Mat img_r,img_gray,img_bw,img_detect;
	for(;;)
	{
		cap>>frame;
		split(frame,channels);
		img_r = channels.at(2);
		inverse_density(img_r,img_r);
		cvtColor(frame,img_gray,CV_BGR2GRAY);
		//threshold(img_r,img_bw,250,255,THRESH_BINARY);

		imshow("camera",img_gray);
		imshow("red",img_r);
		//imshow("bw",img_bw);
#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2
	// Detect blobs
	detector.detect( img_r, keypoints);
#else
	// Detect blobs
	detector->detect( img_r, keypoints);
#endif
	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob
	drawKeypoints( frame, keypoints, img_detect, Scalar(0,255,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	imshow("detect",img_detect);
	cout<<keypoints.size()<<endl;

		if(waitKey(30)>=0)
			break;
	}
	return 0;

}
// src is 3 channels
void inverse_density(const Mat & src, Mat & dst)
{
	if(!dst.data)
		dst = Mat(src.rows,src.cols,src.type());
	uchar * data_s = src.data;
	uchar * data_d = dst.data;
	int step = src.step /sizeof(uchar);
	int idx;
	for(int i=0;i<src.rows;++i)
		for(int j=0;j<src.cols;++j)
		{
			idx = i*step + j*3;
			data_d[idx] = 255 - data_s[idx];
			data_d[idx+1] = 255 - data_s[idx+1];
			data_d[idx+2] = 255 - data_s[idx+2];
		}
}

