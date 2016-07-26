#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor

int contours_test();

int ContourDetection()
{
	
	//return contours_test();
	cv::Mat	image;	// ����ԭʼͼƬ
	cv::Mat	gary;	// �洢�Ҷ�ͼ��
	cv::Mat	dstimg;	// ��������Ŀ��ͼƬ	

	// ������������
	cv::namedWindow("src");	// ԭʼͼƬ��ʾ����
	cv::namedWindow("dst");	// ����ͼƬ��ʾ����

	// ����ԭʼͼƬ
	image = cv::imread("../Image/sisy.jpg");
	if (image.empty()) {
		puts("ͼƬ����ʧ��");
		return -1;
	}
	cv::imshow("src", image);	// ��ʾԭʼͼƬ

	gary.create(image.size(), CV_8U);	// ����Ҷ�ͼ�洢�ռ�
	cv::cvtColor(image, gary, CV_BGR2GRAY);	// ת��ԭʼͼΪ�Ҷ�ͼ
	cv::threshold(gary, gary, 128, 255, cv::THRESH_BINARY);	// ת��Ϊ��ֵͼ	

	std::vector<std::vector<cv::Point> >	contours;	// ������������
	std::vector<cv::Vec4i>					hierarchy;	// 
	int	mode = CV_RETR_EXTERNAL;	// �������ģʽ
	//mode��ʾ�����ļ���ģʽ
	//	CV_RETR_EXTERNAL��ʾֻ���������
	//	CV_RETR_LIST���������������ȼ���ϵ
	//	CV_RETR_CCOMP���������ȼ��������������һ��Ϊ��߽磬�����һ��Ϊ�ڿ׵ı߽���Ϣ������ڿ��ڻ���һ����ͨ���壬�������ı߽�Ҳ�ڶ��㡣
	//	CV_RETR_TREE����һ���ȼ����ṹ������������ο�contours.c���demo

	int method = CV_CHAIN_APPROX_SIMPLE;
	//methodΪ�����Ľ��ư취
	//	CV_CHAIN_APPROX_NONE�洢���е������㣬���ڵ������������λ�ò����1����max��abs��x1 - x2����abs��y2 - y1���� == 1
	//	CV_CHAIN_APPROX_SIMPLEѹ��ˮƽ���򣬴�ֱ���򣬶Խ��߷����Ԫ�أ�ֻ�����÷�����յ����꣬����һ����������ֻ��4����������������Ϣ
	//	CV_CHAIN_APPROX_TC89_L1��CV_CHAIN_APPROX_TC89_KCOSʹ��teh-Chinl chain �����㷨


	// ����contour
	cv::findContours(gary/*����ͼ��(����Ϊһ��2ֵ��ͨ��ͼ��)*/,
		contours/*, hierarchy*/, mode, method);


	// Ϊ������ʾͼƬ����ռ�
	dstimg = cv::Mat(image.size(), CV_8UC3); // 3ͨ��ͼ���Ա��ɫ��ʾ
	image.copyTo(dstimg);					 // ����Դͼ��

	// ����������
	cv::drawContours(dstimg/*Ŀ��ͼ��*/,
		contours/*�����������*/,
		-1 /*ָ�����ڼ�������(��ֵ��ʾȫ������)*/,
		cv::Scalar(0,0,255)/*������ɫBGR(�˴��Ժ�ɫ����)*/,
		2 /*�����߿�*/,
		8 /*��������*/,
		cv::noArray()/*�����ṹ��Ϣ*/);


	// ��ʾ����ͼƬ
	cv::imshow("dst", dstimg);

	// �ȴ�����
	cv::waitKey();
}

int contours_test()
{
	std::string image_name = "../Image/sisy.jpg";

	cv::Mat src = cv::imread(image_name);
	cv::imshow("src", src);

	cv::Mat gray(src.size(), CV_8U);
	cv::cvtColor(src, gray, CV_BGR2GRAY);//ת���ɻҶ�ͼ  

	cv::imshow("gray", gray);

	cv::threshold(gray, gray, 128, 255, cv::THRESH_BINARY);//ת����2ֵͼ��  
	cv::imshow("binary", gray);

	/////////////////////////////////////////////////////////////////////  
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(gray,
		contours, // a vector of contours   
		CV_RETR_EXTERNAL, // retrieve the external contours  
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours  

							   // Print contours' length  
	// std::cout << "Contours: " << contours.size() << std::endl;
	std::vector<std::vector<cv::Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours)
	{

		//std::cout << "Size: " << itContours->size() << std::endl;
	}

	// draw black contours on white image  
	cv::Mat result(gray.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(result, contours,
		-1, // draw all contours  
		cv::Scalar(0), // in black  
		2); // with a thickness of 2  

	cv::namedWindow("Contours");
	cv::imshow("Contours", result);


	// draw contours on the original image  
	cv::Mat original = cv::imread(image_name);
	cv::drawContours(original, contours,
		-1, // draw all contours  
		cv::Scalar(255, 255, 255), // in white  
		-1); // with a thickness of 2  

	cv::namedWindow("Contours on Animals");
	cv::imshow("Contours on Animals", original);

	// Let's now draw black contours on white image  
	result.setTo(cv::Scalar(255));
	cv::drawContours(result, contours,
		-1, // draw all contours  
		cv::Scalar(0), // in black  
		-1); // with a thickness of 1  
			 //image= cv::imread("test.png",0);  




	cv::waitKey(0);

	return 0;
}