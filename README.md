jetson과 opencv를 활용한 lane follower 

라인 트레이서알고리즘을 활용하여 2개의 라인을 인식한후 라인의 무게중심을 더한 값/2의 포인터 값을 중심으로 인식하여 주행한다.

블록도 

![image](https://github.com/user-attachments/assets/2deae1ec-8f7d-4593-ad44-e9eda4676f73)

유튜브

![image](https://github.com/user-attachments/assets/ad871d85-9d9c-47cd-859e-c1103d0c11a7)


https://www.youtube.com/watch?v=5OBzGHZPXrM&ab_channel=%EC%A0%95%EC%9A%A9%EA%B7%9C

헤더파일

![image](https://github.com/user-attachments/assets/3a580858-2330-4ad7-8bcf-87d490e8ea9e)

void setFrame(cv::Mat& frame)

![image](https://github.com/user-attachments/assets/2a6cb299-8668-49d6-b9c2-bc29817df610)


매개변수로 받은 Mat 객체를 전처리한다. 참조로 받은 frame을 영상을 라인 검출하기 좋게 밑 1/4로 설정 후 흑백으로 변환, 평균 밝기 설정 후 이진화 한다.

drawBoundingBox(Mat& frame,Mat stats,Mat centroids, int lable, int index1,int index2, Point po1, Point po2)

![image](https://github.com/user-attachments/assets/54d41123-7ae3-4fa9-a5e8-289bb69c0209)

찾은 객체에 바운딩 박스를 그린다(보라색 차선1,2, 빨강 현재 따라가고 있는 포인트 객체 파랑색 차선으로 인식하지 않은 객체, 노랑색 노이즈)
객체의 무게중심,면적,가로,세로의 정보를 받아 바운딩 박스를 그린다 인식된 객체가 없는 경우 이전의 포인트 객체에 표시해준다(차선이 검출되지 않는 경우) 

findMinIndex(Mat stats,Mat centroids, int lable, Point& po,int MINDISTANCE)

![image](https://github.com/user-attachments/assets/43ccfde2-ba0a-4536-902a-e55bd3c7855f)

포인트 객체 기준으로 가장 가까운 객체의 인덱스를 리턴한다.(라인 함수가 트레이서와 같음) 가장 가까운 객체의 조건은 거리가 MINDISTANCE보다 작아야하며 라인을 찾지 못 하는 경우 0을 리턴한다 (0은 배경의 인덱스로 처리하지 않음으로 객체를 검출하지 못 하는 것을 의미함)

레인 팔로우의 경우 2개의 차선을 인식해야함으로 한 반복문 안에서 이 함수를 두 번 호출하게 된다.

double getErr(Mat frame, Point po,double gain)

![image](https://github.com/user-attachments/assets/8f8263db-2a99-4be9-b188-a0fa3ae79a71)

findMinIndex함수로 구한 포인트 객체 2를 더한값/2 의 값을 가지는 포인트 객체를 리턴한다.(최종적으로 따라가는 포인트 객체임)

double getErr(Mat frame, Point po,double gain)

![image](https://github.com/user-attachments/assets/846b8e8a-4852-445d-98a0-3c31519d28ac)

앞에서 구한 



