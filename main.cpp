#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
int Judge_Condition1(Mat &src, int i, int j);
int Judge_Condition2(Mat &src, int i, int j);
int Judge_Condition3(Mat &src, int i, int j);
int Judge_Condition4(Mat &mask, Mat &src, int i, int j);
int Judge_Condition5(Mat &mask, Mat &src, int i, int j);
struct Direct
{
	int x;
	int y;
};  //方向结构体
	//定义n0...n7八个方向
Direct N[8] = { { -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 } };
int main(void)
{
	//读取图片
	Mat src = imread("K:/handwrite.jpg");
	Mat image;
	//转为灰度图像
	cvtColor(src, image, CV_BGR2GRAY);
	int rows = image.rows;
	int cols = image.cols;
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if ((int)image.at<uchar>(i, j) > 200)
			{
				image.at<uchar>(i, j) = 255;
			}
			else
			{
				image.at<uchar>(i, j) = 0;
			}
		}
	}
	//复制图像
	Mat dst_temp = image;
	Mat mask=image.clone();
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			mask.at<uchar>(i, j) = 0;
		}
	}
	namedWindow("src", 1);
	imshow("src", image);
	bool flag = true;
	while(flag)
	{
		flag = false;
		int count = 0;
		for (int i = 1; i < rows - 1; i++)
		{
			for (int j = 1; j < cols - 1; j++)
			{
				if (image.at<uchar>(i, j) == 0)
					continue;
				if (Judge_Condition1(image, i, j) == 1)
					continue;
				if (Judge_Condition2(image, i, j) == 0)
					continue;
				if (Judge_Condition3(image, i, j) != 1)
					continue;
				if (Judge_Condition4(mask, image, i, j) == 1)
					continue;
				if (Judge_Condition5(mask, image, i, j) == 1)
					continue;
				mask.at<uchar>(i, j) = 1;
				count++;
			}
		}
		if (count)
		{
			for (int i = 0; i < mask.rows; i++)
			{
				for (int j = 0; j < mask.cols; j++)
				{
					if (mask.at<uchar>(i, j) == 1)
						image.at<uchar>(i, j) = 0;
				}
			}
			flag = true;
			/*************一次遍历后将标记复位*******************/
			for (int i = 0; i < image.rows; i++)
			{
				for (int j = 0; j < image.cols; j++)
				{
					mask.at<uchar>(i, j) = 0;
				}
			}
		}
	}
	
	
	namedWindow("dst", 1);
	imshow("dst", image);
	Mat temp = Mat::zeros(image.rows, image.cols, CV_8UC3);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			temp.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i,j)[0];
			temp.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
			temp.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
			if (image.at<uchar>(i, j) > 0)
			{
				temp.at<Vec3b>(i, j)[0] = 0;
				temp.at<Vec3b>(i, j)[1] = 0;
				temp.at<Vec3b>(i, j)[2] = 255;
			}

		}
	}
	namedWindow("test", 1);
	imshow("test", temp);
	waitKey(0);
	return 0;
}

int Judge_Condition1(Mat &image, int i, int j)//四邻域点不全为1
{
	if (((int)image.at<uchar>(i + N[0].x, j + N[0].y) == 0 )&&
		((int)image.at<uchar>(i + N[2].x, j + N[2].y) == 0 )&&
		((int)image.at<uchar>(i + N[4].x, j + N[4].y) == 0 )&&
		((int)image.at<uchar>(i + N[6].x, j + N[6].y) == 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Judge_Condition2(Mat &image, int i, int j)
{
	int sum = 0;
	sum = (int)image.at<uchar>(i + N[0].x, j + N[0].y) +
		(int)image.at<uchar>(i + N[1].x, j + N[1].y) +
		(int)image.at<uchar>(i + N[2].x, j + N[2].y) +
		(int)image.at<uchar>(i + N[3].x, j + N[3].y) +
		(int)image.at<uchar>(i + N[4].x, j + N[4].y) +
		(int)image.at<uchar>(i + N[5].x, j + N[5].y) +
		(int)image.at<uchar>(i + N[6].x, j + N[6].y) +
		(int)image.at<uchar>(i + N[7].x, j + N[7].y);
	if (sum >= 510)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Judge_Condition3(Mat &image, int i, int j)
{
	int neighbor[8] = { 0 };
	for (int k = 0; k < 8; k++)
	{
		neighbor[k] = (int)image.at<uchar>(i + N[k].x, j + N[k].y) == 0 ? 1 : 0;
	}
	int count = neighbor[0] - (neighbor[0] & neighbor[1] & neighbor[2]);
	count += neighbor[2] - (neighbor[2] & neighbor[3] & neighbor[4]);
	count += neighbor[4] - (neighbor[4] & neighbor[5] & neighbor[6]);
	count += neighbor[6] - (neighbor[6] & neighbor[7] & neighbor[0]);
	return count;
 
}

int Judge_Condition4(Mat &mask, Mat &image, int i, int j)
{
	if (mask.at<uchar>(i - 1, j) == 1)
	{
		image.at<uchar>(i - 1, j) = 0;
		if (Judge_Condition3(image, i, j) != 1)
		{
			image.at<uchar>(i - 1, j) = 255;
			return 1;
		}
	}
}

int Judge_Condition5(Mat &mask, Mat &image, int i, int j)
{
	if (mask.at<uchar>(i, j - 1) == 1)
	{
		image.at<uchar>(i, j - 1) = 0;
		if (Judge_Condition3(image, i, j) != 1)
		{
			image.at<uchar>(i, j - 1) = 255;
			return 1;
		}
	}
}