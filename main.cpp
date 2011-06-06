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
#include "process.h"


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

void testReadFromPattern(Buffer *myBuffer,char* dir, char* firstImage) 
{
  ReadFromDisk producer;
  producer.start(myBuffer);
  producer.readFromPattern(dir, firstImage);

};

void testProducer(Buffer *myBuffer)
{
  ReadFromDisk producer;
  producer.start(myBuffer);
  producer.run();
};


void testProcess(Buffer *inBuffer, Buffer *outBuffer)
{
  

int main(int argc, char** argv)
{

  const boost::posix_time::seconds globalSleepTime(2);
  std::cout<<"MAIN: Main launched"<<std::endl;

  Buffer* proBuffer;
  proBuffer = new Buffer;

  Buffer* conBuffer;
  conBuffer = new Buffer;

  //You need to start your consumers first otherwise the buffer will throw away pushed frames//
 
  boost::thread diskWrite(testWriteToDisk, conBuffer);
  boost::thread process(testProcess, proBuffer, conBuffer);
  boost::thread diskRead(testReadFromPattern, proBuffer, argv[1], argv[2]);
  
  

  std::cout<<"MAIN:: Waiting for join"<<std::endl;
  //Boost .join waits for the thread to complete//
  
  diskWrite.join();
  diskRead.join();
  

  std::cout<<"MAIN: Main Closing"<<std::endl;
  return 0;

};
