#include"SIFT.h"

mSIFT::mSIFT(Mat img, int octaves, int intervals) {
	//Khởi tạo đối tượng SIFT
	m_srcImage = img.clone();
	m_numOctaves = octaves;
	m_numIntervals = intervals;
	for (int i = 0; i < m_numOctaves; ++i) {
		ScaleSpace.push_back(MatArray(m_numIntervals + 3));
		DoG.push_back(MatArray(m_numIntervals + 2));
		DoG_KP.push_back(MatArray(m_numIntervals));
		Mag.push_back(MatArray(m_numIntervals));
		Orient.push_back(MatArray(m_numIntervals));
	}
}

void mSIFT::doSIFT() {
	//Thực hiện quá trình tìm SIFT
	CreateScaleSpace();
	DoGExtrema();
	FilterDoGExtrema();
	AssignOrientation();
}

void mSIFT::CreateScaleSpace() {
	//Tạo scale space
	Size ksize(5, 5);
	double sigma;
	Mat smoothScr;
	Mat up;
	Mat down;
	//Làm mượt ảnh đầu vào
	GaussianBlur(m_srcImage, smoothScr, ksize, 0.5);
	pyrUp(smoothScr, up);
	up.copyTo(ScaleSpace[0][0]);

	GaussianBlur(ScaleSpace[0][0], ScaleSpace[0][0], ksize, 1);

	for (unsigned int i = 0; i < m_numOctaves; i++) {
		sigma = 1.4142;
		for (unsigned int j = 0; j < m_numIntervals + 2; j++) {
			sigma = sigma * pow(2, j / 2);
			GaussianBlur(ScaleSpace[i][j], ScaleSpace[i][j + 1], ksize, sigma);
			DoG[i][j] = ScaleSpace[i][j] - ScaleSpace[i][j + 1];
		}
		if (i < m_numOctaves - 1) {
			pyrDown(ScaleSpace[i][0], down);
			down.copyTo(ScaleSpace[i + 1][0]);
		}
	}
}
void mSIFT::DoGExtrema() {
	Mat local_max;
	Mat local_min;
	Mat extrema, current, top, down;

	for (unsigned int i = 0; i < m_numOctaves; i++) {
		for (unsigned int j = 0; j < m_numIntervals; j++) {
			DoG_KP[i][j] = Mat::zeros(DoG[i][j].size(), DoG[i][j].type());
			top = DoG[i][j];
			current = DoG[i][j + 1];
			down = DoG[i][j + 2];
			int x = current.rows;
			int y = current.cols;
			//Tìm local max 
			local_max = (current(Range(1, x - 1), Range(1, y - 1)) > current(Range(0, x - 2), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > current(Range(0, x - 2), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > current(Range(0, x - 2), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > current(Range(1, x - 1), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > current(Range(1, x - 1), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > current(Range(2, x), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > current(Range(2, x), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > current(Range(2, x), Range(2, y)));
			//Tìm local max ở tầng trên 
			local_max = local_max & (current(Range(1, x - 1), Range(1, y - 1)) > top(Range(0, x - 2), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(0, x - 2), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(0, x - 2), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(1, x - 1), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(1, x - 1), Range(1, y - 1))) &  
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(1, x - 1), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(2, x), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(2, x), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > top(Range(2, x), Range(2, y)));
			//Tìm local max ở tầng trên 
			local_max = local_max & (current(Range(1, x - 1), Range(1, y - 1)) > down(Range(0, x - 2), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(0, x - 2), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(0, x - 2), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(1, x - 1), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(1, x - 1), Range(1, y - 1))) &  
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(1, x - 1), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(2, x), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(2, x), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) > down(Range(2, x), Range(2, y)));
			//Tìm local min tầng hiện tại
			local_min = (current(Range(1, x - 1), Range(1, y - 1)) < current(Range(0, x - 2), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < current(Range(0, x - 2), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < current(Range(0, x - 2), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < current(Range(1, x - 1), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < current(Range(1, x - 1), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < current(Range(2, x), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < current(Range(2, x), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < current(Range(2, x), Range(2, y)));
			//Tìm local min tầng trên
			local_min = local_min & (current(Range(1, x - 1), Range(1, y - 1)) < top(Range(0, x - 2), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(0, x - 2), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(0, x - 2), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(1, x - 1), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(1, x - 1), Range(1, y - 1))) &  // same pixel in top
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(1, x - 1), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(2, x), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(2, x), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < top(Range(2, x), Range(2, y)));
			//Tìm local min tầng trên
			local_min = local_min & (current(Range(1, x - 1), Range(1, y - 1)) < down(Range(0, x - 2), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(0, x - 2), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(0, x - 2), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(1, x - 1), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(1, x - 1), Range(1, y - 1))) &  // same pixel in down
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(1, x - 1), Range(2, y))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(2, x), Range(0, y - 2))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(2, x), Range(1, y - 1))) &
				(current(Range(1, x - 1), Range(1, y - 1)) < down(Range(2, x), Range(2, y)));
			//Lấy extrema
			extrema = local_max | local_min;

			extrema.copyTo(DoG_KP[i][j](Range(1, DoG_KP[i][j].rows - 1), Range(1, DoG_KP[i][j].cols - 1)));
		}
	}
}
void mSIFT::FilterDoGExtrema() {
	//Loại bỏ các extrema có độ tương phản thấp hoặc nằm trên cạnh
	Mat locs; int x, y, rx, ry, fxx, fxy, fyy, deter;
	float trace, curvature;
	float curv_threshold = ((10 + 1)*(10 + 1)) / 10;

	for (unsigned int i= 0; i < m_numOctaves; i++)
	{
		for (unsigned int j = 0; j < m_numIntervals; j++)
		{
			int reject_contrast_count = 0, reject_edge_count = 0;
			cv::findNonZero(DoG_KP[i][j], locs);       // vị trí điểm đặc trưng
			int num_keypts = locs.rows;                       // số điểm đặc trưng
			Mat_<uchar> current = DoG[i][j + 1];

			for (int k = 0; k < num_keypts; k++)
			{
				x = locs.at<int>(k, 0);
				y = locs.at<int>(k, 1);

				// Bỏ những điểm tương phản thấp
				if (abs(current(x + 1, y + 1)) < 0.03)
				{
					DoG_KP[i][j].at<uchar>(x, y) = 0;
					reject_contrast_count++;
				}
				// Loại bỏ extrema trên cạnh
				else
				{
					rx = x + 1;
					ry = y + 1;
					// Tính toán các thành phần của ma trận Hessian 
					fxx = current(rx - 1, ry) + current(rx + 1, ry) - 2 * current(rx, ry);   // đạo hàm bậc 2 theo x
					fyy = current(rx, ry - 1) + current(rx, ry + 1) - 2 * current(rx, ry);   // đạo hàm bậc 2 theo y
					fxy = current(rx - 1, ry - 1) + current(rx + 1, ry + 1) - current(rx - 1, ry + 1) - current(rx + 1, ry - 1); // đạo hàm riêng theo x và y
					// Tính định thức và vết ma trận Hessian
					trace = (float)(fxx + fyy);
					deter = (fxx*fyy) - (fxy*fxy);
					curvature = (float)(trace*trace / deter);
					if (deter < 0 || curvature > curv_threshold)   // Loại bỏ điểm trên cạnh
					{
						DoG_KP[i][j].at<uchar>(x, y) = 0;
						reject_edge_count++;
					}
				}
			}
			
		}
	}
}

void mSIFT::AssignOrientation()
{
	//Gán hướng cho keypoint
	for (unsigned int i = 0; i < m_numOctaves; i++)
	{
		for (unsigned int j = 0; j < m_numIntervals; j++)
		{
			Mat_<uchar> current = ScaleSpace[i][j + 1];
			Mag[i][j] = Mat::zeros(current.size(), CV_64FC1);
			Orient[i][j] = Mat::zeros(current.size(), CV_64FC1);

			for (int x = 1; x < current.rows - 1; x++)
			{
				for (int y = 1; y < current.cols - 1; y++)
				{
					// Tính đạo hàm theo x và y 
					double dx = current(x + 1, y) - current(x - 1, y);
					double dy = current(x, y + 1) - current(x, y - 1);

					// Tính độ lớn và hướng cho các vector gradient 
					Mag[i][j].at<double>(x, y) = sqrt(dx * dx + dy * dy);
					Orient[i][j].at<double>(x, y) = (atan2(dy, dx) == 3.1415) ? -3.1415 : atan2(dy, dx);

				}
			}
		}
	}
}
void mSIFT::Display(int octaves,int intervals) {
	imshow("result", DoG_KP[octaves - 1][intervals - 1]);
}