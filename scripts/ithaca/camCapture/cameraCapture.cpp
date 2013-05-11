#include "opencv/cv.h" 
#include "opencv/highgui.h" 
#include <stdio.h> 
#include <iostream>
#include <cstdlib>
#include <ctime>
 
using namespace cv;

int main(int argc, char* argv[]) {
  //argv 1: directory to save whiteboard
  //argv 2: directory to save computer images
  //argv 3: duration
  //argv 4: number of cameras
  int count=0;
  char filename[256];
  Mat frame;
  time_t startTime,cTime;
  int currentTime=0;
  int duration=atoi(argv[3]);
  int numCameras=atoi(argv[4]);

  //what the number in cap refers to:
  // 0: video0 on laptops this is internal camera
  // 1: video1 first usb video device plugged in
  //    should be vga2usb and into usb2.0 slot
  //       if this fails make sure that permissions one the vga2usb are correct
  //       sudo chmod 777 /dev/vga2usb*
  // 2: video2 second
  //    should be camera for video and usb3.0 slot
  // 3: video3 third
  //    should be camera for whiteboard and sub3.0 slot
  VideoCapture cap1(1);//set computer capture thread
  VideoCapture cap3(3);//set whiteboard capture thread
  time(&startTime);
  printf("%d\n",duration);
  while ( currentTime<duration ) {
    //computer capture
    cap1 >> frame;
    time(&cTime);
    currentTime=(int)(cTime-startTime);
    //sprintf arguments
    //argv[1] folder to white output to
    sprintf(filename,"%sscreenscreen%06d-%10d.png",argv[1],count,(int)cTime);
    imwrite(filename,frame);
    printf("%s\n",filename);
    
	// remove whiteboard capture, add vgatousb camera
    //whiteboard capture
    cap3 >> frame;
    time(&cTime);
    currentTime=(int)(cTime-startTime);
    //sprintf arguments
    //argv[1] folder to white output to
    //1 which whiteboard camera it is
    sprintf(filename,"%sframe%1d-%06d-%10d.png",argv[2],1,count,(int)cTime);
    imwrite(filename,frame);
    printf("%s\n",filename);
    
    count++;
  }
  return 0;
}
