#pragma once
#define EDGE_DETECTION
#ifdef EDGE_DETECTION
#include"opencv2/core.hpp"
#include"opencv2/highgui/highgui.hpp"
using namespace cv;
class Detect{
public:
	int rgb2gray(const Mat& srcImage, Mat& dstImage);
	void extendImage(Mat& srcImage, Mat& dstImage);
	int detectBySobel(Mat& srcImage, Mat& dstImage);
	int detectByPrewitt(Mat& srcImage, Mat& dstImage);
	int detectByLaplace(Mat& srcImage, Mat& dstImage);
	int detectByCanny(Mat& srcImage, Mat& dstImage);
};
#endif EDGE_DETECTION
