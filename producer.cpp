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

#include "buffer.h"
#include "producer.h"

using namespace cv;
using namespace boost;


void Producer::start(Buffer *buffer)
{
  myBuffer = buffer;
};

void Producer::push(cv::Mat img)
{
  myBuffer->push(img);
};

void Producer::stop()
{
  boost::mutex::scoped_lock lock(stoplock);
  killMe = true;
};

bool Producer::keepRunning()
{
  boost::mutex::scoped_lock lock(stoplock);
  return (!killMe);
};

void readFromDisk::run()
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

void readFromDisk::readDir()
{

  path currentDir("media/"); //Set dir to look in
  boost::regex pattern("rtBoard"); //All jpg files
  cv::Mat img;
  boost::posix_time::seconds restTime(1);

  for (directory_iterator iter(currentDir), end; iter != end; ++iter)
    {
      std::string name = iter->path().leaf();
      //Check if it is a png
      std::cout<<"readFromDisk:: "<<name<<std::endl;
      if (regex_search(name, pattern))
	{
	  std::string longPath = "media/";
	  longPath.append(name);
	  std::cout<<longPath<<std::endl;
	  img = imread(longPath);
	  push(img);
	  boost::this_thread::sleep(restTime);
	  std::cout << name << "\n";
	};
      stop();
    };
  
};
