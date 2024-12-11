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

Point getCenterPoint(Point po1, Point po2)

![image](https://github.com/user-attachments/assets/8f8263db-2a99-4be9-b188-a0fa3ae79a71)

findMinIndex함수로 구한 포인트 객체 2를 더한값/2 의 값을 가지는 포인트 객체를 리턴한다.(최종적으로 따라가는 포인트 객체임)

double getErr(Mat frame, Point po,double gain)

![image](https://github.com/user-attachments/assets/846b8e8a-4852-445d-98a0-3c31519d28ac)

 getCenterPoint를 활용하여 구한 포인트를 사용하여 에러값을 구한다

 void drawCenter(Mat& frame, Point cen)

 ![image](https://github.com/user-attachments/assets/d53fe02b-5a9a-4db1-a834-066041062866)

 따라가고 있는 포인트 객체의 좌표를 빨간색으로 표시해준다.

 main.c

![image](https://github.com/user-attachments/assets/0f612387-bcaa-4f1a-a652-7b91ed9ad56a)


 필요한 헤더파일 인클루드, 네임 스페이스 설정 및 필요한 상수 디파인, control +c 플래그 설정해준다.

![image](https://github.com/user-attachments/assets/52727097-98f3-49a9-89c7-72c08131ac58)

반복문 전에 필요한 Dxl , VideoWriter , TickMeter, VideoCapture  객체 생성 및 각 vel값을 선언해준다.

전체 while문 

![image](https://github.com/user-attachments/assets/b065c820-534a-4e21-ae86-e800ca499709)

파트별로 설명

![image](https://github.com/user-attachments/assets/25e2ebe8-f4a5-4c59-a8fa-59a42e89b709)

시간 측정을 위한 TickMeter객체 리셋 및 스타트, kbhit을 통해 키보드 눌림을 감지한다(엔터를 누르징 않아도 입력 받아짐)
's'누를시 mode가 true로 바뀌며 모터가 동작하고 'q'를 누르면 코드가 멈춘다


![image](https://github.com/user-attachments/assets/f65182eb-08e4-4365-aadf-c912961969f7)

입력받은 영상을 VideoWriter 객체를 통해 pc로 전송한 후(전처리 되지않은 오리지날 영상) setFrame함수를 통하여 전처리 한다.

![image](https://github.com/user-attachments/assets/182fee1d-4ce7-4dbc-8d7a-03798ecc25ce)

connectedComponentsWithStats 함수를 사용하여 인식되 객체의 정보를 받아온다.(무게중심 면적 가로세로 길이 등)
각 포인트 객체의 초기값을 설정하는데 각 포인트는 영상의 x축 1/4,3/4 y는 1/2 cen은 영상에 중심에 있다고 가정한다

![image](https://github.com/user-attachments/assets/d613f763-fdc8-491e-a406-63caf39e956f)

findMinIndex함수를 통하여 인식한 각 객체의 인식된 객체의 인덱스 값을 구한고 포인트 객체의 위치를 업데이트 한다 (라인이 2개이니 두 번 호출한다)
getCenterPoint 함수를 사용하여 업데이트 된 객체의 좌표를 조합하여 따라가고 있는 포인트 객체의 값을 설정한다.
drawBoundingBox와 drawCenter를 통하여 바운딩 박스와 무게 중심을 표시해준다

![image](https://github.com/user-attachments/assets/d525de27-0e71-445a-91d9-fbe65854fcd9)

VideoWriter 객체를 통해 전처리한 frame을 pc로 전송 후 각 vel값을 부여한다 (rvel같은 경우 모터가 반대로 달려있음으로 -1을 곱해줌) 's'를 눌러 mode가 true인 경우 모터를 동작시킨다.
po1과 po2의 값이 같은 경우 (같은 차선을 서로 인식한 경우) 오류로 판단하여 코드를 멈춘다

![image](https://github.com/user-attachments/assets/95ff668d-cf2d-46ca-8e5a-243bee933117)

컨트롤 + c를 누른 경우 코드를 멈춤 , TickMeter의 시간 측정을 멈추고 에러값 lvel,rvel, 측정한 시간을 출력한다



![image](https://github.com/user-attachments/assets/578e905c-a085-466e-a37f-eedbd510c8fc)

while문이 끝난 경우 dxl,close()해준 후 종료한다.(하지 않으면 모터는 계속회전함)

메이크 파일

![image](https://github.com/user-attachments/assets/a7e5534d-99e8-46d2-be14-b579da9001e6)


