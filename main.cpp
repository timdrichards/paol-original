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
#include "producer.h"
#include "consumer.h"


using namespace cv;
using namespace boost;
//using namespace std;

void testConsumer(Buffer *myBuffer){
  imWindow consumer;
  consumer.start(myBuffer);
  consumer.run();
};

void testProducer(Buffer *myBuffer){
  readFromDisk producer;
  producer.start(myBuffer);
  producer.run();
};


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
