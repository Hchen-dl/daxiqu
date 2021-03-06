// daxiqu.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <iostream>


using cv:: imread;
using cv::imshow;
using cv:: Mat;

using std::cout;
using std::endl;
cv::Point getNextMinLoc(Mat result, int templatWidth, int templatHeight, double maxValIn, cv::Point lastLoc);
int main()
{
	Mat srcImg = imread("C:\\Users\\ahaiya\\Documents\\secondyear\\daohang\\XIQU2.tif", 1);
	Mat tempImg = imread("C:\\Users\\ahaiya\\Documents\\secondyear\\daohang\\phone02.jpg", 1);
	for (int index = 0; index < 7; index++)
	{
		Mat roiImg = srcImg(cv::Rect(0, index* srcImg.rows / 7, srcImg.cols, srcImg.rows / 7));
		
		resize(tempImg, tempImg, cv::Size(260, 206));
		cv::namedWindow("srcImg", CV_WINDOW_KEEPRATIO);
		int width = roiImg.cols - tempImg.cols + 1;
		int height = roiImg.rows - tempImg.rows + 1;
		Mat resultImg(cv::Size(width, height), CV_32FC1);
		//2.模版匹配
		matchTemplate(roiImg, tempImg, resultImg, CV_TM_CCOEFF_NORMED);
		cv::namedWindow("result", CV_WINDOW_KEEPRATIO);
		imshow("result", resultImg);
		//3.正则化(归一化到0-1)
		//normalize(resultImg, resultImg, 0, 1, cv::NORM_MINMAX, -1);
		//4.找出resultImg中的最大值及其位置
		double minValue = 0;
		double maxValue = 0;
		cv::Point minPosition;
		cv::Point maxPosition;
		minMaxLoc(resultImg, &minValue, &maxValue, &minPosition, &maxPosition);//查找数组和子数组的全局最小值和最大值
			//    void minMaxLoc(InputArray src, CV_OUT double* minVal,
			//                               CV_OUT double* maxVal = 0, CV_OUT Point* minLoc = 0,
			//                               CV_OUT Point* maxLoc = 0, InputArray mask = noArray());
			//    src
			//    输入图像数组， 单通道或者设置了 COI 的多通道。
			//    minval
			//    指向返回的最小值的指针。
			//    maxval
			//    指向返回的最大值的指针。
			//    minloc
			//    指向返回的最小值的位置指针。
			//    maxloc
			//    指向返回的最大值的位置指针。
			//    mask
			//    选择一个子数组的操作掩模。
			//    函数 MinMaxLoc 查找元素中的最小值和最大值以及他们的位置。函数在整个数组、
			//    或选定的ROI区域(对IplImage)或当MASK不为NULL时指定的数组区域中，搜索极值 。
			//    如果数组不止一个通道，它就必须是设置了 COI 的 IplImage 类型。 如果是多维数组
			//    minloc->x 和 maxloc->x 将包含极值的原始位置信息 (线性的)。
		cout << "minValue: " << minValue << endl;
		cout << "maxValue: " << maxValue << endl;
		cout << "minPosition: " << minPosition << endl;
		cout << "maxPosition: " << maxPosition << endl;
		//5.根据resultImg中的最大值位置在源图上画出矩形
		rectangle(srcImg, cv::Point(maxPosition.x , maxPosition.y + index * srcImg.rows / 7), cv::Point(maxPosition.x + tempImg.cols, maxPosition.y + tempImg.rows+index* srcImg.rows / 7), cv::Scalar(0, 255, 0), 4, 8);
	}
	
	//circle(srcImg, cv::Point(maxPosition.x + tempImg.cols / 2, maxPosition.y + tempImg.rows / 2), 300, cv::Scalar(0, 0, 255), 2, 8);
	/*
	maxPosition = getNextMinLoc(resultImg, tempImg.rows, tempImg.cols, maxValue, maxPosition);
	rectangle(srcImg, maxPosition, cv::Point(maxPosition.x + tempImg.cols, maxPosition.y + tempImg.rows), cv::Scalar(0, 255, 0), 4, 8);
	/*
	maxPosition = getNextMinLoc(resultImg, tempImg.rows, tempImg.cols, maxValue, maxPosition);
	rectangle(srcImg, maxPosition, cv::Point(maxPosition.x + tempImg.cols, maxPosition.y + tempImg.rows), cv::Scalar(0, 255, 0), 4, 8);
	/*
	maxPosition = getNextMinLoc(resultImg, tempImg.rows, tempImg.cols, maxValue, maxPosition);
	rectangle(srcImg, maxPosition, cv::Point(maxPosition.x + tempImg.cols, maxPosition.y + tempImg.rows), cv::Scalar(0, 255, 0), 4, 8);
	maxPosition = getNextMinLoc(resultImg, tempImg.rows, tempImg.cols, maxValue, maxPosition);
	rectangle(srcImg, maxPosition, cv::Point(maxPosition.x + tempImg.cols, maxPosition.y + tempImg.rows), cv::Scalar(0, 255, 0), 4, 8);
	maxPosition = getNextMinLoc(resultImg, tempImg.rows, tempImg.cols, maxValue, maxPosition);
	rectangle(srcImg, maxPosition, cv::Point(maxPosition.x + tempImg.cols, maxPosition.y + tempImg.rows), cv::Scalar(0, 255, 0), 4, 8);
	maxPosition = getNextMinLoc(resultImg, tempImg.rows, tempImg.cols, maxValue, maxPosition);
	rectangle(srcImg, maxPosition, cv::Point(maxPosition.x + tempImg.cols, maxPosition.y + tempImg.rows), cv::Scalar(0, 255, 0), 4, 8);*/
	imshow("srcImg", srcImg);
	imshow("template", tempImg);

	//-------------------- -
	//	作者：abcvincent
	//	来源：CSDN
	//	原文：https ://blog.csdn.net/abcvincent/article/details/79265549 
	//版权声明：本文为博主原创文章，转载请附上博文链接！
	//imshow("src", srcImg);
	cv::waitKey(0);
    return 0;
}

cv::Point getNextMinLoc(Mat result, int templatWidth, int templatHeight, double maxValIn, cv::Point lastLoc) {

	int y, x;
	int startY, startX, endY, endX;

	//计算大矩形的坐标
	startY = lastLoc.y - templatHeight;
	startX = lastLoc.x - templatWidth;

	//计算大矩形的的坐标 
	endY = lastLoc.y + templatHeight;
	endX = lastLoc.x + templatWidth;

	//不允许矩形越界
	startY = startY < 0 ? 0 : startY;
	startX = startX < 0 ? 0 : startX;
	endY = endY > result.rows - 1 ? result.rows - 1 : endY;
	endX = endX > result.cols - 1 ? result.cols - 1 : endX;

	//将大矩形内部 赋值为最大值 使得 以后找的最小值 不会位于该区域  避免找到重叠的目标       
	for (y = startY; y<endY; y++) {
		for (x = startX; x<endX; x++) {
			result.at<float>(y, x) = maxValIn;
			//SetReal2D(result, y, x, maxValIn);
		}
	}
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;

	//查找result中的最小值 及其所在坐标        
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	return minLoc;
}
//-------------------- -
//作者：大流士一世
//来源：CSDN
//原文：https ://blog.csdn.net/u014751607/article/details/60953994 
//版权声明：本文为博主原创文章，转载请附上博文链接！
Mat RotateImg(Mat src,double angle)
{
	cv::imshow("src", src);
	//double angle = -45;
	cv::Point2f center(src.cols / 2, src.rows / 2);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1);
	cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();

	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size());
	cv::imshow("dst", dst);
	return dst;
}