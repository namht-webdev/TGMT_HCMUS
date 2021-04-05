#include"Edge_Detection.h"
#include"Convolution.h"


int Detect::rgb2gray(const Mat& srcImage, Mat& dstImage) {
	if (srcImage.data == NULL) {
		return 0;
	}
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	dstImage = Mat(height, width, CV_8UC1);
	int dChannels = dstImage.channels();
	for (int i = 0; i < height; i++) {
		const uchar* pSRow = (uchar*)srcImage.ptr(i);
		uchar* pDRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < width; j++, pSRow += sChannels, pDRow += dChannels) {
			for (int k = 0; k < sChannels; k++) {
				uchar B = pSRow[0];
				uchar G = pSRow[1];
				uchar R = pSRow[2];
				pDRow[0] = 0.3 * B + 0.59 * G + 0.11 * R;
			}
		}
	}
	return 1;
}


void Detect::extendImage(Mat& srcImage, Mat& dstImage) {
	if (srcImage.channels() == 3)
		rgb2gray(srcImage, dstImage);
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	dstImage = Mat(height + 2, width + 2, CV_8UC1, Scalar(0));
	int dChannels = dstImage.channels();
	for (int i = 0; i < height; i++) {
		const uchar* pSRow = (uchar*)srcImage.ptr(i);
		uchar* pDRow = (uchar*)dstImage.ptr(i + 1);
		pDRow += dChannels;
		for (int j = 0; j < width; j++, pSRow += sChannels, pDRow += dChannels) {
			pDRow[0] = pSRow[0];
		}
	}

}


int Detect::detectBySobel(Mat& srcImage, Mat& dstImage) {
	if (srcImage.data == NULL) {
		return 0;
	}
	float Gx[3][3] = { 1,0,-1,2,0,-2,1,0,-1 };
	float Gy[3][3] = { 1,2,1,0,0,0,-1,-2,-1 };
	Convolution* cvl = new Convolution();
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
			pTRow[0] += cvl->doConvolution(srcImage, Gy, i, j);
		}
	}
	imshow("Destination Image, Sobel filter, gradient x", dstImage);
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gy, i, j);
		}
	}


	imshow("Destination Image, Sobel filter, gradient y", dstImage);

	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
			pTRow[0] += cvl->doConvolution(srcImage, Gy, i, j);
		}
	}

	delete cvl;
	return 1;
}


int Detect::detectByPrewitt(Mat& srcImage, Mat& dstImage) {
	if (srcImage.data == NULL) {
		return 0;
	}

	float Gx[3][3] = { 1,0,-1,1,0,-1,1,0,-1 };
	float Gy[3][3] = { 1,1,1,0,0,0,-1,-1,-1 };
	Convolution* cvl = new Convolution();
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
		}
	}
	imshow("Destination Image, Pre-witt filter, gradient x", dstImage);
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gy, i, j);
		}
	}

	imshow("Destination Image, Pre-witt filter, gradient y", dstImage);
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
			pTRow[0] += cvl->doConvolution(srcImage, Gy, i, j);
		}
	}

	delete cvl;
	return 1;
}

int Detect::detectByLaplace(Mat& srcImage, Mat& dstImage) {
	if (srcImage.data == NULL) {
		return 0;
	}
	if (srcImage.data == NULL) {
		return 0;
	}

	float Gx[3][3] = { 0,-1,0,-1,4,-1,0,-1,0 };
	float Gy[3][3] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	Convolution* cvl = new Convolution();
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
			pTRow[0] += cvl->doConvolution(srcImage, Gy, i, j);
		}
	}
	delete cvl;


	return 1;
}


int Detect::detectByCanny(Mat& srcImage, Mat& dstImage) {
	if (srcImage.data == NULL) {
		return 0;
	}
	if (srcImage.data == NULL) {
		return 0;
	}

	float Gx[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	float Gy[3][3] = { -1,-2,-1,0,0,0,1,2,1};
	Convolution* cvl = new Convolution();
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		uchar* pSRow = (uchar*)srcImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pSRow += sChannels, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
			pTRow[0] += cvl->doConvolution(srcImage, Gy, i, j);
		}
	}

	delete cvl;
	return 1;
}

