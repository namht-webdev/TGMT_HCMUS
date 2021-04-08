#pragma once
#define EDGE_DETECTION
#ifdef EDGE_DETECTION
#include"opencv2/core.hpp"
#include"opencv2/highgui/highgui.hpp"
using namespace cv;
//Lớp phát hiện biên cạnh
class Detect{
public:
	// Hàm chuyển ảnh màu sang ảnh độ xám
	int rgb2gray(const Mat& srcImage, Mat& dstImage);
	// mở rộng kích thước để tính tích chập
	void extendImage(Mat& srcImage, Mat& dstImage);
	// Phát hiện biên bằng bộ lọc Sobel, Prewitt, Laplace, Canny
	int detectBySobel(Mat& srcImage, Mat& dstImage);
	int detectByPrewitt(Mat& srcImage, Mat& dstImage);
	int detectByLaplace(Mat& srcImage, Mat& dstImage);
	int detectByCanny(Mat& srcImage, Mat& dstImage);
};
#endif EDGE_DETECTION
