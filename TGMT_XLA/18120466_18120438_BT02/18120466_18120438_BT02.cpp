#include"Edge_Detection.h"
#include<iostream>

using namespace std;



int main(char argc, char** argv) {
	Detect* dt = new Detect();
	Mat src; 
	if (argv[2] == 0) {
		src = imread(argv[1], IMREAD_GRAYSCALE);
	}
	else 
		src = imread(argv[1], IMREAD_COLOR);
	Mat dst;
	Mat dst2;
	string Command = argv[3];
	imshow("Source Image", src);
	dt->extendImage(src, dst);
	if (Command == "detectBySobel") {
		dt->detectBySobel(dst, dst2);
	}
	else if (Command == "detectByPrewitt") {
		dt->detectByPrewitt(dst, dst2);
	}
	else if (Command == "detectByLaplace") {
		dt->detectByLaplace(dst, dst2);
		imshow("Detecting by Laplace", dst2);
	}
	else if (Command == "detectByCanny") {
		dt->detectByCanny(dst, dst2);
		imshow("Detecting by Canny", dst2);
	}
	waitKey(0);
	delete dt;
	return 0;
}