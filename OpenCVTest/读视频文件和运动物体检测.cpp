#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor


// http://blog.csdn.net/Scythe666/article/details/38147051
// http://blog.csdn.net/crzy_sparrow/article/details/7414851/
// http://www.cnblogs.com/justany/archive/2012/11/21/2779978.html

int videoAndMovingObjectDetection()
{
	cv::VideoCapture capture;	// ��Ƶ�������

	// ��������
	cv::namedWindow("video");		// ��Ƶ����
	cv::namedWindow("background");	// ����
	cv::namedWindow("foreground");	// ǰ��
	// ���д���λ��
	cv::moveWindow("video", 100, 100);
	cv::moveWindow("background", 580, 100);
	cv::moveWindow("foreground", 1060, 100);


	// ������ͷ
	//if (!capture.open(-1)) {
	//	puts("������ͷʧ��!!!");
	//	return -1;
	//}

	// ����Ƶ�ļ�
	if (!capture.open("../Image/video.short.mjpg.avi")) {
		puts("����Ƶ�ļ�ʧ��!!!");
		return -1;
	}

	cv::Mat	frameImage;	// �洢��Ƶ֡
	cv::Mat	backImage, foreImage, grayImage;	// ǰ����������֡ͼ��ĻҶ�֡
	cv::Mat	binaryImage;	// �����ֵ����ͼ��(������)

	int	frameCount = 0;	// ֡����

	while (capture.read(frameImage)) {
		++frameCount;
		// ����ǵ�һ֡�������ڴ�
		if (frameCount == 1) {
			backImage.create(frameImage.size(), CV_32FC1);	// ע��������32λfloat���洢
			foreImage.create(frameImage.size(), CV_32FC1);	// ��Ϊ������м����ʱ����Ҫ�߾���
			grayImage.create(frameImage.size(), CV_32FC1);

			binaryImage.create(frameImage.size(), CV_8UC1);	// ��ֵ��ֻ��Ҫ��ô�ߵľ���

			// ת��֡ͼ��Ϊ�Ҷ�ͼ��洢Ϊǰ���ͱ���
			cv::cvtColor(frameImage, backImage, CV_BGR2GRAY);
			cv::cvtColor(frameImage, foreImage, CV_BGR2GRAY);
			continue;
		}

			// ת��֡ͼ��Ϊ�Ҷ�ͼ��
		cv::cvtColor(frameImage, grayImage, CV_BGR2GRAY);

		// ��˹�˲���ƽ����ǰͼ��
		//cv::GaussianBlur(
		//	grayImage/*����ͼ��*/,
		//	grayImage/*���ͼ��*/,
		//	cv::Size(3, 3)/*��˹�ں˴�С*/,
		//	0.0 /*��˹�ں���X����ı�׼ƫ��*/,
		//	0.0 /*��˹�ں���Y����ı�׼ƫ��(���Ϊ0������sigmaX��ͬ)*/,
		//	4/*�����ж�ͼ��߽��ģʽ*/);

		// ��ǰ֡�뱳��ͼ���(�������ǰ��ͼ)
		cv::absdiff(grayImage, backImage, foreImage);
		// ��ֵ��ǰ��ͼ
		cv::threshold(foreImage, binaryImage,
			60.0, 255.0/*�Եõ���ǰ��������ֵѡȡ��ȥ��αǰ��*/,
			CV_THRESH_BINARY);

		// ������̬ѧ�˲���ȥ������
		//cv::erode(binaryImage,binaryImage)
		//cv::dilate()

		// ���±���(http://blog.csdn.net/brilliantstone/article/details/10161313)
		// cv::accumulateWeighted(grayImage, backImage, 0.05,binaryImage);
		// ��֪Ϊ�μ��˴˾������http://answers.opencv.org/question/63781/accumulateweighted-problems/


		// ��ʾͼ��
		cv::imshow("video", frameImage);
		cv::imshow("background", backImage);
		cv::imshow("foreground", foreImage);

		// �ȴ������¼�
		// �˴��ȴ�ҲΪ��ʾͼ�����ṩ�㹻��ʱ�������ʾ
		// �ȴ��¼����԰���CPU�ٶȽ��е���
		if (cv::waitKey(2) >= 0) {
			break;	// �������˳�
		}


	}
	return 0;
}