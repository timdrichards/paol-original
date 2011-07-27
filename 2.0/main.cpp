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

using namespace cv;

int main(int argc, char** argv)
{
  Ptr<paolMat> img;
  Ptr<paolMat> bg;
  
  img = new paolMat;
  img->read("test.ppm","test.ppm",1,1);

  Buffer* readBuffer;
  readBuffer = new Buffer;
  Buffer* writeBuffer;
  writeBuffer = new Buffer;

  WriteMod writer(writeBuffer);
  GenericProcess generic(readBuffer, writeBuffer);
  ReadMod reader(readBuffer);

  boost::thread writerThread(&WriteMod::WriteMats, &writer);
  boost::thread genericThread(&GenericProcess::run, &generic);
  boost::thread readerThread(&ReadMod::ReadFromPattern, &reader, argv[1], argv[2]);
  
  readerThread.join();
  genericThread.join();
  writerThread.join();

  delete readBuffer;
  delete writeBuffer;

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
