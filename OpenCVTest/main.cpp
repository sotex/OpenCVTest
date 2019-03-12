#include <iostream>
// #include "opencv2/opencv.hpp"
#include <opencv2/core/version.hpp>

#ifdef _MSC_VER
#ifdef _DEBUG
#define CV_VERSION_NAME CVAUX_STR(CV_VERSION_MAJOR)  CVAUX_STR(CV_VERSION_MINOR) CVAUX_STR(CV_VERSION_REVISION) "d"
#else
#define CV_VERSION_NAME CVAUX_STR(CV_VERSION_MAJOR)  CVAUX_STR(CV_VERSION_MINOR) CVAUX_STR(CV_VERSION_REVISION)
#endif // DEBUG

#pragma comment(lib,"opencv_core" CV_VERSION_NAME ".lib")
#pragma comment(lib,"opencv_highgui" CV_VERSION_NAME ".lib")
#pragma comment(lib,"opencv_imgproc" CV_VERSION_NAME ".lib")
#pragma comment(lib,"opencv_photo" CV_VERSION_NAME ".lib")
#pragma comment(lib,"opencv_objdetect" CV_VERSION_NAME ".lib")
#if (CV_VERSION_MAJOR < 3)
#pragma comment(lib,"opencv_legacy" CV_VERSION_NAME ".lib")
#endif
#endif // !_MSC_VER


#define TEST(func,...)	\
	std::cout<<"start "#func" ..."<<endl;	\
	func(__VA_ARGS__);	\
	std::cout<<"end "#func"!!!"<<endl;

extern int CannyEdgeDetection();
extern int ContourDetection();
extern int rotateAndScale();
extern int videoAndMovingObjectDetection();
extern int houghLineDetection();
extern int mousePainter();
extern int snakeContour();
extern int FaceDetection(int, char**);
extern int FrequencyDomainWatermark();
using namespace std;

int main(int c,char**v)
{
    printf("OpenCV Version:" CV_VERSION "\n");

	// Canny边缘检测
	//TEST(CannyEdgeDetection);
	
	// 轮廓检测
	//TEST(ContourDetection);

	// 图像旋转与缩放
	//TEST(rotateAndScale);

	// 读取视频文件和运动物体检测
	//TEST(videoAndMovingObjectDetection);

	// Hough线段检测
	//TEST(houghLineDetection);

	// 鼠标绘图
	//TEST(mousePainter);

	// Snake轮廓检测
	//TEST(snakeContour);

	// 人脸检测
	//TEST(FaceDetection,c,v);

    // 频域水印
    TEST(FrequencyDomainWatermark);

	//// 加载图片
	//cv::Mat img = cv::imread("D:\\OpenCV\\doc\\opencv-logo2.png");
	//if (img.empty()) {
	//	cerr << "open image failed!!!" << endl;
	//	return -1;
	//}
	//// 创建一个窗口
	////cv::namedWindow("Show Image");
	//// 在窗口显示图片
	//cv::imshow("opencv-logo2.png", img);
	//// 等待按任意键
	//cv::waitKey();
	getchar();
	return 0;
}