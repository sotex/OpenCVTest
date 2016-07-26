#include <iostream>
// #include "opencv2/opencv.hpp"

#define TEST(func)	\
	std::cout<<"start "#func" ..."<<endl;	\
	func();	\
	std::cout<<"end "#func"!!!"<<endl;

extern int CannyEdgeDetection();
extern int ContourDetection();
extern int rotateAndScale();
extern int videoAndMovingObjectDetection();
extern int houghLineDetection();
extern int mousePainter();

using namespace std;

int main()
{
	// Canny��Ե���
	//TEST(CannyEdgeDetection);
	
	// �������
	//TEST(ContourDetection);

	// ͼ����ת������
	//TEST(rotateAndScale);

	// ��ȡ��Ƶ�ļ����˶�������
	//TEST(videoAndMovingObjectDetection);

	// Hough�߶μ��
	//TEST(houghLineDetection);

	// ����ͼ
	TEST(mousePainter);


	//// ����ͼƬ
	//cv::Mat img = cv::imread("D:\\OpenCV\\doc\\opencv-logo2.png");
	//if (img.empty()) {
	//	cerr << "open image failed!!!" << endl;
	//	return -1;
	//}
	//// ����һ������
	////cv::namedWindow("Show Image");
	//// �ڴ�����ʾͼƬ
	//cv::imshow("opencv-logo2.png", img);
	//// �ȴ��������
	//cv::waitKey();
	getchar();
	return 0;
}