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
#include "genericProcess.h"
#include "whiteBoard.h"
#include "whiteBoardFoot.h"
#include "genericProcess.h"
#include "locateSpeaker.h"

void wbPipeline(char* outDir, char* dir, char* first, int start)
{
  char out[1024];
  std::strcpy(out, outDir);
  std::strcat(out, "whiteboard/");
  char WBdir[1024];
  std::strcpy(WBdir, dir);
  char WBfirst[1024];
  std::strcpy(WBfirst, first);
  

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
  WriteMod wbSlidesWriter(wbSlidesWriteBuffer);
  WhiteBoardFoot wbSlides(wbSlidesBuffer,wbSlidesWriteBuffer);
  WhiteBoardProcess wbproc(wbBuffer, wbSlidesBuffer);
  LocateSpeaker locateSpeaker(wbReadBuffer, wbBuffer);
  ReadMod readFromDisk(wbReadBuffer);
  
  boost::thread_group wbThreads;
  
  wbThreads.create_thread(boost::bind(&WriteMod::WriteSlides, &wbSlidesWriter, out, "whiteboard", start));
  wbThreads.create_thread(boost::bind(&WhiteBoardFoot::run, &wbSlides));
  wbThreads.create_thread(boost::bind(&WhiteBoardProcess::run, &wbproc, 1));
  wbThreads.create_thread(boost::bind(&LocateSpeaker::run, &locateSpeaker));
  wbThreads.create_thread(boost::bind(&ReadMod::ReadFromPatternFlipExtCrop, &readFromDisk, WBdir,WBfirst,400,1400,3600,1000));
  
  wbThreads.join_all();
  
  delete wbSlidesBuffer;
  delete wbBuffer;
  delete lectVideoBuffer;
  delete wbReadBuffer;
  
}
