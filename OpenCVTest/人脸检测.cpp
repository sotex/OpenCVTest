#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#include <cstdio>
// g++ face.cpp -o face -std=c++11 -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect

static const char* cascade_name = "D:\\OpenCV\\share\\OpenCV\\haarcascades\\"
						   "haarcascade_frontalface_alt2.xml";


// http://wiki.opencv.org.cn/index.php/%E4%BA%BA%E8%84%B8%E6%A3%80%E6%B5%8B


void frameFaceDetection(cv::Mat& src, cv::CascadeClassifier& cascade,
	const char* showWindowName);

int FaceDetection(int c,char**v)
{
	//cv::Mat	src,gray;	// 源图像，灰度图像
	
	cv::VideoCapture	capture;	// 视频捕获对象
	cv::Mat				frame;		// 视频帧
	cv::CascadeClassifier	cascade;	//级联分类器
	
	/**
	// 加载源图像
	const char* picpath = "../Image/sisy.jpg";
	if (c > 1) { picpath = v[1]; }
	src = cv::imread(picpath);

	if (src.empty()) {
		puts("图片加载失败!!!");
		return -1;
	}
	*/

	// 打开视频文件
	if (!capture.open("../Image/video.mov")) {
		puts("打开视频文件失败!!!");
		return -1;
	}

	// 加载人脸识别级联分类器文件
	if (!cascade.load(cascade_name)){
		puts("人脸识别级联分类器文件加载失败!!!");
		return 0;
	}

	// 创建显示窗口
	cv::namedWindow("src");

	cv::Mat	src_scale;
	src_scale.create(cv::Size(480, 320), CV_8SC3);
	// 循环取帧并显示
	while (capture.read(frame)) {
		// 进行缩放
		cv::resize(frame, src_scale, src_scale.size());
		// 检测并显示
		frameFaceDetection(src_scale, cascade, "src");
		// 等待按键事件
		// 此处等待也为显示图像函数提供足够的时间完成显示
		// 等待事件可以按照CPU速度进行调节
		if (cv::waitKey(1) >= 0) {
			break;	// 按键就退出
		}
	}
	//cv::waitKey();
	cv::destroyAllWindows();
	return 0;
}


void frameFaceDetection(cv::Mat& src,
	cv::CascadeClassifier& cascade,
	const char* showWindowName)
{
	std::vector<cv::Rect>	faces;
	cv::Mat gray;

	// 获取源图像的灰度图像
	gray.create(src.size(), CV_8UC1);
	cv::cvtColor(src, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY);
	// 使灰度图象直方图均衡化
	cv::equalizeHist(gray, gray);

	// 获取初步检测结果
	cascade.detectMultiScale(
		gray/*源图像*/,
		faces/*检测出的物体边缘(得到被检测物体的矩形框向量组)*/,
		1.1/*每一个图像尺度中的尺度参数，默认值为1.1*/,
		2/*每一个级联矩形应该保留的邻近个数*/,
		cv::CASCADE_SCALE_IMAGE,
		cv::Size(30, 30)/*最小可能的对象的大小，小于的对象将被忽略*/);

	// 将检测得到的结果，绘制到原图像上
	for (auto face : faces) {
		// 绘制红色矩形
		cv::rectangle(src, face, cv::Scalar(0, 0, 255), 3);
	}
	// 显示图像
	cv::imshow(showWindowName, src);
}