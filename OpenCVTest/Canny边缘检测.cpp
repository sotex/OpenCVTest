#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor

// http ://blog.csdn.net/hitwengqi/article/details/6877864

static cv::Mat image, edge;	// ԭʼͼƬ��canny��Ե����ͼƬ
static cv::Mat	gray, gedge;	// �Ҷ�ͼƬ��canny���ڱ���ͼƬ

int edgeThresh = 1;		// ���ƽ�����(�������������Ƶ�ֵ)

void onTrackBar(int, void*);

int CannyEdgeDetection()
{
	// ����ԭʼͼƬ
	image = cv::imread("../Image/sisy.jpg");
	// �ж������Ƿ�ɹ�
	if (image.empty()) {
		printf("����ͼƬʧ��\n");
		return -1;
	}
	// ���ɻҶ�ͼƬ����Ϊֻ�лҶ�ͼƬ�������ɱ�ԵͼƬ
	// gray.create(image.size(), image.type());	// ����create��û���õ�
	cv::cvtColor(image, gray, CV_BGR2GRAY);		// ������Զ�ȥcreate��ת����Ŀ�걣�����ͣ�����Ӧ����UINT8

	cv::imshow("gray", gray);

	// �½�һ������
	cv::namedWindow("Edge Map", 1);

	// ����һ�������������Ʊ�Ե���
	cv::createTrackbar("Canny Threshold", "Edge Map",
		&edgeThresh/*������������ֵ*/,100/*����������*/,
		onTrackBar /*������ֵ�ı�ʱ���õĺ���*/);

	// ��ʼ��ͼ��
	onTrackBar(0, NULL);

	// �ȴ�����
	cv::waitKey();
	return 0;
}


void onTrackBar(int, void*)
{
	// blur �Ҷ�ͼƬ(blur ģ��)
	cv::blur(gray, gedge, cv::Size(3, 3));
	// Canny ��Ե���
	cv::Canny(gray /*��ͨ������ͼ��.*/,
		gedge/*��ͨ���洢��Ե�����ͼ��*/,
		edgeThresh/*��һ����ֵ*/,
		edgeThresh * 3 /*�ڶ�����ֵ*/,
		3 /*Sobel�����ں˴�С*/);

	// ȫ������Ϊ0
	edge = cv::Scalar::all(0);

	// ������Ե���ص�
	image.copyTo(edge, gedge);

	// ��ʾͼƬ
	cv::imshow("Edge Map", edge);
}