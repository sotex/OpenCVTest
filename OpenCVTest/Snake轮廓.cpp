#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


//http://wiki.opencv.org.cn/index.php/Snake%E8%BD%AE%E5%BB%93%E4%BE%8B%E5%AD%90

static cv::Mat	src, src_bak;	// 原始图像及其备份

int	thresholdness = 141;
int ialpha = 20;
int ibeta  = 20;
int igamma = 20;

void on_change(int pos);

int snakeContour()
{
	return 0;
}