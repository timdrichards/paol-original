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

using namespace cv;

int main(int arc, char** argv)
{

  //Buffer* diskReadBuffer;
  //diskReadBuffer = new Buffer();

  Buffer* diskWriteBuffer;
  diskWriteBuffer = new Buffer();

  ReadFromPattern readFromPatternMod(diskWriteBuffer, argv[1], argv[2]);
  Module dumpMod(NULL, diskWriteBuffer, 1);

  /*
  for(int i = 0; i < 10; i++)
    {
      paolMat* img;
      img = new paolMat;
      img->src = imread("test.jpg");
      diskWriteBuffer->push(img);

    };
  */

  readFromPatternMod.run();
  dumpMod.nullRun();


  delete diskWriteBuffer;
  
  std::cout<<"End of main"<<std::endl;
  return 0;

};
