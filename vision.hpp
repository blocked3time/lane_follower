#ifndef VISION_HPP
#define VISION_HPP

#include "opencv2/opencv.hpp"


void setFrame(cv::Mat& frame);

void drawBoundingBox(cv::Mat& frame,cv::Mat stats,cv::Mat centroids, int lable, int index1,int index,cv::Point po1,cv::Point po2);

int findMinIndex(cv::Mat stats,cv::Mat centroids, int lable, cv::Point& po,int MINDISTANCE);

double getErr(cv::Mat frame, cv::Point po,double gain);

void drawCenter(cv::Mat& frame, cv::Point cen);

cv::Point getCenterPoint(cv::Point po1, cv::Point po2); 
#endif 