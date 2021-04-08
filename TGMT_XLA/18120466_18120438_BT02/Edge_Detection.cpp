#include"Edge_Detection.h"
#include"Convolution.h"

// Hàm chuyển ảnh RGB sang grayscale
int Detect::rgb2gray(const Mat& srcImage, Mat& dstImage) {
	if (srcImage.data == NULL) {
		return 0;
	}
	// lấy kích thước của ảnh và số kênh màu của ảnh gốc
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	// khởi tạo ảnh đích cùng kích thước ảnh gốc và kiểu ảnh Grayscale
	dstImage = Mat(height, width, CV_8UC1);
	// Tính số kênh màu của ảnh đích
	int dChannels = dstImage.channels();
	for (int i = 0; i < height; i++) {
		// con trỏ đầu dòng ảnh gốc pSRow
		const uchar* pSRow = (uchar*)srcImage.ptr(i);
		// con trỏ đầu dòng ảnh đích pDRow
		uchar* pDRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < width; j++, pSRow += sChannels, pDRow += dChannels) {
			for (int k = 0; k < sChannels; k++) {
				uchar B = pSRow[0];
				uchar G = pSRow[1];
				uchar R = pSRow[2];
				// tính độ xám ảnh theo công thức
				pDRow[0] = 0.3 * R + 0.59 * G + 0.11 * B;
			}
		}
	}
	return 1;
}

// hàm mở rộng kích thước ảnh, row + 2 và col + 2 để tính tích chập cho phần biên
void Detect::extendImage(Mat& srcImage, Mat& dstImage) {
	if (srcImage.channels() == 3)
		rgb2gray(srcImage, dstImage);
	// lấy kích thước của ảnh và số kênh màu của ảnh gốc
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	// ảnh được mở rộng
	dstImage = Mat(height + 2, width + 2, CV_8UC1, Scalar(0));
	int dChannels = dstImage.channels();
	// bắt đầu tiến hành mở rộng ảnh
	for (int i = 0; i < height; i++) {
		// con trỏ đầu dòng ảnh gốc pSRow
		const uchar* pSRow = (uchar*)srcImage.ptr(i);
		// con trỏ đầu dòng ảnh đích pDRow
		uchar* pDRow = (uchar*)dstImage.ptr(i + 1);
		pDRow += dChannels;
		for (int j = 0; j < width; j++, pSRow += sChannels, pDRow += dChannels) {
			pDRow[0] = pSRow[0];
		}
	}

}

// Phát hiện biên cạnh bằng bộ lọc Soble 
// Gx[3][3] = { 1,0,-1,2,0,-2,1,0,-1 }, Gy[3][3] = { 1,2,1,0,0,0,-1,-2,-1 }
int Detect::detectBySobel(Mat& srcImage, Mat& dstImage) {
	if (srcImage.data == NULL) {
		return 0;
	}
	// Bộ lọc Sobel theo x và y
	float Gx[3][3] = { 1,0,-1,2,0,-2,1,0,-1 };
	float Gy[3][3] = { 1,2,1,0,0,0,-1,-2,-1 };
	Convolution* cvl = new Convolution();
	// lấy kích thước của ảnh và số kênh màu của ảnh gốc
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	// khỏi tạo ảnh kết quả
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	// lấy kích thước của ảnh và số kênh màu của ảnh kết quả
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	// Tiến hành tìm biên cạnh bằng tích chập
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
			// tổng tích chập theo Gx
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
		}
	}
	// hiển thị ảnh theo đạo hàm x
	imshow("Destination Image, Sobel filter, gradient x", dstImage);
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
			// tổng tích chập theo Gy
			pTRow[0] = cvl->doConvolution(srcImage, Gy, i, j);
		}
	}

	// hiển thị ảnh theo đạo hàm x
	imshow("Destination Image, Sobel filter, gradient y", dstImage);

	// tích chập tổng cho Gx và Gy
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
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
	// Bộ lọc Prewitt theo x và y
	float Gx[3][3] = { 1,0,-1,1,0,-1,1,0,-1 };
	float Gy[3][3] = { 1,1,1,0,0,0,-1,-1,-1 };
	Convolution* cvl = new Convolution();
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	// khỏi tạo ảnh kết quả
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	// lấy kích thước của ảnh và số kênh màu của ảnh kết quả
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	// Tiến hành tìm biên cạnh bằng tích chập
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
		}
	}
	// hiển thị ảnh theo đạo hàm x
	imshow("Destination Image, Pre-witt filter, gradient x", dstImage);
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
			pTRow[0] = cvl->doConvolution(srcImage, Gy, i, j);
		}
	}
	// hiển thị ảnh theo đạo hàm y
	imshow("Destination Image, Pre-witt filter, gradient y", dstImage);
	// tích chập tổng cho Gx và Gy
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
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
	// Bộ lọc Laplace theo x và y
	float Gx[3][3] = { 0,-1,0,-1,4,-1,0,-1,0 };
	float Gy[3][3] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	Convolution* cvl = new Convolution();
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	// khỏi tạo ảnh kết quả
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	// lấy kích thước của ảnh và số kênh màu của ảnh kết quả
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	// Tiến hành tìm biên cạnh bằng tích chập
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
			// tích chập tổng cho Gx và Gy
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
	// Bộ lọc Canny theo x và y
	float Gx[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	float Gy[3][3] = { -1,-2,-1,0,0,0,1,2,1};
	Convolution* cvl = new Convolution();
	int width = srcImage.cols;
	int height = srcImage.rows;
	int sChannels = srcImage.channels();
	// khỏi tạo ảnh kết quả
	dstImage = Mat(height - 2, width - 2, CV_8UC1);
	// lấy kích thước của ảnh và số kênh màu của ảnh kết quả
	int tWidth = dstImage.cols;
	int tHeight = dstImage.rows;
	int tChannels = dstImage.channels();
	// Tiến hành tìm biên cạnh bằng tích chập
	for (int i = 0; i < tHeight; i++) {
		uchar* pTRow = (uchar*)dstImage.ptr(i);
		for (int j = 0; j < tWidth; j++, pTRow += tChannels) {
			// tích chập tổng cho Gx và Gy
			pTRow[0] = cvl->doConvolution(srcImage, Gx, i, j);
			pTRow[0] += cvl->doConvolution(srcImage, Gy, i, j);
		}
	}

	delete cvl;
	return 1;
}

