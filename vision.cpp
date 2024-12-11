#include<math.h>
#include"vision.hpp"
using namespace cv;

void setFrame(Mat& frame){
	frame = frame(Rect(Point(0,frame.rows/4*3),Point(frame.cols,frame.rows)));  
	cvtColor(frame,frame,COLOR_RGB2GRAY);
	frame +=  Scalar(100) - mean(frame);
	GaussianBlur(frame, frame, Size(3, 3), 0);
	threshold(frame, frame, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//morphologyEx(frame, frame, MORPH_OPEN, Mat(), Point(-1, -1), 1);
}

void drawBoundingBox(Mat& frame,Mat stats,Mat centroids, int lable, int index1,int index2, Point po1, Point po2){
	cvtColor(frame, frame, COLOR_GRAY2BGR);
	Scalar sc;
	 for(int i = 1;i<lable;i++){
		if(i == index1||i == index2) sc = Scalar(255,0,255);
        else sc =stats.at<int>(i,4)<100 ? Scalar(0,255,255):Scalar(255,0,0);
        rectangle(frame,Rect(stats.at<int>(i,0),stats.at<int>(i,1),stats.at<int>(i,2),stats.at<int>(i,3)),sc,2);
        rectangle(frame,Rect(centroids.at<double>(i,0),centroids.at<double>(i,1),3,3),sc,2);
        } 
	if(index1 == 0){
		 rectangle(frame,Rect(po1.x,po1.y,3,3),Scalar(255,0,255),2);
	}
	else if(index2 == 0){
		 rectangle(frame,Rect(po2.x,po2.y,3,3),Scalar(255,0,255),2);
	}
}

int findMinIndex(Mat stats,Mat centroids, int lable, Point& po,int MINDISTANCE){
	int index = 0;
	double mindistance = MINDISTANCE;
	for(int i = 1;i<lable;i++){
        if(stats.at<int>(i,4)<150) continue;
		double distance = sqrt(pow((po.x-centroids.at<double>(i,0)),2)+pow((po.y- centroids.at<double>(i,1)),2));
		if(distance<mindistance){
            mindistance = distance;
            index = i;
        }
     }
	 Point cen= Point(centroids.at<double>(index,0),centroids.at<double>(index,1));
	if(mindistance <  MINDISTANCE && po != cen && cen.y > 20 ) 
		po = cen;
	return index;
}

double getErr(Mat frame, Point po,double gain){
	return (frame.cols/2 -po.x)*gain;
}

Point getCenterPoint(Point po1, Point po2){
	return Point((po1.x+po2.x)/2 , (po1.y+po2.y)/2);
}

void drawCenter(Mat& frame, Point cen){
	  rectangle(frame,Rect(cen.x,cen.y,3,3),Scalar(0,0,255),2);
}