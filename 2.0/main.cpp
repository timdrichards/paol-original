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
//#include "epiphanCapture.h"
#include "WebCamCapture.h"
#include "usbCam.h"
#include "sample-capture.h"
//#include "gigE.h"
//#include "PvApi.h"
//#include <ImageLib.h>


#define _wb_
//#define _compCap_
//#define _usbCompCap_
//#define _usbCam_
//#define _live_
//#define _gigE2Disk_
//define _ethCompCap_



#define _readFromDisk_
using namespace cv;

#ifdef _readFromDisk_
int main(int argc, char** argv)
{
  //Hack to avoid unused var warning
  argc++;
  argc--;
  char outDir[1024];
  strcpy(outDir, argv[1]);
  char WBdir[1024];
  //"/home/diw08/code/testData/"); "frame000000-1327327396.ppm");
  strcpy(WBdir, argv[2]);
  char WBfirst[1024];
  strcpy(WBfirst, argv[3]);
#endif
#ifdef _live_
int main()
{
#endif

#ifdef _wb_
  ////////////////////////////////
  /// Camera/WB Cap/Proc Buffers /
  //Buffer* webCamCaptureBuffer;
  //webCamCaptureBuffer = new Buffer;
  Buffer* wbBuffer;
  wbBuffer = new Buffer;
  Buffer* lectVideoBuffer;
  lectVideoBuffer = new Buffer;
  Buffer* wbSlidesWriteBuffer;
  wbSlidesWriteBuffer = new Buffer;
  Buffer* wbReadBuffer;
  wbReadBuffer = new Buffer;

  //////////////////////////////////////
  // WB Cap/Proc Modules
  WriteMod presVideoFramesWriter(lectVideoBuffer);
  WriteMod wbSlidesWriter(wbSlidesWriteBuffer);
  WhiteBoardProcess wbproc(wbBuffer, wbSlidesWriteBuffer);
  LectVideoFrameCreate lectFrameCreator(wbBuffer, lectVideoBuffer);
  LocateSpeaker locateSpeaker(wbReadBuffer, wbBuffer);
  ReadMod readFromDisk(wbReadBuffer);
#endif  

#ifdef _compCap_
  ///////////////////////////////
  /// Computer Cap/Proc Buffers /
  Buffer* epiphanBuffer;
  epiphanBuffer = new Buffer;
  Buffer* tempWriterBuffer;
  tempWriterBuffer = new Buffer;
  Buffer* compSlidesProcBuffer;
  compSlidesProcBuffer = new Buffer;
  Buffer* compMovieWriterBuffer;
  compMovieWriterBuffer = new Buffer;
  Buffer* compSlidesWriterBuffer;
  compSlidesWriterBuffer = new Buffer;

  /////////////////////////////////////
  // Computer Cap/Proc Modules
  WriteMod compMovieWriter(compMovieWriterBuffer);
  WriteMod compTempWriter(tempWriterBuffer);
  WriteMod compSlidesWriter(compSlidesWriterBuffer);
  ComputerProcess compSlidesProc(compSlidesProcBuffer, compSlidesWriterBuffer);
  ComputerDistribute compDistribute(epiphanBuffer, compSlidesProcBuffer, compMovieWriterBuffer, tempWriterBuffer);
#ifdef _usbCompCap_
  EpiphanCapture compGrabber(epiphanBuffer);
#endif
#ifdef _ethCompCap_
  EpiphanCapture compGrabber(epiphanBuffer);
#endif
#endif

#ifdef _usbCam_
  Buffer* usbCamCapBuffer;
  usbCamCapBuffer = new Buffer;

  //WriteMod usbCamFrameWriter(usbCamCapBuffer);
  UsbCam usbCam(usbCamCapBuffer);
#endif

#ifdef _gigE2Disk_
  Buffer* gigE2DiskBuffer;
  gigE2DiskBuffer = new Buffer;
  
  WriteMod gigE2DiskWriter(gigE2DiskBuffer);
  GigE gigE2DiskCam(gigE2DiskBuffer, 0);
#endif
  
  
#ifdef _wb_
  ///////////////////////////////////////
  // Launch WB mods in revers order /////
  
  boost::thread presVideoFramesWriterThread(boost::bind(&WriteMod::WriteMats, &presVideoFramesWriter, outDir));
  boost::thread wbSlidesWriterThread(boost::bind(&WriteMod::WriteMats, &wbSlidesWriter, outDir));
  boost::thread wbprocThread(boost::bind(&WhiteBoardProcess::run, &wbproc, 10));
  boost::thread lectFrameCreatorThread(&LectVideoFrameCreate::run, &lectFrameCreator);
  boost::thread locateSpeakerThread(&LocateSpeaker::run, &locateSpeaker);
  boost::thread wbReadThread(&ReadMod::ReadFromPatternFlip, &readFromDisk, WBdir,WBfirst);
#endif

#ifdef _compCap_
  ///////////////////////////////////////
  // Launch Comp mods in reverse order
  boost::thread compMovieWriterThread(&WriteMod::WriteCompVideo, &compMovieWriter);
  boost::thread compTempWriterThread(&WriteMod::WriteMats, &compTempWriter);
  boost::thread compSlidesWriterThread(&WriteMod::WriteMats, &compSlidesWriter);
  boost::thread compSlidesProcThread(&ComputerProcess::run, &compSlidesProc);
  boost::thread compDistributeThread(&ComputerDistribute::run, &compDistribute);
  #ifdef _usbCompCap_
  boost::thread compGrabberThread(&EpiphanCapture::run, &compGrabber);
  #endif
  #ifdef _ethCompCap_
  boost::thread compGrabberThread(&EpiphanCapture::runEthCap, &compGrabber);
  #endif
#endif

#ifdef _usbCam_
  //boost::thread usbCamFrameWriterThread(&WriteMod::WriteMats, &usbCamFrameWriter);
  boost::thread usbCamThread(&UsbCam::run, &usbCam);
#endif

#ifdef _gigE2Disk_
  std::cout<<"Main: Launching gigE2Disk"<<std::endl;
  //boost::thread gigE2DiskWriterThread(&WriteMod::WriteMats, &gigE2DiskWriter);
  boost::thread gigE2DiskCamThread(&GigE::run, &gigE2DiskCam);
#endif

#ifdef _wb_
  wbReadThread.join();
  locateSpeakerThread.join();
  lectFrameCreatorThread.join();
  wbprocThread.join();
  wbSlidesWriterThread.join();
  presVideoFramesWriterThread.join();

  std::cout<<"Main: about to begin deleting WB buffers"<<std::endl;
  //delete webCamCaptureBuffer;
  delete wbBuffer;
  std::cout<<"Main: deleted buffer 0"<<std::endl;
  delete lectVideoBuffer;
  std::cout<<"Main: deleted buffer 1"<<std::endl;
  //delete lectVideoBuffer;
  std::cout<<"Main: deleted buffer 2"<<std::endl;
  delete wbReadBuffer;
  std::cout<<"Main: finished deleting WB buffers"<<std::endl;
#endif

#ifdef _compCap_
  compGrabberThread.join();
  compDistributeThread.join();
  compSlidesProcThread.join();
  compSlidesWriterThread.join();
  compTempWriterThread.join();
  compMovieWriterThread.join();

  delete epiphanBuffer;
  delete tempWriterBuffer;
  delete compSlidesProcBuffer;
  delete compMovieWriterBuffer;
  delete compSlidesWriterBuffer;
#endif

#ifdef _usbCam_
  usbCamThread.join();
  //usbCamFrameWriterThread.join();

  delete usbCamCapBuffer;
#endif

#ifdef _gigE2Disk
  gigE2DiskWriterThread.join();
  gigE2DiskCamThread.join();

  delete gigE2DiskBuffer;
#endif
 

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
