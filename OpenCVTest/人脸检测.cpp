#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

static const char* cascade_name = "D:\\OpenCV\\share\\OpenCV\\haarcascades\\"
						   "haarcascade_frontalface_alt2.xml";


// http://wiki.opencv.org.cn/index.php/%E4%BA%BA%E8%84%B8%E6%A3%80%E6%B5%8B


void frameFaceDetection(cv::Mat& src, cv::CascadeClassifier& cascade,
	const char* showWindowName);

int FaceDetection(int c,char**v)
{
	//cv::Mat	src,gray;	// Դͼ�񣬻Ҷ�ͼ��
	
	cv::VideoCapture	capture;	// ��Ƶ�������
	cv::Mat				frame;		// ��Ƶ֡
	cv::CascadeClassifier	cascade;	//����������
	
	/**
	// ����Դͼ��
	const char* picpath = "../Image/sisy.jpg";
	if (c > 1) { picpath = v[1]; }
	src = cv::imread(picpath);

	if (src.empty()) {
		puts("ͼƬ����ʧ��!!!");
		return -1;
	}
	*/

	// ����Ƶ�ļ�
	if (!capture.open("../Image/111.mp4")) {
		puts("����Ƶ�ļ�ʧ��!!!");
		return -1;
	}

	// ��������ʶ�����������ļ�
	if (!cascade.load(cascade_name)){
		puts("����ʶ�����������ļ�����ʧ��!!!");
		return 0;
	}

	// ������ʾ����
	cv::namedWindow("src");

	cv::Mat	src_scale;
	src_scale.create(cv::Size(480, 320), CV_8SC3);
	// ѭ��ȡ֡����ʾ
	while (capture.read(frame)) {
		// ��������
		cv::resize(frame, src_scale, src_scale.size());
		// ��Ⲣ��ʾ
		frameFaceDetection(src_scale, cascade, "src");
		// �ȴ������¼�
		// �˴��ȴ�ҲΪ��ʾͼ�����ṩ�㹻��ʱ�������ʾ
		// �ȴ��¼����԰���CPU�ٶȽ��е���
		if (cv::waitKey(2) >= 0) {
			break;	// �������˳�
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

	// ��ȡԴͼ��ĻҶ�ͼ��
	//gray.create(src.size(), CV_8UC1);
	cv::cvtColor(src, gray, CV_BGR2BGRA);
	// ʹ�Ҷ�ͼ��ֱ��ͼ���⻯
	//cv::equalizeHist(gray, gray);

	// ��ȡ���������
	cascade.detectMultiScale(
		gray/*Դͼ��*/,
		faces/*�����������Ե(�õ����������ľ��ο�������)*/,
		1.1/*ÿһ��ͼ��߶��еĳ߶Ȳ�����Ĭ��ֵΪ1.1*/,
		2/*ÿһ����������Ӧ�ñ������ڽ�����*/,
		CV_HAAR_SCALE_IMAGE,
		cv::Size(30, 30)/*��С���ܵĶ���Ĵ�С��С�ڵĶ��󽫱�����*/);

	// �����õ��Ľ�������Ƶ�ԭͼ����
	for (auto face : faces) {
		// ���ƺ�ɫ����
		cv::rectangle(src, face, cv::Scalar(0, 0, 255), 3);
	}
	// ��ʾͼ��
	cv::imshow(showWindowName, src);
}