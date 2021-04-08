#include"Convolution.h"

// row, col là tham số của phần tính từ row col trở đi bằng với kích thước bộ lọc
uchar Convolution::doConvolution(Mat& srcImage, float G[][3], int row, int col) {
	
	float s = 0;
	int k = 2;
	for (int i = row; i < row + 3; i++, k--) {
		int l = 2;
		for (int j = col; j < col + 3; j++, l--) {
			s += (float)srcImage.at<uchar>(i, j) * G[k][l];
		}
	}
	return saturate_cast<uchar>(abs(s));
}