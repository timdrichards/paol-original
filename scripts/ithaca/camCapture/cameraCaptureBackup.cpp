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
  // 2: video2 second
  // 3: video3 third
  //currently everything is set up for whiteboard to be captured from the first
  // usb device and computer to be captured from the third usb device (vga2usb)
  // if this fails make sure that permissions one the vga2usb are correct
  // sudo chmod 777 /dev/vga2usb*
  VideoCapture cap(2);
  //  if (numCameras>1){
  //VideoCapture cap2(2);
  //if (numCameras>2){
  //VideoCapture cap3(3);

      //}
      //}
  time(&startTime);
  printf("%d\n",duration);
  while ( currentTime<duration ) {
    cap >> frame;
    time(&cTime);
    currentTime=(int)(cTime-startTime);
    sprintf(filename,"%sframe%1d-%06d-%10d.png",argv[1],1,count,(int)cTime);
    imwrite(filename,frame);

    /* if (numCameras>1){
          cap2 >> frame;
	  time(&cTime);
	  currentTime=(int)(cTime-startTime);
	  sprintf(filename,"%sframe%1d-%06d-%10d.png",argv[1],2,count,(int)cTime);
	  imwrite(filename,frame);*/
    /*if (numCameras>2){*/
    //cap3 >> frame;
    time(&cTime);
    currentTime=(int)(cTime-startTime);
    //this name still needs to be corrected for computer and the path location
    //for writting the images fixed
    //sprintf(filename,"%sframe%1d-%06d-%10d.png",argv[1],3,count,(int)cTime);
    

    //for writing the computer images
    sprintf(filename,"%sscreenscreen%06d-%10d.png",argv[2],3,count,(int)cTime);
    imwrite(filename,frame);
    //	    }*/
    //    }
    
    printf("%s\n",filename);
    count++;
  }
  return 0;
}
