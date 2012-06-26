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

int main(int argc, char** argv)
{
  //Hack to avoid unused var warning
  argc++;
  argc--;
  char outDir[1024];
  std::strcpy(outDir, argv[1]);
  char WBdir[1024];
  std::strcpy(WBdir, argv[2]);
  char WBfirst[1024];
  std::strcpy(WBfirst, argv[3]);
  char WBpattern[512];
  std::strcpy(WBpattern, argv[4]);
  
  ////////////////////////////////
  /// Camera/WB Cap/Proc Buffers /
  Buffer* wbBuffer;
  wbBuffer = new Buffer;
  Buffer* lectVideoBuffer;
  lectVideoBuffer = new Buffer;
  Buffer* wbSlidesBuffer;
  wbSlidesBuffer = new Buffer;
  Buffer* wbSlidesWriteBuffer;
  wbSlidesWriteBuffer = new Buffer;
  Buffer* wbReadBuffer;
  wbReadBuffer = new Buffer;

  //////////////////////////////////////
  // WB Cap/Proc Modules
  //  WriteMod presVideoFramesWriter(lectVideoBuffer);
  WriteMod wbSlidesWriter(wbSlidesWriteBuffer);
  WhiteBoardFoot wbSlides(wbSlidesBuffer,wbSlidesWriteBuffer);
  WhiteBoardProcess wbproc(wbBuffer, wbSlidesBuffer);
  // LectVideoFrameCreate lectFrameCreator(wbBuffer, lectVideoBuffer);
  LocateSpeaker locateSpeaker(wbReadBuffer, wbBuffer);
  ReadMod readFromDisk(wbReadBuffer);

  ///////////////////////////////////////
  // Launch WB mods in revers order /////
  
  boost::thread_group wbThreads;

  //wbThreads.create_thread(boost::bind(&WriteMod::WriteMatsByCount, &presVideoFramesWriter, outDir));
  wbThreads.create_thread(boost::bind(&WriteMod::WriteMats, &wbSlidesWriter, outDir));
  wbThreads.create_thread(boost::bind(&WhiteBoardFoot::run, &wbSlides));
  wbThreads.create_thread(boost::bind(&WhiteBoardProcess::run, &wbproc, 1));
  wbThreads.create_thread(boost::bind(&LocateSpeaker::run, &locateSpeaker));
  wbThreads.create_thread(boost::bind(&ReadMod::ReadFromPatternFlipExt, &readFromDisk, WBdir,WBfirst,WBpattern));
  
  wbThreads.join_all();
  
  std::cout<<"Main: about to begin deleting WB buffers"<<std::endl;
  delete wbSlidesBuffer;
  delete wbBuffer;
  delete lectVideoBuffer;
  //delete lectVideoBuffer;
  delete wbReadBuffer;
  std::cout<<"Main: finished deleting WB buffers"<<std::endl;

  return 0;
}

