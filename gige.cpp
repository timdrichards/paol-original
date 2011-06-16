// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/assert.hpp>

//Boost for Dir Navigation
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
//#include <algorithm>
#include <cstdio>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"

#include "paolMat.h"
#include "buffer.h"
#include "producer.h"
#include "gige.h"

using namespace cv;
using namespace boost;

CameraCap::CameraCap(Buffer *buffer, int incam)
{
  cam = incam;
  start(buffer);
};


void CameraCap::run()
{
  
  std::cout<<"CameraCap:: Run launched"<<std::endl;
  int count =0;
  //cvNamedWindow("Example", CV_WINDOW_AUTOSIZE);
  CvCapture* capture = cvCreateCameraCapture(800);
  BOOST_ASSERT(capture != NULL);

  paolMat matFrame;
  IplImage* frame;
  matFrame.name = "TestCam";
  matFrame.time = 0;

  std::cout<<"CameraCap:: About to launch loop"<<std::endl;

  while(keepRunning())
    {
      std::cout<<"CameraCap:: Loop Head"<<std::endl;
      frame = cvQueryFrame(capture);
      if(!frame)
	{
	  std::cout<<"CameraCap:: Frame Query Failed"<<std::endl;
	  break;
	};
      
      //cvShowImage("Example", frame);
      matFrame.src = cvarrToMat(frame);
      //imwrite("image.png", matFrame);
      //char c = cvWaitKey(50);
      //if (c == 27) break;
      matFrame.count = count;
      count++;
      push(matFrame);
      std::cout<<"CameraCap:: Loop Foot"<<std::endl;
    };
  std::cout<<"CameraCap:: Loop Broken Exiting"<<std::endl;
  cvReleaseCapture( &capture);
  stop();
  //cvDestroyWindow("Example");
  
};
