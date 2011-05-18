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
#include "src/buffer.h"
#include "src/producer.h"
#include "src/consumer.h"

using namespace cv;
using namespace  boost;


int main()
{
  std::cout<<"MAIN: Main launched"<<std::endl;

  frameListItem testFrame;
  std::cout<<"MAIN: Made a frame"<<std::endl;

  FrameLinkedList testList;
  std::cout<<"MAIN: Made a test list"<<std::endl;

  Buffer testBuffer;
  std::cout<<"MAIN: Made a test buffer"<<std::endl;


  std::cout<<"MAIN: Main Ending"<<std::endl;

  return 0;

};
