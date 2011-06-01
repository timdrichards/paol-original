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
//#include "src/consumer.h"


using namespace cv;
using namespace boost;
using namespace std;

//////Testing Code//////
//void readImageToLinkedList(FrameLinkedList *list);

int main()
{
  std::cout<<"MAIN: Main launched"<<std::endl;

  FrameLinkedList testList;
  //testList = new FrameLinkedList;
  std::cout<<"MAIN: Created test list"<<std::endl;

  //readImageToLinkedList(testList);
  cv::Mat img;
  img = imread("test.jpg");
  testList.push(img);
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
      std::cout<<"Producer:: Linked list has the image"<<std::endl;
    }else
    {
      std::cout<<"Error Loading Image"<<std::endl;
    };
};

*/
