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
#include "writeToDisk.h"


using namespace cv;
using namespace boost;
//using namespace std;

void testConsumer(Buffer *myBuffer)
{
  imWindow consumer;
  consumer.start(myBuffer);
  consumer.run();
};

void testWriteToDisk(Buffer *myBuffer)
{
  WriteToDisk consumer;
  consumer.start(myBuffer);
  consumer.setup("testImg", "outMedia/");
  consumer.run();

};

void testProducer(Buffer *myBuffer)
{
  readFromDisk producer;
  producer.start(myBuffer);
  producer.run();
};


int main()
{

  const boost::posix_time::seconds globalSleepTime(2);
  std::cout<<"MAIN: Main launched"<<std::endl;

  Buffer* testBuffer;
  testBuffer = new Buffer;

  //You need to start your consumers first otherwise the buffer will throw away pushed frames//
  //boost::thread consumer1(testConsumer, testBuffer);
  boost::thread consumer2(testWriteToDisk, testBuffer);
  boost::thread producer1(testProducer, testBuffer);
  

  std::cout<<"MAIN:: Waiting for join"<<std::endl;
  //Boost .join waits for the thread to complete//
  //consumer1.join();
  consumer2.join();
  producer1.join();
  

  std::cout<<"MAIN: Main Closing"<<std::endl;
  return 0;

};
