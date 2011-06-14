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

#define _debug_


Processor::Processor(Buffer* in, Buffer* out)
{
  boost::mutex::scoped_lock lock(proLock);
  killMe = false;
  input = in;
  output = out;
  myID = input->registerConsumer();
};


paolMat Processor::pop(){
  return input->pop(myID);
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

void Processor::run()
{
  paolMat inputImg;
  paolMat backgroundImg;
  inputImg.copy(pop());
  
  backgroundImg.copy(inputImg);
  cv::Point centerPoint(-1,-1);
  while(inputImg.src.data)
    {
#ifndef _debug_
      std::cout<<"Processor:: Pushing and poppin!"<<std::endl;
#endif
      cv::blur(inputImg.src, backgroundImg.src, cv::Size(25,25), centerPoint, 1);
      output->push(backgroundImg);
      inputImg = pop();
    };
#ifndef _debug_
  std::cout<<"Processor:: Loop Ended"<<std::endl;
#endif
  paolMat nullImage;
  output->push(nullImage);
  output->stop();
#ifndef _debug_
  std::cout<<"Processor:: Null image passed"<<std::endl;
#endif



};
