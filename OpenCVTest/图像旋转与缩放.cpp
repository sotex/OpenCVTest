#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor


static int	scaleFactor = 50;	// ��������(50Ϊ1.0��+0.2��������0)
static int	angle = 0;			// ��ת�Ƕ�


void onTrackBar_rotate(int, void*);	// ��ת�������ص�����
void onTrackBar_scale(int, void*);	// ���ſ������ص�����

void update_show(cv::Mat* img);	// ������ʾ(��ʵ�������������������������)

int rotateAndScale()
{
	cv::Mat	src;	// Դͼ��

	src = cv::imread("../Image/sisy.jpg");
	if (src.empty()) {
		puts("ͼ�����ʧ��");
		return -1;
	}

	// ��ʾԴͼ��
	cv::imshow("src", src);

	// �������ڣ���ʾĿ��ͼ��Ϳ�����
	cv::namedWindow("dest", 1);
	// ���������������ڿ�������ֵ
	cv::createTrackbar("Scale Control", "dest",
		&scaleFactor, 100, onTrackBar_scale, &src);
	// ���������������ڿ�����תֵ
	cv::createTrackbar("Rotate Control", "dest",
		&angle, 360, onTrackBar_rotate, &src);

	// ��ˢ��һ��
	update_show(&src);

	cv::waitKey();
	return 0;
}

// ��ת�������ص�����
void onTrackBar_rotate(int, void* mat)
{
	update_show((cv::Mat*)mat);

}

// ���ſ������ص�����
void onTrackBar_scale(int, void* mat)
{
	update_show((cv::Mat*)mat);
}

void update_show(cv::Mat* img)
{
	cv::Mat	dst;	// Ŀ��ͼ��
	dst.create(img->size(), img->type());	// Ŀ��ͼ�������ڴ�

/*
	float m[6];	// ����任����
				// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
				// [ m3  m4  m5 ]       [ A21  A22   b2 ]


	// ������ת����ֵ
	float arc = angle*CV_PI / 180.0;
	// �任����(��Ҫ������ת������)
	float factor = (cos(arc) + 1.0) * 2;

	m[0] = factor * cos(-arc);
	m[1] = factor * sin(-arc);
	m[3] = -m[1];
	m[4] = m[0];
	// ����ת�����ƶ���ͼ������
	m[2] = img->size().width * 0.5f;
	m[5] = img->size().height * 0.5f;

	// dst(x,y) = A*src(x,y) + b
	cv::Mat M = cv::Mat(2, 3, CV_32F, m);
*/
	// ��ȡ�任����
	cv::Mat M = cv::getRotationMatrix2D(
		cv::Point2f(img->size().width * 0.5f, img->size().height * 0.5f),
		(double)angle,
		scaleFactor/50.0 +0.2);

	cv::warpAffine(*img, dst, M, dst.size());
	
	cv::imshow("dest", dst);
}