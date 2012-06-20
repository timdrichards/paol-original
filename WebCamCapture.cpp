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
#include "module.h"
#include "genericProcess.h"
#include "WebCamCapture.h"

#define _debug_

using namespace cv;

void WebCamCapture::run()
{
  Ptr<paolMat> img;
  img = new paolMat;
  img->name = "WebCam";
  img->count = 0;
  img->time = 0;
  
  //Init Cam
  VideoCapture webCamCap(0);
  if(webCamCap.isOpened())
    {
      webCamCap >> img->src;
      //Grab First Frame
      while(running() && img->src.data)
	{
	  //Grab Frames as fast as we can
	  webCamCap >> img->src;
	  push(img);
	  img->count++;
	};
    }
  else
    std::cout<<"Error opening webCam Capture"<<std::endl;
  
  stop();
};

