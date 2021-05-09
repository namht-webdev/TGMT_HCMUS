#include<opencv2/opencv.hpp>
#include<iostream>
#include"SIFT.h"
int main(int argc, char** argv) {
	Mat scr = imread(argv[1], 0);
	mSIFT sift(scr, 4, 5);

	sift.doSIFT();
	sift.Display(2, 4);
}