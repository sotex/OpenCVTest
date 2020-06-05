#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>	// cv::inpaint

static cv::Mat	src,src_bak;	// 原图
static cv::Mat	inpainted, inpaint_mask;	// 绘制鼠标/掩码图
static cv::Point	prev_pt = { -1,-1 };	// 鼠标左键按下时候的鼠标位置或鼠标移动时候的起点位置


void on_mouse(int event, int x, int y, int flags, void* zhang);

int mousePainter()
{
	// 打开原图
	src = cv::imread("../Image/sisy.jpg");
	if (src.empty()) {
		puts("打开图像失败!!!");
		return -1;
	}
	src.copyTo(src_bak);	// 备份一下
	// 
	inpainted.create(src.size(), src.type());
	src.copyTo(inpainted);
	inpaint_mask = cv::Mat::zeros(src.size(), CV_8UC1);

	cv::namedWindow("src");
	cv::imshow("src", src);	// 显示原图
	cv::namedWindow("watershed transform");
	cv::imshow("watershed transform", inpainted);	// 分水岭变换

	cv::setMouseCallback("src", on_mouse);	// 鼠标事件回调

	while (true) {
		int key = cv::waitKey(0);	// 等待按键
		if (key == 27) {
			break;	// 按下的是ESC键
		}
		if (key == 'r') {
			// 按下r 键的时候，重来
			inpaint_mask = cv::Mat::zeros(src.size(), CV_8UC1);
			src_bak.copyTo(src);
			cv::imshow("src", src);
		}
		if (key == 'i' || key == '\n') {
			cv::namedWindow("inpainted image");
			// http://www.tuicool.com/articles/rymYNn
			// http://docs.opencv.org/2.4/modules/photo/doc/inpainting.html
			//src:要修复的图像；
			//mask：修复模板，必须是单通道图像；
			//dst：目标图像；
			//inpaintRange：选取邻域半径;
			//flags:要使用的方法，可以是CV INPAINT NS或CV INPAINT TELEA
			cv::inpaint(src, inpaint_mask, inpainted, 3, cv::INPAINT_TELEA);
			cv::imshow("inpainted image", inpainted);
		}
	}
	return 0;
}


void on_mouse(int event, int x, int y, int flags, void* zhang)
{
	if (event == cv::EVENT_LBUTTONUP  || !(flags & cv::EVENT_FLAG_LBUTTON)) {
		prev_pt = cv::Point(-1, -1);	// 鼠标左键弹起或鼠标左键没有按下
	}
	else if (event == cv::EVENT_LBUTTONDOWN) {
		prev_pt = cv::Point(x, y);	// 鼠标左键按下，记录位置
	}
	else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON)) {
		// 鼠标移动，并且左键是按下的
		cv::Point	pt(x, y);
		if (prev_pt.x < 0) {
			prev_pt = pt;
		}
		// 绘线(红色，宽度为5)
		cv::line(src, prev_pt, pt, cv::Scalar(0,0,255), 5, 8, 0);	// 绘制到原图
		
		// 绘制到inpaint_mask上，后面修复时候用(要注意，是单通道的)
		cv::line(inpaint_mask, prev_pt, pt, cv::Scalar(255), 5, 8, 0);
		prev_pt = pt;
		cv::imshow("src", src);
	}
}