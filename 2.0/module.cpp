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

paolMat* Module::pop()
{
  if(consumer)
    return input->pop(myID);
  else
    return NULL;
};

int Module::push(paolMat* m)
{
  if(producer)
    {
      std::cout<<"output->push"<<std::endl;
      output->push(m);
      std::cout<<"output->push"<<std::endl;
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
      std::cout<<"1a"<<std::endl;
      paolMat* img;
      img = pop();
      std::cout<<"1b"<<std::endl;
      while(img != NULL)
	{
	  std::cout<<"1ba"<<std::endl;
	  push(img);
	  std::cout<<"1c"<<std::endl;
	  img = pop();
	};
      std::cout<<"stop"<<std::endl;
      stop();
    }
  else if(consumer)
    {
      paolMat* img;
      img = pop();
      while(img != NULL)
	{
	  delete img;
	  img = pop();
	};
    }
  else
    {
      paolMat* img;
      img = NULL;
      push(img);
      stop();
    };
};

/*
ReadFromPattern::ReadFromPattern(Buffer* buffer, char* dirIn, char* firstImgIn) : Module(NULL, buffer, 0)
{
  dir = dirIn;
  firstImg = firstImgIn;
};

ReadFromPattern::~ReadFromPattern()
{
  delete dir;
  delete firstImg;
};

void ReadFromPattern::run()
{
  int count, seconds, lastLoaded;
  char name[256];

  
  boost::posix_time::millisec sleepTime(100);

  sscanf(firstImg,"image%06d-%10d.ppm",&count,&seconds);
  lastLoaded = seconds;

  sprintf(name,"%simage%06d-%10d.ppm",dir,count,seconds);

  while((seconds-lastLoaded)<20)
    {
      paolMat* img;
      boost::this_thread::sleep(sleepTime);
      img = new paolMat();
      img->read(name,count,seconds);
      if(img->src.data)
	{
	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  sprintf(name,"%simage%06d-%10d.ppm",dir,count+1,seconds);
	  img->read(name,count,seconds);
	  if(img->src.data)
	    {
	      push(img);
	      lastLoaded=seconds;
	      count+=2;
	    }
	  else
	    seconds++;
	}
      sprintf(name,"%simage%06d-%10d.ppm",dir,count,seconds);
      //delete img;
    };
  stop();
};

*/

