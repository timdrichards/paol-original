// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
//#include <algorithm>
#include <cstdio>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"

//Our Libs

#include "buffer.h"
//#include "src/producer.h"
#include "consumer.h"


using namespace cv;
using namespace boost;
using namespace std;

void testProducer(Buffer *myBuffer){

  
  /*  
  std::cout<<"TestProducer:: Starting"<<std::endl;
  cv::Mat img = imread("test.jpg");
  myBuffer->push(img);
  myBuffer->push(img);
  myBuffer->push(img);
  std::cout<<"TestProducer:: Pushed image to myBuffer!"<<std::endl;
*/
};

void testConsumer(Buffer *myBuffer){
  std::cout<<"TestConusmer:: Starting"<<std::endl;
  cv::Mat img;
  int myID = myBuffer->registerConsumer();
  std::cout<<"TestConsumer:: My id is "<<myID<<std::endl;
  img = myBuffer->pop(myID);
  namedWindow("myWindow");
  imshow("myWindow", img);
  waitKey(0);
};

//////Testing Code//////

//void readImageToLinkedList(FrameLinkedList *list);
//void displayPopAndDisplay(FrameLinkedList *list);

//void readImageToLinkedList(FrameLinkedList *list);

int main()
{

  boost::posix_time::seconds globalSleepTime(2);
  std::cout<<"MAIN: Main launched"<<std::endl;

  Buffer* testBuffer;
  testBuffer = new Buffer;

  boost::thread consumer1(testConsumer, testBuffer);
  boost::thread producer1(testProducer, testBuffer);
  

  std::cout<<"MAIN:: Waiting for join"<<std::endl;

  consumer1.join();
  producer1.join();
  

  std::cout<<"MAIN: Main Closing"<<std::endl;
  return 0;

};



/*
void readImageToLinkedList(FrameLinkedList *list)
{
  std::cout<<"Producer:: about to read image"<<std::endl;
  cv::Mat newImage;
  newImage = imread("test.jpg");
  
  if (newImage.data)
    {
      list->push(newImage);
      std::cout<<"Producer:: Linked list has the image, list size is now: "<< list->size <<std::endl;
    }else
    {
      std::cout<<"Error Loading Image"<<std::endl;
    };
};


void displayPopAndDisplay(FrameLinkedList *list)
{
  for(int i=0; i<4; i++)
    {
      std::cout<<"Consumer:: About to attempt pop, alocating memory"<<std::endl;
      cv::Mat outImage;
      outImage = list->pop();
      std::cout<<"Consumer:: I have the image!"<<std::endl;
      namedWindow("Window");
      imshow("Window", outImage);
      waitKey(0);
      std::cout<<"Consumer:: Done showing Image"<<std::endl;
    }
};

FrameLinkedList testList;
  FrameLinkedList* listPointer;
  listPointer = new FrameLinkedList;
  std::cout<<"MAIN: Created test list"<<std::endl;
  
  //Launch viewer thread//
  boost::thread consumer1(displayPopAndDisplay, listPointer);
  boost::this_thread::sleep(globalSleepTime);

  //Load Images into the Buffer//
  boost::thread producer1(readImageToLinkedList, listPointer);
  boost::this_thread::sleep(globalSleepTime);
  boost::thread producer2(readImageToLinkedList, listPointer);
  boost::this_thread::sleep(globalSleepTime);
  boost::thread producer3(readImageToLinkedList, listPointer);
  boost::this_thread::sleep(globalSleepTime);
  boost::thread producer4(readImageToLinkedList, listPointer);
  boost::this_thread::sleep(globalSleepTime);
  
  producer1.join();
  producer2.join();
  producer3.join();
  producer4.join();
  consumer1.join();
  
*/
