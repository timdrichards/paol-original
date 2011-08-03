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
#include "opencv/cvaux.h"
//Our Libs

#include "paolMat.h"
#include "buffer.h"
#include "module.h"
#include "whiteBoard.h"
#include "genericProcess.h"
#include "locateSpeaker.h"
#include "ac.h"
#include "lectVideoFrameCreate.h"
#include "computerDistribute.h"
#include "computerProcess.h"


using namespace cv;

int main(int argc, char** argv)
{
  //Hack to avoid unused var warning
  argc++;
  argc--;

  /*
  
  Buffer* readBuffer;
  readBuffer = new Buffer;
  //Buffer* wbBuffer;
  //wbBuffer = new Buffer;
  //Buffer* lectVideoBuffer;
  //lectVideoBuffer = new Buffer;
  Buffer* tempWriterBuffer;
  tempWriterBuffer = new Buffer;
  
  Buffer* compSlidesProcBuffer;
  compSlidesProcBuffer = new Buffer;
  
  Buffer* compMovieWriterBuffer;
  compMovieWriterBuffer = new Buffer;
  
  Buffer* compSlidesWriterBuffer;
  compSlidesWriterBuffer = new Buffer;
  //Buffer* writeBuffer;
  //writeBuffer = new Buffer;

  //Create in reverse order;
  //WriteMod writer(writeBuffer);
  //WhiteBoardProcess wbproc(wbBuffer, writeBuffer);
  //WriteMod videoWriter(lectVideoBuffer);
  //LectVideoFrameCreate lectFrameCreator(wbBuffer, lectVideoBuffer);
  //LocateSpeaker speaker(readBuffer, wbBuffer);
  WriteMod compMovieWriter(compMovieWriterBuffer);
  WriteMod compTempWriter(tempWriterBuffer);
  WriteMod compSlidesWriter(compSlidesWriterBuffer);
  ComputerProcess compSlidesProc(compSlidesProcBuffer, compSlidesWriterBuffer);
  //Module null1(compSlidesWriterBuffer, NULL, 150);
  //Module null2(compMovieWriterBuffer, NULL, 150);
  //Module null3(tempWriterBuffer, NULL, 150);
  ComputerDistribute compDistribute(readBuffer, compSlidesProcBuffer, compMovieWriterBuffer, tempWriterBuffer);
  ReadMod reader(readBuffer);
  
  //Launch in reverse order;
  //boost::thread writerThread(&WriteMod::WriteMats, &writer);
  //boost::thread wbprocThread(&WhiteBoardProcess::run, &wbproc);
  //boost::thread videoWriterThread(&WriteMod::WriteVideo, &videoWriter);
  //boost::thread lectFrameCreatorThread(&LectVideoFrameCreate::run, &lectFrameCreator);
  //boost::thread speakerThread(&LocateSpeaker::run, &speaker);
  boost::thread compMovieWriterThread(&WriteMod::WriteVideo, &compMovieWriter);
  boost::thread compTempWriterThread(&WriteMod::WriteMats, &compTempWriter);
  boost::thread compSlidesWriterThread(&WriteMod::WriteMats, &compSlidesWriter);
  boost::thread compSlidesProcThread(&ComputerProcess::run, &compSlidesProc);
  //boost::thread null1Thread(&Module::nullRun, &null1);
  //boost::thread null2Thread(&Module::nullRun, &null2);
  //boost::thread null3Thread(&Module::nullRun, &null3);
  boost::thread compDistributeThread(&ComputerDistribute::run, &compDistribute);
  boost::thread readerThread(&ReadMod::ReadFromPattern, &reader, argv[1], argv[2]);
  

  //Close in order of operation completion; avoid deadlocks;
  readerThread.join();
  compDistributeThread.join();
  compSlidesProcThread.join();
  //null1Thread.join();
  //null2Thread.join();
  //null3Thread.join();
  compSlidesWriterThread.join();
  compTempWriterThread.join();
  //compMovieWriterThread.join();
  //speakerThread.join();
  //lectFrameCreatorThread.join();
  //videoWriterThread.join();
  //wbprocThread.join();
  //writerThread.join();

  delete readBuffer;
  delete tempWriterBuffer;
  delete compMovieWriterBuffer;
  delete compSlidesWriterBuffer;
  delete compSlidesProcBuffer;
  //delete lectVideoBuffer;
  //delete wbBuffer;
  //delete writeBuffer;

  */

  return 0;

};


/////////////////////////////////////////////////////////////////////////
/////PaolMat tests //////////////////////////////////////////////////////
/*
  Ptr<paolMat> img;
  Ptr<paolMat> bg;
  Ptr<paolMat> improve;
  Ptr<paolMat> remove;
  Ptr<paolMat> contrast;
  Ptr<paolMat> sharpen;
  Ptr<paolMat> shrink;
  Ptr<paolMat> aCopy;
  img = new paolMat;
  img->read("test.ppm","test.ppm",1,1);

  bg=img->returnCreateBackgroundImg(7);
  bg->write();
  //img->createBackgroundImg(7);
  //img->write();
  img->improveInputImg(bg);
  img->write();
  img->removeProf();
  img->write();
  img->createContrast();
  img->write();
  img->sharpen();
  img->write();
  img->shrink();
  img->write(); 
  
  bg=img->returnCreateBackgroundImg(7);
  bg->write();
  improve=img->returnImproveInputImg(bg);
  improve->write();
  remove=improve->returnRemoveProf();
  remove->write();
  shrink=remove->returnShrink();
  shrink->write();
  contrast=shrink->returnCreateContrast();
  contrast->write();
  sharpen=contrast->returnSharpen();
  sharpen->write();
  
*/
/////////////////////////////////////////////////////////
