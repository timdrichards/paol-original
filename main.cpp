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
#include "computerMain.h"
#include "wbMain.h"

using namespace cv;

void computerPipeline(char* outDir, char* dir, char* first);
void wbPipeline(char* outDif, char* dir, char* first);


int main(int argc, char** argv)
{
  
  boost::thread_group pipelines;
  //pipelines.create_thread(boost::bind(computerPipeline, argv[1],argv[2],argv[3]) );
  pipelines.create_thread(boost::bind(wbPipeline, argv[1], argv[2], argv[3]));
  pipelines.join_all();
  
  return 0;
}

