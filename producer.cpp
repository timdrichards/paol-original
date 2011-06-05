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

#include "buffer.h"
#include "producer.h"

using namespace cv;
using namespace boost;


void Producer::start(Buffer *buffer)
{
  myBuffer = buffer;
};

void Producer::push(cv::Mat img)
{
  myBuffer->push(img);
};

void Producer::stop()
{
  boost::mutex::scoped_lock lock(stoplock);
  killMe = true;
};

bool Producer::keepRunning()
{
  boost::mutex::scoped_lock lock(stoplock);
  return (!killMe);
};

void readFromDisk::run()
{
  cv::Mat img;
  boost::posix_time::seconds restTime(3);
  while (keepRunning())
    {
      img = imread("test.jpg");
      push(img);
      boost::this_thread::sleep(restTime);

    };
};
