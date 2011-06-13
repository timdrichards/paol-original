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
#include "consumer.h"

using namespace cv;
using namespace std;
//using namespace boost;

cv::Mat Consumer::pop()
{
  cv::Mat img = conBuffer->pop(myID);
  if (img.data)
    {
      return img;
    }else
    {
      stop();
      return img;
    };
};


void Consumer::stop()
{
  boost::mutex::scoped_lock lock(stoplock);
  killMe = true;
};

bool Consumer::keepRunning()
{
  boost::mutex::scoped_lock lock(stoplock);
  return !killMe;
};

void Consumer::start(Buffer *buffer)
{
  conBuffer = buffer;
  myID = conBuffer->registerConsumer();
};

void imWindow::run()
{
  //Hop into a loop as long as your ID is set and you haven't recieved a stop()
  while(myID != -1 && keepRunning())
    {
      namedWindow("Window");
      cv::Mat img;
      img = pop();
      if (img.data)
	{
	  imshow("Window", img);
	  waitKey(0);
	}else
	{
	  destroyWindow("Window");
	};
    };
  std::cout<<"ReadFromDisk:: I was told to stop, my ID was: "<<myID<<std::endl;
};
