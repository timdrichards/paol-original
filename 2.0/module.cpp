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
      Ptr<paolMat> img;
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
  std::cout<<"WriteMats a"<<std::endl;
  Ptr<paolMat> img;
  img = pop();
  std::cout<<"WriteMats b"<<std::endl;
  while(img!=NULL)
    {
      img->write();
      //delete img;
      img = pop();
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
      Ptr<paolMat> img;
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

