#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor

// http ://blog.csdn.net/hitwengqi/article/details/6877864

static cv::Mat image, edge;	// 原始图片，canny边缘保存图片
static cv::Mat	gray, gedge;	// 灰度图片，canny便于保存图片

int edgeThresh = 1;		// 控制进度条(传出进度条控制的值)

void onTrackBar(int, void*);

int CannyEdgeDetection()
{
	// 加载原始图片
	image = cv::imread("../Image/sisy.jpg");
	// 判断载入是否成功
	if (image.empty()) {
		printf("载入图片失败\n");
		return -1;
	}
	// 生成灰度图片，因为只有灰度图片才能生成边缘图片
	// gray.create(image.size(), image.type());	// 这里create是没有用的
	cv::cvtColor(image, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY);		// 这里会自动去create到转换的目标保存类型，这里应该是UINT8

	cv::imshow("gray", gray);

	// 新建一个窗口
	cv::namedWindow("Edge Map", 1);

	// 生成一个进度条来控制边缘检测
	cv::createTrackbar("Canny Threshold", "Edge Map",
		&edgeThresh/*传出进度条的值*/,100/*进度条长度*/,
		onTrackBar /*进度条值改变时调用的函数*/);

	// 初始化图像
	onTrackBar(0, NULL);

	// 等待按键
	cv::waitKey();
	return 0;
}


void onTrackBar(int, void*)
{
	// blur 灰度图片(blur 模糊)
	cv::blur(gray, gedge, cv::Size(3, 3));
	// Canny 边缘检测
	cv::Canny(gray /*单通道输入图像.*/,
		gedge/*单通道存储边缘的输出图像*/,
		edgeThresh/*第一个阈值*/,
		edgeThresh * 3 /*第二个阈值*/,
		3 /*Sobel算子内核大小*/);

	// 全部设置为0
	edge = cv::Scalar::all(0);

	// 拷贝边缘像素点
	image.copyTo(edge, gedge);

	// 显示图片
	cv::imshow("Edge Map", edge);
}