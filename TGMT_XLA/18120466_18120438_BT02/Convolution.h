#pragma once
#define CONVOLUTION
#ifdef CONVOLUTION
#include"Edge_Detection.h"
class Convolution {
public:
	uchar doConvolution(Mat &srcImage, float G[][3], int row, int col);
};

#endif // CONVOLUTION
