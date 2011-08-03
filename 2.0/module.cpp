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
#include "module.h"

#define _debug_

using namespace cv;


Module::Module(Buffer* in, Buffer* out, int arraySize)
{
  arraySize++;
  arraySize--;
  if(in != NULL)
    {
      input = in;
      myID = input->registerConsumer(); 
      consumer = true;
    }
  else
    consumer = false;

  if(out != NULL)
    {
      output = out;
      producer = true;
    }
  else
    producer = false;

  keepRunning = true;

};

Module::~Module()
{

};

Ptr<paolMat> Module::pop()
{
  if(consumer)
    return input->pop(myID);
  else
    return NULL;
};

int Module::push(Ptr<paolMat> m)
{
  if(producer)
    {
      output->push(m);
    }else
    return 1;
  
  return 0;
};

void Module::stop()
{
  boost::mutex::scoped_lock lock(modLock);
  keepRunning = false;
  if(producer)
    output->stop();
};

bool Module::running()
{
  boost::mutex::scoped_lock lock(modLock);
  return keepRunning;
};

void Module::nullRun()
{
  if(consumer&&producer)
    {
      Ptr<paolMat> img;
      img = pop();
      while(img != NULL)
	{
	  push(img);
	  img = pop();
	};
      stop();
    }
  else if(consumer)
    {
      Ptr<paolMat> img;
      img = pop();
      while(img != NULL)
	{
	  //delete img;
	  img = pop();
	};
    }
  else
    {
      Ptr<paolMat> img;
      img = NULL;
      push(img);
      stop();
    };
};

void ReadMod::ReadFromPattern(char* dir, char* firstImg)
{
  int count, seconds, lastLoaded;
  char name[256];
  char fullName[256];
  
  boost::posix_time::millisec sleepTime(10);

  sscanf(firstImg,"image%06d-%10d.ppm",&count,&seconds);
  lastLoaded = seconds;

  sprintf(name,"image");
  sprintf(fullName,"%simage%06d-%10d.ppm",dir,count,seconds);
  Ptr<paolMat> img;
  img = new paolMat();
  while((seconds-lastLoaded)<20)
    {
      boost::this_thread::sleep(sleepTime);
      img->read(fullName,name,count,seconds);
      if(img->src.data)
	{
	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  sprintf(name,"image");
	  sprintf(fullName,"%simage%06d-%10d.ppm",dir,count,seconds);
	  img->read(fullName,name,count,seconds);
	  if(img->src.data)
	    {
	      push(img);
	      lastLoaded=seconds;
	      count+=2;
	    }
	  else
	    seconds++;
	}
      sprintf(name,"image");
      sprintf(fullName,"%simage%06d-%10d.ppm",dir,count,seconds);
    };
  stop();
  //delete img;
};

void WriteMod::WriteMats()
{
  Ptr<paolMat> img;
  img = pop();
  while(img!=NULL)
    {
      img->write();
      //delete img;
      img = pop();
    };
};


void WriteMod::WriteVideo()
{
  Ptr<paolMat> img;
  Ptr<paolMat> cropped;
  img = pop();
  cropped = img->cropFrame(640,480);
  VideoWriter outVideo("outMedia/lectVideo.mp4", CV_FOURCC('F','M','P','4'), 15,cropped->src.size(), true);
  while(img!=NULL)
    {
      cropped = img->cropFrame(640,480);
      outVideo << img->src;
      //img->write();
      //delete img;
      img = pop();
      
    };
  
};
