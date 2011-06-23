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


#include "poalMat.h"
#include "buffer.h"
#include "producer.h"
#include "consumer.h"
#include "writeToDisk.h"
#include "process.h"
#include "whiteBoardProcess.h"
#include "locateProf.h"
#include "gige.h"


//Toggle Debug Text
//#define _debug_
using namespace cv;
using namespace boost;
//using namespace std;


int main(int argc, char** argv)
{
  
  const boost::posix_time::seconds globalSleepTime(2);
  std::cout<<"MAIN:: Main launched"<<std::endl;
  
  /////////////////////////////////////////////////////////////////////
  //Create All the Buffers/////////////////////////////////////////////
  Buffer* diskWriteBuffer;
  diskWriteBuffer = new Buffer;
  
  Buffer* preProcessBuffer;
  preProcessBuffer = new Buffer;

  Buffer* diskReadBuffer;
  diskReadBuffer = new Buffer;

  Buffer* camera1Buffer;
  camera1Buffer = new Buffer;

  std::cout<<"MAIN:: Buffers created"<<std::endl;

  /////////////////////////////////////////////////////////////////////
  //Create All the Modules/////////////////////////////////////////////

  WriteToDisk frameWriter(diskWriteBuffer, "output", "outMedia/");

  //WriteDebugMovie debugMovieWriter(diskWriteBuffer, "outMedia/movie.mp4", 15);

  Processor nullProc(diskReadBuffer, diskWriteBuffer);
  
  //Accumulate accumulate(diskReadBuffer, diskWriteBuffer);

  //LocateProf findProf(diskReadBuffer, diskWriteBuffer);

  ReadFromDisk diskReader(diskReadBuffer, argv[1], argv[2]);

  std::cout<<"MAIN:: Modules Created"<<std::endl;

  //////////////////////////////////////////////////////////////////////
  // Launch the Modules in reverse order ///////////////////////////////
  //
  // By creating the modules in main we can point boost to the class 
  // methods and launch the methodes as seperate threads, this way we
  //  can call stop from inside main
  //
  // We first pass a referance to the CLASS::Method and then a reference
  // to the instance of the class
  //////////////////////////////////////////////////////////////////////////
  
  boost::thread frameWriterThread(&WriteToDisk::run, &frameWriter);
  //boost::thread debugMovieWriterThread(&WriteDebugMovie::run, &debugMovieWriter);
  //boost::thread accumulateThread(&Accumulate::run, &accumulate);
  boost::thread nullProcThread(&Processor::passOn, &nullProc);
    //boost::thread findProfThread(&LocateProf::run, &findProf);
  boost::thread diskReaderThread(&ReadFromDisk::run, &diskReader);

  std::cout<<"MAIN:: Modules Running, waiting for joing"<<std::endl;

  diskReaderThread.join();
  nullProcThread.join();
  //findProfThread.join();
  //accumulateThread.join();
  //debugMovieWriterThread.join();
  frameWriterThread.join();

  delete diskWriteBuffer;
  delete preProcessBuffer;
  delete diskReadBuffer;
  delete camera1Buffer;
  
  std::cout<<"Main:: Joins complete"<<std::endl;
};






/*

void testConsumer(Buffer *myBuffer)
{
  imWindow consumer;
  consumer.start(myBuffer);
  consumer.run();
};

void testWriteToDisk(Buffer *myBuffer)
{
  WriteToDisk consumer;
  consumer.start(myBuffer);
  consumer.setup("testImg", "outMedia/");
  consumer.run();
  #ifndef _debug_
  //std::cout<<"MAIN:: testWriteToDisk Done"<<std::endl;
  #endif

};

void testReadFromPattern(Buffer *myBuffer,char* dir, char* firstImage) 
{
  ReadFromDisk producer;
  producer.start(myBuffer);
  producer.readFromPattern(dir, firstImage);
  #ifndef _debug_
  //std::cout<<"MAIN:: testReadFromPattern Done"<<std::endl;
  #endif
};

void testProducer(Buffer *myBuffer)
{
  ReadFromDisk producer;
  producer.start(myBuffer);
  producer.run();
};


void testProcess(Buffer *inBuffer, Buffer *outBuffer)
{

  Accumulate acc(inBuffer, outBuffer);
  acc.run();

  //  WhiteBoardProcess debug(inBuffer, outBuffer);
  //debug.run();
  Processor passOn(inBuffer, outBuffer);
  passOn.run();
  #ifndef _debug_
  //std::cout<<"MAIN:: testProcessDone"<<std::endl;
  #endif 
};

void nullProcess(Buffer *inBuffer, Buffer *outBuffer)
{
  Processor nullPros(inBuffer, outBuffer);
  nullPros.passOn();

};

void locateProf(Buffer *inBuffer, Buffer *outBuffer)
{
  LocateProf testLocate(inBuffer, outBuffer);
  testLocate.run();
};

void findDiff(Buffer *inBuffer, Buffer *outBuffer)
{
  test testDiff(inBuffer, outBuffer);
  testDiff.run();

};

void record(Buffer *outBuffer, int camN)
{
  CameraCap cam(outBuffer, camN);
  cam.run();
};

*/
