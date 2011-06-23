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

Consumer::Consumer(Buffer *buffer)
{
  conBuffer = buffer;
  myID = conBuffer->registerConsumer();
  killMe = false;
  
};

paolMat Consumer::pop()
{
  //std::cout<<"Consumer Pop1"<<std::endl;
  paolMat img;
  //std::cout<<"Consumer Pop2"<<std::endl;
  img.copy(conBuffer->pop(myID));
  //std::cout<<"Consumer Pop3"<<std::endl;
  if (img.src.data)
    {
      //std::cout<<"Consumer Pop4"<<std::endl;
      return img;
    }else
    {
      stop();
      //std::cout<<"Consumer Pop5"<<std::endl;
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

/*

void imWindow::run()
{
  //Hop into a loop as long as your ID is set and you haven't recieved a stop()
  while(myID != -1 && keepRunning())
    {
      namedWindow("Window");
      paolMat img;
      img = pop();
      if (img.src.data)
	{
	  imshow("Window", img.src);
	  waitKey(0);
	}else
	{
	  destroyWindow("Window");
	};
    };
  //std::cout<<"ReadFromDisk:: I was told to stop, my ID was: "<<myID<<std::endl;
};

*/
