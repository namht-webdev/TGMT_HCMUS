#include"opencv2/imgproc.hpp"
#include"Edge_Detection.h"
#include<iostream>

using namespace std;

int main(int argc, char** argv) {
	Detect* dt = new Detect(); // con trỏ đến các hàm detect
	Mat src; // ảnh gốc
	if (atoi(argv[2]) == 0) { // nếu tham số 2 là 0 đọc ảnh với kiểu ảnh độ xám
		src = imread(argv[1], IMREAD_GRAYSCALE); // đọc ảnh
	}
	else // nếu không là 0 đọc ảnh với kiểu ảnh RGB
		src = imread(argv[1], IMREAD_COLOR);
	Mat dst; // ảnh sau quá trình tiền xử lý
	Mat dst2; // ảnh đích
	string Command = argv[3]; // tham số tên hàm
	dt->extendImage(src, dst); // tiền xử lý ảnh(mở rộng ảnh để tính tích chập)
	// Phát hiện biên cạnh bằng bộ lọc Sobel
	if (Command == "detectBySobel") {
		dt->detectBySobel(dst, dst2);
		imshow("Source Image", src);
	}
	// Phát hiện biên cạnh bằng bộ lọc Prewitt
	else if (Command == "detectByPrewitt") {
		dt->detectByPrewitt(dst, dst2);
		imshow("Source Image", src);
	}
	// Phát hiện biên cạnh bằng bộ lọc Laplace
	else if (Command == "detectByLaplace") {
		dt->detectByLaplace(dst, dst2);
		imshow("Detecting by Laplace", dst2);
		imshow("Source Image", src);
	}
	// Phát hiện biên cạnh bằng bộ lọc Canny
	else if (Command == "detectByCanny") {
		dt->detectByCanny(dst, dst2);
		imshow("Detecting by Canny", dst2);
		imshow("Source Image", src);
	}
	//So sánh với bộ lọc Sobel của OpenCV
	else if (Command == "compareSobel") {
		Sobel(src, dst, CV_8UC1, 1, 1, 3);
		imshow("Sobel in OpenCV", dst);
		dt->detectBySobel(src, dst2);
		destroyWindow("Destination Image, Sobel filter, gradient x");
		destroyWindow("Destination Image, Sobel filter, gradient y");
		imshow("My code", dst2);
	}
	//So sánh với bộ lọc Laplacian của OpenCV
	else if (Command == "compareLaplacian") {
		Laplacian(src, dst, CV_8UC1, 3);
		imshow("Laplacian in OpenCV", dst);
		dt->detectByLaplace(src, dst2);
		imshow("My code", dst2);
	}
	//So sánh với bộ lọc Canny của OpenCV
	else if (Command == "compareCanny") {
		Canny(src, dst, 50, 70);
		imshow("Canny in OpenCV", dst);
		dt->detectByCanny(src, dst2);
		imshow("My code", dst2);
	}
	// dừng màn hình để hiển thị ảnh
	waitKey(0);
	delete dt;
	return 0;
}

