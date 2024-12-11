#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "vision.hpp"  
#include "opencv2/opencv.hpp"
#include "dxl.hpp"
#define MINDISTANCE 100
#define RPM 100
using namespace std;
using namespace cv;
double GAIN =0.25;
bool ctrl_c_pressed;
void ctrlc(int)
{
	ctrl_c_pressed = true;
}

int main(void)
{	

    string src = "nvarguscamerasrc sensor-id=0 ! \
video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
format=(string)NV12, framerate=(fraction)30/1 ! \
nvvidconv flip-method=0 ! video/x-raw, \
width=(int)640, height=(int)360, format=(string)BGRx ! \
videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	VideoCapture source(src, CAP_GSTREAMER);

	Dxl dxl;
	if (!dxl.open()) { cout << "dxl open error" << endl; return -1; }
    //VideoCapture source("5_lt_cw_100rpm_out.mp4");
	//VideoCapture source("lanefollow_100rpm_ccw.mp4");
	if (!source.isOpened()) { cout << "Camera error" << endl; return -1; }
	/*VideoWriter vw("output.mp4", VideoWriter::fourcc('X', '2', '6', '4'), 30, Size(640, 360), true);
	if (!vw.isOpened())
	{
		std::cout << "Can't write video !!! check setting" << std::endl;
		return -1;
	}
	*/
	string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
h264parse ! rtph264pay pt=96 ! \
udpsink host=203.234.58.157 port=8001 sync=false";
	VideoWriter writer1(dst1, 0, (double)30, Size(640, 360/4), true);
	if (!writer1.isOpened()) {
		cerr << "Writer open failed!" << endl; return -1;
	}

	string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
h264parse ! rtph264pay pt=96 ! \
udpsink host=203.234.58.157 port=8002 sync=false";
	VideoWriter writer2(dst2, 0, (double)30, Size(640, 360), true);
	if (!writer2.isOpened()) {
		cerr << "Writer2 open failed!" << endl; return -1;
	}
	signal(SIGINT, ctrlc); //시그널 핸들러 지정
	Mat stats, centroids, labels,frame, gray;
	TickMeter tm;
	bool mode = false;
	double lvel = RPM, rvel = RPM*-1;
	while (true)
	{
		tm.reset();
		tm.start(); 
		if(dxl.kbhit()) {// 없으면 제어 멈춤
			char ch = dxl.getch();
			if(ch == 'q') break;
			else if(ch == 's') mode = true; 
		}
		
		source >> frame;
		if (frame.empty()) { cerr << "frame empty!" << endl; break; }
		writer2<<frame;

		setFrame(frame);

		int lable =  connectedComponentsWithStats(frame, labels, stats, centroids);

		static Point po1 = Point(frame.cols/4,frame.rows/2), po2 = Point(frame.cols*3/4,frame.rows/2),cen =Point(frame.cols/2,frame.rows/2);
		 

		int index1 = findMinIndex(stats,centroids, lable,po1, MINDISTANCE),index2 = findMinIndex(stats,centroids, lable,po2, MINDISTANCE);
		cen = getCenterPoint(po1,po2);
		drawBoundingBox(frame,stats,centroids, lable, index1,index2,po1,po2);
		drawCenter(frame, cen);
		writer1 <<frame; 

		lvel = RPM - getErr(frame, cen, GAIN);
		rvel = RPM*-1 - getErr(frame, cen, GAIN);
		if(mode) dxl.setVelocity(lvel, rvel);

		if (ctrl_c_pressed) break;
		//usleep(20 * 1000);
		tm.stop();
		cout<<"error : "<<getErr(frame, cen, GAIN) <<" lvel : " << lvel <<" rvel : " << rvel <<" time : " <<tm.getTimeSec() << "Sec"<<endl;
	}
	dxl.close();
	return 0;
}