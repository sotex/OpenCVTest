#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor


static int	scaleFactor = 50;	// 缩放因子(50为1.0倍+0.2避免缩到0)
static int	angle = 0;			// 旋转角度


void onTrackBar_rotate(int, void*);	// 旋转控制条回调函数
void onTrackBar_scale(int, void*);	// 缩放控制条回调函数

void update_show(cv::Mat* img);	// 更新显示(其实两个进度条都调用这个就行了)

int rotateAndScale()
{
	cv::Mat	src;	// 源图像

	src = cv::imread("../Image/sisy.jpg");
	if (src.empty()) {
		puts("图像加载失败");
		return -1;
	}

	// 显示源图像
	cv::imshow("src", src);

	// 创建窗口，显示目标图像和控制条
	cv::namedWindow("dest", 1);
	// 创建进度条，用于控制缩放值
	cv::createTrackbar("Scale Control", "dest",
		&scaleFactor, 100, onTrackBar_scale, &src);
	// 创建进度条，用于控制旋转值
	cv::createTrackbar("Rotate Control", "dest",
		&angle, 360, onTrackBar_rotate, &src);

	// 先刷新一下
	update_show(&src);

	cv::waitKey();
	return 0;
}

// 旋转控制条回调函数
void onTrackBar_rotate(int, void* mat)
{
	update_show((cv::Mat*)mat);

}

// 缩放控制条回调函数
void onTrackBar_scale(int, void* mat)
{
	update_show((cv::Mat*)mat);
}

void update_show(cv::Mat* img)
{
	cv::Mat	dst;	// 目标图像
	dst.create(img->size(), img->type());	// 目标图像申请内存

/*
	float m[6];	// 仿射变换矩阵
				// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
				// [ m3  m4  m5 ]       [ A21  A22   b2 ]


	// 计算旋转缩放值
	float arc = angle*CV_PI / 180.0;
	// 变换因子(需要考虑旋转和缩放)
	float factor = (cos(arc) + 1.0) * 2;

	m[0] = factor * cos(-arc);
	m[1] = factor * sin(-arc);
	m[3] = -m[1];
	m[4] = m[0];
	// 将旋转中心移动到图像中心
	m[2] = img->size().width * 0.5f;
	m[5] = img->size().height * 0.5f;

	// dst(x,y) = A*src(x,y) + b
	cv::Mat M = cv::Mat(2, 3, CV_32F, m);
*/
	// 获取变换矩阵
	cv::Mat M = cv::getRotationMatrix2D(
		cv::Point2f(img->size().width * 0.5f, img->size().height * 0.5f),
		(double)angle,
		scaleFactor/50.0 +0.2);

	cv::warpAffine(*img, dst, M, dst.size());
	
	cv::imshow("dest", dst);
}