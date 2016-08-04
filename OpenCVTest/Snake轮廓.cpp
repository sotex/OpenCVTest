#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/legacy/legacy.hpp>	// cvSnakeImage

// http://blog.csdn.net/hongxingabc/article/details/51606520
//http://wiki.opencv.org.cn/index.php/Snake%E8%BD%AE%E5%BB%93%E4%BE%8B%E5%AD%90

static cv::Mat	src, src_bak;	// ԭʼͼ���䱸��

static int	thresholdness = 128;	// ��ֵ
// ����Ĳ������ڻ����ֵ����
static int ialpha = 20;	// alpha������໥��£��Ȩֵ(0-1.0)
static int ibeta  = 20;	// beta��ʾ����������ԽСԽ����������(0-1.0)
static int igamma = 20;	// gamma��ʾ��������(0-1.0)

void on_change(int pos,void*);

int snakeContour()
{
	// ��������
	cv::namedWindow("win", 0);
	// ����ĸ�������(������)
	cv::createTrackbar("Thd", "win", &thresholdness, 255, on_change);
	cv::createTrackbar("alpha", "win", &ialpha, 100, on_change);
	cv::createTrackbar("beta", "win", &ibeta, 100, on_change);
	cv::createTrackbar("gamma", "win", &igamma, 100, on_change);
	
	cv::resizeWindow("win", 640, 480);
	// ����һ��on_change���Ա���ʾ������
	on_change(0,NULL);

	while (true){
		if (cv::waitKey(40) == 27) {
			cv::destroyWindow("win");
			break;	// ����ESC�����˳�
		}
		// ��ʾͼ�񵽴���
		cv::imshow("win", src_bak);
	}

	return 0;
}

void on_change(int,void*)
{
	// ��ȡԭͼ��
	src_bak = cv::imread("../Image/sisy.jpg", 1);
	// ת��Ϊ�Ҷ�ͼ��
	cv::cvtColor(src_bak, src, CV_BGR2GRAY);

	// �Ե�ͨ������Ӧ�ù̶���ֵ�������ú����ĵ���Ӧ���ǶԻҶ�ͼ�������ֵ�����õ���ֵͼ��
	// ��ֵ������
	cv::threshold(src/*ԭʼͼ��(��ͨ��)*/,
		src/*Ŀ��ͼ��(��ԭʼͼ������һ��)*/,
		thresholdness/*��ֵ��������ֵ*/,
		255/*ʹ�� CV_THRESH_BINARY �� CV_THRESH_BINARY_INV �����ֵ*/,
		CV_THRESH_BINARY/*��ֵ����*/);
	// CV_THRESH_BINARY:��� src(x, y)>threshold, dst(x, y) = max_value; ����, dst��x, y�� = 0;
	// CV_THRESH_BINARY_INV:��� src(x, y)>threshold, dst(x, y) = 0; ����, dst(x, y) = max_value.
	// CV_THRESH_TRUNC:��� src(x, y)>threshold��dst(x, y) = max_value; ����dst(x, y) = src(x, y).
	// CV_THRESH_TOZERO:���src(x, y)>threshold��dst(x, y) = src(x, y); ���� dst(x, y) = 0��
	// CV_THRESH_TOZERO_INV:��� src(x, y)>threshold��dst(x, y) = 0; ����dst(x, y) = src(x, y).
	
	std::vector<std::vector<cv::Point> > contours;

	// http://www.360doc.com/content/12/1028/14/6408986_244247418.shtml
	// Ѱ�ҳ�ʼ������
	cv::findContours(src/*����ͼ��(2ֵ��ͨ��ͼ��)*/,
		contours/*������������������*/,
		CV_RETR_EXTERNAL/*�������ģʽ*/,
		CV_CHAIN_APPROX_SIMPLE/*�������Ʒ�ʽ*/,
		cv::Point(0, 0)/*������ƫ����*/);

	if (contours.empty()) {
		return;	//û�м�⵽����
	}

	// ����������(���Ƶ����ݵ�ͼ����)
	cv::drawContours(src_bak, contours, -1, cv::Scalar(0, 0, 255), 2, 8);

	{
	// ��ȡalpha��beta��gamma����ֵ
		float alpha = ialpha / 100.0f;
		float beta = ibeta / 100.0f;
		float gamma = igamma / 100.0f;

		// �ı�����λ��ʹ������������С
		// ��Ϊû��C++�Ľӿڣ���������ʹ��C�Ľӿ�
		IplImage ipl_src = IplImage(src);
		
		// ÿ��������������Сֵ������ߴ磬���� win.width �� win.height ������������
		CvSize size;
		size.width = 3; size.height = 3;
		// ��ֹ����
		CvTermCriteria criteria;
		criteria.type = CV_TERMCRIT_ITER;
		criteria.max_iter = 1000;
		criteria.epsilon = 0.1;
		// �ݶȷ��š�������㣬����Ϊÿһ��ͼ�����ؼ����ݶȷ�ֵ���Ұ�������������������������ͼ����
		int calc_gradient = 1;

		std::vector<CvPoint> point_arr;	// ���ڻ�ȡһ����������������
		for (int nline = 0; nline < contours.size(); ++nline) {
			// ��ǰ�����ߵ���
			int point_count = contours[nline].size();
			if (point_count > 2) {	// ����3�����ʱ�򣬻����
				point_arr.resize(point_count);
			} else {
				continue;
			}
			// ����������
			for (int npt = 0; npt < contours[nline].size(); ++npt) {
				point_arr[npt] = contours[nline][npt];
			}
			// ִ�иı�����λ��ʹ������������С
			cvSnakeImage(&ipl_src/*����ͼ����ⲿ������*/,
				point_arr.data()/*���봫��������*/,
				point_count/*���������������С*/,
				&alpha/*������������Ȩ Weight[s]�������������򳤶�Ϊ length �ĸ��������飬ÿ����������һ��Ȩ*/,
				&beta/*����������Ȩ Weight[s]���� alpha ����*/,
				&gamma/*ͼ��������Ȩ Weight[s]���� alpha ����*/,
				CV_VALUE/*ǰ�����������Ĳ�ͬʹ�÷���*/,
				/*CV_VALUE ��ʾÿ�� alpha, beta, gamma ����ָ��Ϊ���е����õ�һ��������ֵ;
				CV_ARRAY ��ʾÿ�� alpha, beta, gamma ��һ��ָ��ϵ�������ָ�룬snake ��������ϵ��������ͬ��
				��ˣ�����ϵ�������������������ͬ���Ĵ�С�����������������������ͬ����С*/
				size, criteria, calc_gradient);

			// ��ʾ����(��ɫ)
			for (int prev = 0; prev < point_count; ++prev) {
				int next = (prev + 1) % point_count;
				// ���ߣ���ɫ
				cv::line(src_bak, point_arr[prev], point_arr[next], cv::Scalar(255, 0, 0), 2, 8, 0);
			}
		}
	}
}