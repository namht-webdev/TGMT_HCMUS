#pragma once
#include<opencv2/opencv.hpp>
#include<vector>

#include<opencv2\highgui.hpp>
#include<cmath>
using namespace cv;
using namespace std;

typedef std::vector<Mat> MatArray;
typedef std::vector<MatArray> Mat2DArray;

class Keypoint {
public:
	float x, y;
	std::vector<double> size;
	std::vector<double> dir;
	unsigned int scale;

	Keypoint() {}
	Keypoint(float a, float b) { x = a; y = b; }
	Keypoint(float a, float b, std::vector<double> const& s, std::vector<double> const& d, unsigned int sc) {
		x = a;
		y = b;
		size = s;
		dir = d;
		scale = sc;
	}
};

class mSIFT {
public:
	mSIFT(cv::Mat img, int octaves, int intervals);
	~mSIFT();
	void doSIFT();
	
	void Display(int octaves, int intervals);
private:
	cv::Mat m_srcImage;
	unsigned int m_numOctaves;
	unsigned int m_numIntervals;
	unsigned int m_numKeypoints;

	Mat2DArray ScaleSpace;
	Mat2DArray DoG;
	Mat2DArray DoG_KP;
	Mat2DArray Mag;
	Mat2DArray Orient;
	void CreateScaleSpace();
	void DoGExtrema();
	void FilterDoGExtrema();
	void AssignOrientation();
	std::vector<Keypoint> m_Keypoints;
	
};
