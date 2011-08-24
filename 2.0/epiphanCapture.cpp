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
#include "epiphanCapture.h"

#include <string.h>
#include "frmgrab.h"
#include "v2u_lib.h"
#include "v2u_version.h"

#define _debug_

extern "C"
{
  FrmGrabber* FrmGrabNet_Open(void);


};

using namespace cv;

void EpiphanCapture::run()
{
  Ptr<paolMat> img;
  img = new paolMat;
  img->name = "EpiphanFrame";
  img->count = 0;
  img->time = 0;
  
  //Init Epiphan
  VideoCapture epiphancap(1);
  if(epiphancap.isOpened())
    {

      //Grab First Frame
      while(running())
	{
	  //Grab Frames as fast as we can
	  epiphancap >> img->src;
	  push(img);
	  img->count++;
	};
    }
  else
    std::cout<<"Error opening Epiphan Capture"<<std::endl;
  
  stop();
};

void EpiphanCapture::runEthCap()
{

  
  //FrmGrabNet_Open();
  std::cout<<"I got here"<<std::endl;
  //stop();
};
