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
  //baseName = char[basename.length() +1];
  strcpy(baseName, basename.c_str());
  //dir = char[dirIn.length() + 1];
  strcpy(dir, dirIn.c_str());
  
};

WriteToDisk::~WriteToDisk()
{
  //delete baseName;
  //delete dir;
 
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
  cout<<"WriteToDisk:: Popped first Img"<<endl;
  #endif
  while(img.src.data)
    {
      img.name = "output";
      img.print();
      //count++;
      img.copy(pop());
    };
  //gets deallocated elsewhere? errors if we do it here.
  //img.~paolMat();
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

//////////////////////////////////////////////////////////////////
//DEBUG MOVIE ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

WriteDebugMovie::WriteDebugMovie(Buffer* in, std::string dest, int fpsIn) : Consumer(in)
{
  destName = new char[dest.length()+1];
  strcpy(destName, dest.c_str());
  fps = fpsIn;
};

void WriteDebugMovie::run()
{
  cout<<"WriteDebugMovie:: Launched"<<endl;
  
  paolMat img;

  img.copy(pop());

  cv::VideoWriter record(destName, CV_FOURCC('D','I','V','X'), fps, img.src.size(), true);
  //std::cout<<img.src.cols<<" x "<<img.src.rows<<std::endl;
  if(!record.isOpened())
    {
      std::cout<<"Error recording"<<std::endl;
      return;
    };
  int left,top,right,bottom;
  while(img.src.data)
    {
      left = img.camera.x-160;
      right = img.camera.x+160;
      top = img.camera.y-120;
      bottom = img.camera.y+120;
      //std::cout<<"DebugMovie Writer X: "<<img.camera.x<<" y: "<<img.camera.y<<std::endl;
      if(top<0)
	{
	  top = 0;
	  bottom = top+240;
	};
      if(bottom>img.src.rows-1)
	{
	  bottom = img.src.rows -1;
	  top = bottom -240;
	};
      if(left<0)
	{
	  left = 0;
	  right = left+320;
	};
      if(right>img.src.cols-1)
	{
	  right = img.src.cols-1;
	  left = right -320;
	};
      //std::cout<<"rec "<<std::endl;;
      rectangle(img.src, Point(left, top), Point(right, bottom), Scalar(0,0,255, 255),2,8,0);
      rectangle(img.src, Point(img.prof.x-1, img.prof.y-1), Point(img.prof.x+1, img.prof.y+1), Scalar(255,0,0,255),2,8,0);
      //std::cout<<"done"<<std::endl;
      img.name="rec";
      img.print();
      record << img.src;
      std::cout<<"recorded"<<std::endl;
      img.copy(pop());
      std::cout<<"Popped new"<<std::endl;

    };

  std::cout<<"WriteDebugMovie:: Done recording"<<std::endl;
};
