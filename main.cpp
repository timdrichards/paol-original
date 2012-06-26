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
  
  //boost::thread presVideoFramesWriterThread(boost::bind(&WriteMod::WriteMatsByCount, &presVideoFramesWriter, outDir));
  boost::thread wbSlidesWriterThread(boost::bind(&WriteMod::WriteMats, &wbSlidesWriter, outDir));
  boost::thread wbSlidesThread(boost::bind(&WhiteBoardFoot::run, &wbSlides));
  boost::thread wbprocThread(boost::bind(&WhiteBoardProcess::run, &wbproc, 1));
  // boost::thread lectFrameCreatorThread(&LectVideoFrameCreate::run, &lectFrameCreator);
  boost::thread locateSpeakerThread(&LocateSpeaker::run, &locateSpeaker);
  boost::thread wbReadThread(&ReadMod::ReadFromPatternFlipExt, &readFromDisk, WBdir,WBfirst,WBpattern);


  wbReadThread.join();
  locateSpeakerThread.join();
  //lectFrameCreatorThread.join();
  wbprocThread.join();
  wbSlidesThread.join();
  wbSlidesWriterThread.join();
  // presVideoFramesWriterThread.join();

  std::cout<<"Main: about to begin deleting WB buffers"<<std::endl;
  delete wbSlidesBuffer;
  delete wbBuffer;
  delete lectVideoBuffer;
  //delete lectVideoBuffer;
  delete wbReadBuffer;
  std::cout<<"Main: finished deleting WB buffers"<<std::endl;

  return 0;
}

