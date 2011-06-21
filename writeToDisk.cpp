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


#include "paolMat.h"
#include "buffer.h"
#include "consumer.h"
#include "writeToDisk.h"

using namespace cv;
using namespace std;
//using namespace boost;

//#define _debug_

WriteToDisk::WriteToDisk(Buffer* inBuffer, std::string basename, std::string dirIn) : Consumer(inBuffer)
{
  baseName = new char[basename.length() +1];
  strcpy(baseName, basename.c_str());
  dir = new char[dirIn.length() + 1];
  strcpy(dir, dirIn.c_str());
  
};

void WriteToDisk::run()
{
 
  cout<<"WriteToDisk:: Launching disk write"<<endl;
  
  paolMat img;
  //int count;
  //count = 1;
  //char name[256];
  #ifndef _debug_
  cout<<"WriteToDisk:: Set variables, about to pop"<<endl;
  #endif
  img.copy(pop());
  #ifndef _debug_
  cout<<"WriteToDisk:: Poped first Img"<<endl;
  #endif
  while(img.src.data)
    {
      img.name = "output";
      img.print();
      //count++;
      img.copy(pop());
    };
  cout<<"WriteToDisk:: Finnished loop, must have recieved null img, closing thread"<<endl;
  
};

WriteMovie::WriteMovie(Buffer* in, std::string dest, int fpsIn) : Consumer(in)
{
  destName = new char[dest.length()+1];
  strcpy(destName, dest.c_str());
  fps = fpsIn;
};

void WriteMovie::run()
{
  cout<<"WriteMovie:: Launched"<<endl;
  
  paolMat img;

  img.copy(pop());

  cv::VideoWriter record(destName, CV_FOURCC('D','I','V','X'), fps, img.src.size(), true);
  std::cout<<img.src.cols<<" x "<<img.src.rows<<std::endl;
  if(!record.isOpened())
    {
      std::cout<<"Error recording"<<std::endl;
      return;
    };
  while(img.src.data)
    {
      record << img.src;
      img.copy(pop());

    };

  std::cout<<"WriteMovie:: Done recording"<<std::endl;
};
