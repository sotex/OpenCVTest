#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>	// cvtColor

// 从零开始的频域水印完全解析 https://zhuanlan.zhihu.com/p/27632585
// OpenCV离散傅里叶变换（DFT前篇） https://blog.csdn.net/qq_37059483/article/details/77979910
// 阿里根据截图查到泄露者,用的什么黑科技？(数字盲水印) https://stgod.com/1482

/*
 +----+      +----------+    +--------+
 |图像|----->|傅里叶变换|--->|图像频域|------+
 +----+      +----------+    +--------+    +-|--+     +------------+   +--------+
                                           |叠加|---->|傅里叶逆变换|-->|水印图像|
 +------+          + - - - +               +-|--+     +------------+   +--------+
 |水印图|--------->| 编码  |-----------------+
 +------+          + - - - +
*/

void WaterMarkEncode(cv::Mat& wm)
{

}

int FrequencyDomainWatermark()
{
    // 读取图像(转为灰度图)
    cv::Mat	srcGray = cv::imread("./Image/sisy.jpg", cv::IMREAD_GRAYSCALE);
    // 获取目标图像合适大小
    int m = cv::getOptimalDFTSize(srcGray.cols);
    int n = cv::getOptimalDFTSize(srcGray.rows);
    cv::Mat padded;
    //把灰度图像放在左上角,在右边和下边扩展图像,扩展部分填充为0;
    cv::copyMakeBorder(srcGray, padded, 0, m/* - srcGray.rows*/, 0, n/* - srcGray.cols*/, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    // 生成一个与扩展图像一样大的全部为0的图像
    // 将扩展图像放在一起，用于后面合并。dft变换的实部和虚部
    cv::Mat planes[2];
    planes[0] = cv::Mat_<float>(padded);
    planes[1] = cv::Mat::zeros(padded.size(), CV_32F);
    //将几个单通道的mat融合成一个多通道的mat,这里融合的complexImg既有实部又有虚部
    cv::Mat complexImg;
    cv::merge(planes, 2, complexImg);
    //对上边合成的mat进行傅里叶变换,支持原地操作,傅里叶变换结果为复数.通道1存的是实部,通道二存的是虚部
    cv::dft(complexImg, complexImg);
    //把变换后的结果分割到两个mat,一个实部,一个虚部,方便后续操作
    cv::split(complexImg, planes);
    //这一部分是为了计算dft变换后的幅值，傅立叶变换的幅度值范围大到不适合在屏幕上显示。
    // 高值在屏幕上显示为白点，而低值为黑点，高低值的变化无法有效分辨。
    // 为了在屏幕上凸显出高低变化的连续性，我们可以用对数尺度来替换线性尺度,以便于显示幅值,计算公式如下:
    //=> log(1 + sqrt(Re(DFT(I))^2 +Im(DFT(I))^2))
    cv::magnitude(planes[0], planes[1], planes[0]);
    cv::Mat mag = planes[0];
    mag += cv::Scalar::all(1);
    cv::log(mag, mag);

    //crop the spectrum, if it has an odd number of rows or columns
    //修剪频谱,如果图像的行或者列是奇数的话,那其频谱是不对称的,因此要修剪
    //这里为什么要用  &-2这个操作，我会在代码后面的 注2 说明
    mag = mag(cv::Rect(0, 0, mag.cols & -2, mag.rows & -2));
    cv::Mat _magI = mag.clone();
    //这一步的目的仍然是为了显示,但是幅度值仍然超过可显示范围[0,1],我们使用 normalize() 函数将幅度归一化到可显示范围。
    cv::normalize(_magI, _magI, 0, 1, CV_MINMAX);
    cv::imshow("重新排列前图像", _magI);

    //rearrange the quadrants of Fourier image
    //so that the origin is at the image center
    //重新分配象限，使（0,0）移动到图像中心，  
    //在《数字图像处理》中，傅里叶变换之前要对源图像乘以（-1）^(x+y)进行中心化。  
    //这是是对傅里叶变换结果进行中心化
    int cx = mag.cols / 2;
    int cy = mag.rows / 2;

    cv::Mat tmp;
    cv::Mat q0(mag, cv::Rect(0, 0, cx, cy));   //Top-Left - Create a ROI per quadrant
    cv::Mat q1(mag, cv::Rect(cx, 0, cx, cy));  //Top-Right
    cv::Mat q2(mag, cv::Rect(0, cy, cx, cy));  //Bottom-Left
    cv::Mat q3(mag, cv::Rect(cx, cy, cx, cy)); //Bottom-Right

    //swap quadrants(Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // swap quadrant (Top-Rightwith Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);


    normalize(mag, mag, 0, 1, CV_MINMAX);
    imshow("Input Image", srcGray);
    cv::imshow("傅里叶变换", mag);

    //傅里叶的逆变换
    cv::Mat ifft;
    idft(complexImg, ifft, cv::DFT_REAL_OUTPUT);
    normalize(ifft, ifft, 0, 1, CV_MINMAX);
    imshow("逆傅里叶变换", ifft);

    return cv::waitKey();
}