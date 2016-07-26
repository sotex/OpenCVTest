#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor


// http://blog.csdn.net/Scythe666/article/details/38147051
// http://blog.csdn.net/crzy_sparrow/article/details/7414851/
// http://www.cnblogs.com/justany/archive/2012/11/21/2779978.html

int videoAndMovingObjectDetection()
{
	cv::VideoCapture capture;	// 视频捕获对象

	// 创建窗口
	cv::namedWindow("video");		// 视频窗口
	cv::namedWindow("background");	// 背景
	cv::namedWindow("foreground");	// 前景
	// 排列窗口位置
	cv::moveWindow("video", 100, 100);
	cv::moveWindow("background", 580, 100);
	cv::moveWindow("foreground", 1060, 100);


	// 打开摄像头
	//if (!capture.open(-1)) {
	//	puts("打开摄像头失败!!!");
	//	return -1;
	//}

	// 打开视频文件
	if (!capture.open("../Image/video.short.mjpg.avi")) {
		puts("打开视频文件失败!!!");
		return -1;
	}

	cv::Mat	frameImage;	// 存储视频帧
	cv::Mat	backImage, foreImage, grayImage;	// 前景、背景和帧图像的灰度帧
	cv::Mat	binaryImage;	// 保存二值化的图像(计算结果)

	int	frameCount = 0;	// 帧计数

	while (capture.read(frameImage)) {
		++frameCount;
		// 如果是第一帧，申请内存
		if (frameCount == 1) {
			backImage.create(frameImage.size(), CV_32FC1);	// 注意这里是32位float来存储
			foreImage.create(frameImage.size(), CV_32FC1);	// 因为后面进行计算的时候需要高精度
			grayImage.create(frameImage.size(), CV_32FC1);

			binaryImage.create(frameImage.size(), CV_8UC1);	// 二值化只需要这么高的精度

			// 转换帧图像为灰度图像存储为前景和背景
			cv::cvtColor(frameImage, backImage, CV_BGR2GRAY);
			cv::cvtColor(frameImage, foreImage, CV_BGR2GRAY);
			continue;
		}

			// 转换帧图像为灰度图像
		cv::cvtColor(frameImage, grayImage, CV_BGR2GRAY);

		// 高斯滤波，平滑当前图像
		//cv::GaussianBlur(
		//	grayImage/*输入图像*/,
		//	grayImage/*输出图像*/,
		//	cv::Size(3, 3)/*高斯内核大小*/,
		//	0.0 /*高斯内核在X方向的标准偏差*/,
		//	0.0 /*高斯内核在Y方向的标准偏差(如果为0，将于sigmaX相同)*/,
		//	4/*用于判断图像边界的模式*/);

		// 当前帧与背景图相减(结果存入前景图)
		cv::absdiff(grayImage, backImage, foreImage);
		// 二值化前景图
		cv::threshold(foreImage, binaryImage,
			60.0, 255.0/*对得到的前景进行阈值选取，去掉伪前景*/,
			CV_THRESH_BINARY);

		// 进行形态学滤波，去掉噪音
		//cv::erode(binaryImage,binaryImage)
		//cv::dilate()

		// 更新背景(http://blog.csdn.net/brilliantstone/article/details/10161313)
		// cv::accumulateWeighted(grayImage, backImage, 0.05,binaryImage);
		// 不知为何加了此句便会崩溃http://answers.opencv.org/question/63781/accumulateweighted-problems/


		// 显示图像
		cv::imshow("video", frameImage);
		cv::imshow("background", backImage);
		cv::imshow("foreground", foreImage);

		// 等待按键事件
		// 此处等待也为显示图像函数提供足够的时间完成显示
		// 等待事件可以按照CPU速度进行调节
		if (cv::waitKey(2) >= 0) {
			break;	// 按键就退出
		}


	}
	return 0;
}