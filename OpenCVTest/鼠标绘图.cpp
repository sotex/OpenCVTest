#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>	// cv::inpaint

cv::Mat	src,src_bak;	// ԭͼ
cv::Mat	inpainted, inpaint_mask;	// �������/����ͼ
cv::Point	prev_pt = { -1,-1 };	// ����������ʱ������λ�û�����ƶ�ʱ������λ��


void on_mouse(int event, int x, int y, int flags, void* zhang);

int mousePainter()
{
	// ��ԭͼ
	src = cv::imread("../Image/sisy.jpg");
	if (src.empty()) {
		puts("��ͼ��ʧ��!!!");
		return -1;
	}
	src.copyTo(src_bak);	// ����һ��
	// 
	inpainted.create(src.size(), src.type());
	src.copyTo(inpainted);
	inpaint_mask = cv::Mat::zeros(src.size(), CV_8UC1);

	cv::namedWindow("src");
	cv::imshow("src", src);	// ��ʾԭͼ
	cv::namedWindow("watershed transform");
	cv::imshow("watershed transform", inpainted);	// ��ˮ��任

	cv::setMouseCallback("src", on_mouse);	// ����¼��ص�

	while (true) {
		int key = cv::waitKey(0);	// �ȴ�����
		if (key == 27) {
			break;	// ���µ���ESC��
		}
		if (key == 'r') {
			// ����r ����ʱ������
			inpaint_mask = cv::Mat::zeros(src.size(), CV_8UC1);
			src_bak.copyTo(src);
			cv::imshow("src", src);
		}
		if (key == 'i' || key == '\n') {
			cv::namedWindow("inpainted image");
			// http://www.tuicool.com/articles/rymYNn
			// http://docs.opencv.org/2.4/modules/photo/doc/inpainting.html
			//src:Ҫ�޸���ͼ��
			//mask���޸�ģ�壬�����ǵ�ͨ��ͼ��
			//dst��Ŀ��ͼ��
			//inpaintRange��ѡȡ����뾶;
			//flags:Ҫʹ�õķ�����������CV INPAINT NS��CV INPAINT TELEA
			cv::inpaint(src, inpaint_mask, inpainted, 3, CV_INPAINT_TELEA);
			cv::imshow("inpainted image", inpainted);
		}
	}
	return 0;
}


void on_mouse(int event, int x, int y, int flags, void* zhang)
{
	if (event == CV_EVENT_LBUTTONUP  || !(flags & CV_EVENT_FLAG_LBUTTON)) {
		prev_pt = cv::Point(-1, -1);	// �����������������û�а���
	}
	else if (event == CV_EVENT_LBUTTONDOWN) {
		prev_pt = cv::Point(x, y);	// ���������£���¼λ��
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON)) {
		// ����ƶ�����������ǰ��µ�
		cv::Point	pt(x, y);
		if (prev_pt.x < 0) {
			prev_pt = pt;
		}
		// ����(��ɫ�����Ϊ5)
		cv::line(src, prev_pt, pt, cv::Scalar(0,0,255), 5, 8, 0);	// ���Ƶ�ԭͼ
		
		// ���Ƶ�inpaint_mask�ϣ������޸�ʱ����(Ҫע�⣬�ǵ�ͨ����)
		cv::line(inpaint_mask, prev_pt, pt, cv::Scalar(255), 5, 8, 0);
		prev_pt = pt;
		cv::imshow("src", src);
	}
}