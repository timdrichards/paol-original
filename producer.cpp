// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>

//Boost for Dir Navigation
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"

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
#include "producer.h"

using namespace cv;
using namespace boost;


void Producer::start(Buffer *buffer)
{
  proBuffer = buffer;
};

void Producer::push(paolMat img)
{
  proBuffer->push(img);
};

void Producer::stop()
{
  boost::mutex::scoped_lock lock(stoplock);
  killMe = true;
  proBuffer->stop();
};

bool Producer::keepRunning()
{
  boost::mutex::scoped_lock lock(stoplock);
  return (!killMe);
};

void ReadFromDisk::run()
{
  
  /*while (keepRunning())
    {
      img = imread("test.jpg");
      push(img);
      boost::this_thread::sleep(restTime);

    };
  */
  readDir();
};


//Boost filesystem namespace for readDir
using namespace boost::filesystem;

void ReadFromDisk::readDir()
{

  path currentDir("media/"); //Set dir to look in
  boost::regex pattern("rtBoard"); //All jpg files
  paolMat img;
  boost::posix_time::seconds restTime(1);

  for (directory_iterator iter(currentDir), end; iter != end; ++iter)
    {
      std::string name = iter->path().leaf();
      //Check if it is a png
      #ifndef _debug_
      //std::cout<<"readFromDisk:: "<<name<<std::endl;
      #endif
      if (regex_search(name, pattern))
	{
	  std::string longPath = "media/";
	  longPath.append(name);
	  #ifndef _debug_
	  //std::cout<<longPath<<std::endl;
	  #endif
	  img.src = imread(longPath);
	  img.name = name;
	  push(img);
	  boost::this_thread::sleep(restTime);
	  #ifndef _debug_
	  //std::cout << name << "\n";
	  #endif
	};
      
    };
  stop();
};

void ReadFromDisk::readFromPattern(char *dir, char* firstImage)
{
  int count, seconds, lastLoaded;
  char name[256];
  FILE *fp;
  paolMat img;
  boost::posix_time::millisec sleepTime(50);
 
  sscanf(firstImage,"image%06d-%10d.ppm",&count,&seconds);
  lastLoaded=seconds;

  sprintf(name,"%simage%06d-%10d.ppm",dir,count,seconds);

  while((seconds-lastLoaded)<20){
    boost::this_thread::sleep(sleepTime);
    //try opening a file of the given name
    img.src.release();
    //std::cout<<"Producer:: Image data is null: "<<img.src.empty()<<std::endl;
    img.read(name,count,seconds);
    if (img.src.data){
      //std::cout<<"Producer:: Read1 image named: "<<name<<std::endl;
      push(img);
      //std::cout<<"\n\nRFP:: img count, time: "<<img.count<<" "<<img.time<<std::endl;
      //std::cout<<"Producer:: Read1.1 image pushed: "<<name<<std::endl;
      lastLoaded=seconds;
      count++;
    } else {
      sprintf(name,"%simage%06d-%10d.ppm",dir,count+1,seconds);
      img.read(name,count,seconds);
      if (img.src.data){
	//std::cout<<"Producer:: Read2 image named: "<<name<<std::endl;
	push(img);
	lastLoaded=seconds;
	count+=2;
      } else {
	seconds++;
      }
    }
    sprintf(name,"%simage%06d-%10d.ppm",dir,count,seconds);
  }
  stop();
};
