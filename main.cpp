// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
#include <cstdio>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
//Our Libs

#include "paolMat.h"
#include "buffer.h"
#include "module.h"
#include "whiteBoard.h"
#include "whiteBoardFoot.h"
#include "genericProcess.h"
#include "locateSpeaker.h"
#include "ac.h"
#include "lectVideoFrameCreate.h"
#include "computerDistribute.h"
#include "computerProcess.h"
#include "WebCamCapture.h"
#include "usbCam.h"
#include "sample-capture.h"


using namespace cv;

void computerPipeline()
{


}

int main(int argc, char** argv)
{
  
  char outDir[1024];
  std::strcpy(outDir, argv[1]);
  
  char Cdir[1024];
  std::strcpy(Cdir, argv[2]);
  
  char Cfirst[1024];
  std::strcpy(Cfirst, argv[3]);
  
  
  return 0;
}

