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

#include "paolMat.h"

#include "buffer.h"
#include "producer.h"
#include "consumer.h"
#include "process.h"

using namespace cv;
using namespace boost;

//#define _debug_


Processor::Processor(Buffer* in, Buffer* out) : Producer(in) , Consumer(out) 
{
  boost::mutex::scoped_lock lock(proLock);
  killMe = false;
  //input = in;
  //output = out;
  myID = proBuffer->registerConsumer();
};


paolMat* Processor::pop(){
  return proBuffer->pop(myID);
};

bool Processor::keepRunning()
{
  boost::mutex::scoped_lock lock(proLock);
  return (!killMe);
};

void Processor::stop()
{
  boost::mutex::scoped_lock lock(proLock);
  killMe=true;
};

/*
void Processor::run()
{
  paolMat inputImg;
  paolMat* ptr;
  //paolMat backgroundImg;
  ptr = pop();
  inputImg.copy(&ptr);
  
  //backgroundImg.copy(inputImg);
  //cv::Point centerPoint(-1,-1);
  while(inputImg.src.data)
    {
#ifndef _debug_
      //std::cout<<"Processor:: Pushing and poppin!"<<std::endl;
#endif
      //cv::blur(inputImg.src, backgroundImg.src, cv::Size(25,25), centerPoint, 1);
      conBuffer->push(inputImg);
      inputImg.src.release();
      ptr = pop();
      inputImg.copy(ptr);
    };
#ifndef _debug_
  //std::cout<<"Processor:: Loop Ended"<<std::endl;
#endif
  inputImg.src.release();
  paolMat nullImage;
  conBuffer->push(nullImage);
  conBuffer->stop();
  
#ifndef _debug_
  //std::cout<<"Processor:: Null image passed"<<std::endl;
#endif



};

*/

void Processor::passOn()
{
  paolMat* img;
  img = pop();
  while (img->src.data)
    {
      conBuffer->push(img);
      delete img;
      img = proBuffer->pop(myID);
    };
  delete img;
  paolMat* nullImg;
  nullImg = new paolMat;
  conBuffer->push(nullImg);
  conBuffer->stop();
};
