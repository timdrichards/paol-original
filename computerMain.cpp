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
#include "computerDistribute.h"
#include "computerProcess.h"

void computerPipeline(std::string outDir, std::string dir, std::string first)
{
  std::cout<<outDir<<" "<<dir<<" "<<first<<std::endl;

}

void foo(){

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
 
}
