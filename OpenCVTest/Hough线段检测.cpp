#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


// http://blog.csdn.net/zhaocj/article/details/50281537

int houghLineDetection()
{
	// ����ԭͼ��
	cv::Mat src = cv::imread("../Image/houghline2.jpg");
	if (src.empty()) {
		puts("ԭͼ���ȡʧ��!!!");
		return -1;
	}
	cv::Mat	gray(src.size(), CV_8UC1);	// ԭͼ�ĻҶ�ͼ��
	cv::cvtColor(src, gray, CV_BGR2GRAY);

	cv::Mat	edge(src.size(), CV_8UC1);	// ��Եͼ��
	cv::Mat dst(src.size(), src.type());// ����ֱ�ߵ�Ŀ��ͼ��
	cv::cvtColor(gray, dst, CV_GRAY2BGR);	// �ӻҶ�ͼת���������ں�����ƺ�ɫ������

	// ����Canny��Ե���
	cv::Canny(gray, edge, 50, 200, 3);

#if 0
	// ����������飬���ڴ洢ֱ�ߵĽǶȺ;�������������  
	std::vector<cv::Vec2f> lines;
	// ����ֱ���Ϊ1���Ƕȷֱ���Ϊ��/180����ֵΪ215  
	// ��ֵ��ѡȡֱ��Ӱ�쵽���ֱ�ߵ�����
	cv::HoughLines(edge, lines, CV_HOUGH_PROBABILISTIC,
		CV_PI / 180.0,215,0,0);
	// ��ֱ��
	for (size_t i = 0; i < lines.size(); ++i) {
		// ��ȡ������ͽǶ�
		float rho = lines[i][0];
		float theta = lines[i][1];

		// ���������㣬ȷ��һ��ֱ��
		cv::Point p1, p2;
		// ����õ������������
		// ����cos��-1000sin�ȣ���sin��+1000cos�ȣ�������cos��+1000sin�ȣ���sin��-1000cos�ȣ�  
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a*rho;
		double y0 = b*rho;
		p1.x = cvRound(x0 + 1000 * (-b));
		p1.y = cvRound(y0 + 1000 * a);
		p2.x = cvRound(x0 - 1000 * (-b));
		p2.y = cvRound(y0 - 1000 * a);
		// �ڻҶ�ͼ�ϻ����Ϊ2�ĺ���
		cv::line(dst, p1, p2, cv::Scalar(0, 0, 255), 2, 8);
	}
#else
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(edge, lines, CV_HOUGH_PROBABILISTIC,
		CV_PI / 180.0, 80, 30, 10);
	for (size_t i = 0; i < lines.size(); ++i) {
		cv::line(dst, cv::Point(lines[i][0], lines[i][1]),
			cv::Point(lines[i][2], lines[i][3]), cv::Scalar(0, 0, 255), 2, 8);
	}

#endif

	// ����������ʾԭͼ��Ŀ��ͼ
	cv::namedWindow("src");
	cv::namedWindow("dst");
	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::imshow("edge", edge);

	// �ȴ������˳�
	cv::waitKey();
	return 0;
}