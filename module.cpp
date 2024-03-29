// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/filesystem.hpp>

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

//#define _debug_

namespace fs = boost::filesystem;

using namespace cv;
using namespace std;

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

}

Module::~Module()
{

}

Ptr<paolMat> Module::pop()
{
  if(consumer)
    {
      Ptr<paolMat> temp;
      std::cout<<"Module: input->pop"<<std::endl;
      temp = input->pop(myID);
      std::cout<<"Module: input->popped"<<std::endl;
      return temp;//input->pop(myID);
      
    }
  else
    return NULL;
}

int Module::push(Ptr<paolMat> m)
{
  if(producer)
    {
      output->push(m);
    }else
    return 1;
  
  return 0;
}

void Module::stop()
{
  boost::mutex::scoped_lock lock(modLock);
  keepRunning = false;
  if(producer)
    output->stop();
}

bool Module::running()
{
  boost::mutex::scoped_lock lock(modLock);
  return keepRunning;
}

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
	}
      stop();
    }
  else if(consumer)
    {
      Ptr<paolMat> img;
      img = pop();
      while(img != NULL)
	{
#ifdef _debug_
	  std::cout<<"NullMod Throwing out: "<<img->name<<img->count<<"-"<<img->time<<std::endl;
#endif
	  //delete img;
	  img = pop();
	}
    }
  else
    {
      Ptr<paolMat> img;
      img = NULL;
      push(img);
      stop();
    }
}

void ReadMod::ReadFromPattern(char* dir, char* firstImg)
{
  int count, seconds, lastLoaded, tempCount, tempSeconds;
  char name[256];
  char fullName[256];
  
  //boost::posix_time::millisec sleepTime(1);

  sscanf(firstImg,"frame%06d-%10d.ppm",&count,&seconds);
  lastLoaded = seconds;

  sprintf(name,"frame");
  sprintf(fullName,"%sframe%06d-%10d.ppm",dir,count,seconds);
  Ptr<paolMat> img;
  img = new paolMat();
  while((seconds-lastLoaded)<20)
    {
      //boost::this_thread::sleep(sleepTime);
      img->read(fullName,name,count,seconds);
      if(img->src.data)
	{
	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  tempCount=count;
	  while(!img->src.data && tempCount-count<25)
	    {
	      tempSeconds=seconds;
	      while(!img->src.data && tempSeconds-seconds<25)
		{
		  tempSeconds++;
		  sprintf(name,"frame");
		  sprintf(fullName,"%sframe%06d-%10d.ppm",dir,tempCount,tempSeconds);
		  img->read(fullName,name,tempCount,tempSeconds);
		  tempSeconds++;
		}
	      tempCount++;
	    }
	  seconds=tempSeconds-1;
	  count=tempCount-1;
	}
      sprintf(name,"frame");
      sprintf(fullName,"%sframe%06d-%10d.ppm",dir,count,seconds);
    }
  stop();
  //delete img;
}
void ReadMod::ReadFromPatternComp(char* dir, char* firstImg)
{
  int count, seconds, lastLoaded, tempCount, tempSeconds;
  char name[256];
  char fullName[256];
  char ext[5];
  
  boost::posix_time::millisec sleepTime(1);

  sscanf(firstImg,"screenscreen%06d-%10d.%s",&count,&seconds,&ext);
  lastLoaded = seconds;

  sprintf(name,"frame");
  sprintf(fullName,"%sscreenscreen%06d-%10d.%s",dir,count,seconds,ext);
  Ptr<paolMat> img;
  img = new paolMat();
  while((seconds-lastLoaded)<20)
    {
      //boost::this_thread::sleep(sleepTime);
      img->read(fullName,name,count,seconds);
      if(img->src.data)
	{
	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  tempCount=count;
	  
	  while(!img->src.data && tempCount-count<25)
	    {
	      tempSeconds=seconds;
	      while(!img->src.data && tempSeconds-seconds<25)
		{
		  //cout<<"Read: did not find image: screenscreen "<<tempCount<<"-"<<tempSeconds<<endl;
		  //tempSeconds++;
		  sprintf(name,"frame");
		  sprintf(fullName,"%sscreenscreen%06d-%10d.%s",dir,tempCount,tempSeconds,ext);
		  img->read(fullName,name,tempCount,tempSeconds);
		  tempSeconds++;
		}
	      tempCount++;
	    }
	  seconds=tempSeconds-1;
	  count=tempCount-1;
	}
      sprintf(name,"frame");
      sprintf(fullName,"%sscreenscreen%06d-%10d.%s",dir,count,seconds,ext);
    }
  stop();
  //delete img;
}


void ReadMod::ReadFromPatternFlip(char* dir, char* firstImg)
{
  int count, seconds, lastLoaded, tempCount, tempSeconds;
  char name[256];
  char fullName[256];
  
  boost::posix_time::millisec sleepTime(1);

  sscanf(firstImg,"frame%06d-%10d.ppm",&count,&seconds);
  lastLoaded = seconds;

  sprintf(name,"frame");
  sprintf(fullName,"%sframe%06d-%10d.ppm",dir,count,seconds);
  Ptr<paolMat> img;
  img = new paolMat();
  while((seconds-lastLoaded)<20)
    {
      //boost::this_thread::sleep(sleepTime);
      img->read(fullName,name,count,seconds);
      if(img->src.data)
	{
	  

	  Mat rot_mat(2, 3, CV_32FC1);
	  Mat src, warp_rotate_dst;
	  src = img->src;
	  Point center = Point(src.cols/2, src.rows/2);
	  double angle = 180.0;
	  double scale = 1.0;

	  rot_mat = getRotationMatrix2D(center, angle, scale);
	  warpAffine(src, warp_rotate_dst, rot_mat, src.size());
	  
	  img->src = warp_rotate_dst;

	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  tempCount=count;
	  while(!img->src.data && tempCount-count<25)
	    {
	      tempSeconds=seconds;
	      while(!img->src.data && tempSeconds-seconds<25)
		{
		  tempSeconds++;
		  sprintf(name,"frame");
		  sprintf(fullName,"%sframe%06d-%10d.ppm",dir,tempCount,tempSeconds);
		  img->read(fullName,name,tempCount,tempSeconds);
		  tempSeconds++;
		}
	      tempCount++;
	    }
	  seconds=tempSeconds-1;
	  count=tempCount-1;
	}
      sprintf(name,"frame");
      sprintf(fullName,"%sframe%06d-%10d.ppm",dir,count,seconds);
    }
  stop();
  //delete img;
}

void ReadMod::ReadFromPatternFlipExt(char* dir, char* firstImg, char* pattern)
{
  int count, seconds, lastLoaded, tempCount, tempSeconds;
  char name[256];
  char fullName[256];
  char ext[5];
  
  boost::posix_time::millisec sleepTime(1);

  sscanf(firstImg,"wboardwboard%06d-%10d.%s",&count,&seconds,&ext);
  lastLoaded = seconds;

  sprintf(name,"frame");
  sprintf(fullName,"%swboardwboard%06d-%10d.%s",dir,count,seconds,ext);
  Ptr<paolMat> img;
  img = new paolMat();
  while((seconds-lastLoaded)<20)
    {
      //boost::this_thread::sleep(sleepTime);
      img->read(fullName,name,count,seconds);
      if(img->src.data)
	{
	  

	  Mat rot_mat(2, 3, CV_32FC1);
	  Mat src, warp_rotate_dst;
	  src = img->src;
	  Point center = Point(src.cols/2, src.rows/2);
	  double angle = 180.0;
	  double scale = 1.0;

	  rot_mat = getRotationMatrix2D(center, angle, scale);
	  warpAffine(src, warp_rotate_dst, rot_mat, src.size());
	  
	  img->src = warp_rotate_dst;

	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  tempCount=count;
	  
	  while(!img->src.data && tempCount-count<25)
	    {
	      tempSeconds=seconds;
	      while(!img->src.data && tempSeconds-seconds<25)
		{
		  //cout<<"Read: did not find image: wboardwboard "<<tempCount<<"-"<<tempSeconds<<endl;
		  //tempSeconds++;
		  sprintf(name,"frame");
		  sprintf(fullName,"%swboardwboard%06d-%10d.%s",dir,tempCount,tempSeconds,ext);
		  img->read(fullName,name,tempCount,tempSeconds);
		  tempSeconds++;
		}
	      tempCount++;
	    }
	  seconds=tempSeconds-1;
	  count=tempCount-1;
	}
      sprintf(name,"frame");
      sprintf(fullName,"%swboardwboard%06d-%10d.%s",dir,count,seconds,ext);
    }
  stop();
  //delete img;
}

void ReadMod::ReadFromPatternFlipExtCrop(char* dir, char* firstImg, int x, int y, int width, int height)
{
  int count, seconds, lastLoaded, tempCount, tempSeconds;
  char name[256];
  char fullName[256];
  char ext[5];
  
  boost::posix_time::millisec sleepTime(1);

  sscanf(firstImg,"wboardwboard%06d-%10d.%s",&count,&seconds,&ext);
  lastLoaded = seconds;

  sprintf(name,"frame");
  sprintf(fullName,"%swboardwboard%06d-%10d.%s",dir,count,seconds,ext);
  Ptr<paolMat> img;
  img = new paolMat();
  Ptr<paolMat> temp;
  temp = new paolMat();
  while((seconds-lastLoaded)<20)
    {
      //boost::this_thread::sleep(sleepTime);
      img->read(fullName,name,count,seconds);
      if(img->src.data)
	{
	  

	  Mat rot_mat(2, 3, CV_32FC1);
	  Mat src, warp_rotate_dst;
	  src = img->src;
	  Point center = Point(src.cols/2, src.rows/2);
	  double angle = 180.0;
	  double scale = 1.0;

	  rot_mat = getRotationMatrix2D(center, angle, scale);
	  warpAffine(src, warp_rotate_dst, rot_mat, src.size());
	  
	  img->src = warp_rotate_dst;
	  temp->copy(img);
	  img = temp->crop(x,y,width,height);
	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  tempCount=count;
	  
	  while(!img->src.data && tempCount-count<25)
	    {
	      tempSeconds=seconds;
	      while(!img->src.data && tempSeconds-seconds<25)
		{
		  //cout<<"Read: did not find image: wboardwboard "<<tempCount<<"-"<<tempSeconds<<endl;
		  //tempSeconds++;
		  sprintf(name,"frame");
		  sprintf(fullName,"%swboardwboard%06d-%10d.%s",dir,tempCount,tempSeconds,ext);
		  img->read(fullName,name,tempCount,tempSeconds);
		  tempSeconds++;
		}
	      tempCount++;
	    }
	  seconds=tempSeconds-1;
	  count=tempCount-1;
	}
      sprintf(name,"frame");
      sprintf(fullName,"%swboardwboard%06d-%10d.%s",dir,count,seconds,ext);
    }
  stop();
  //delete img;
}


void ReadMod::ReadFromPatternFlipTiff(char* dir, char* firstImg)
{
  int count, seconds, lastLoaded, tempCount, tempSeconds;
  char name[256];
  char fullName[256];
  
  boost::posix_time::millisec sleepTime(1);

  sscanf(firstImg,"frame%06d-%10d.tiff",&count,&seconds);
  lastLoaded = seconds;

  sprintf(name,"frame");
  sprintf(fullName,"%sframe%06d-%10d.tiff",dir,count,seconds);
  Ptr<paolMat> img;
  img = new paolMat();
  while((seconds-lastLoaded)<20)
    {
      //boost::this_thread::sleep(sleepTime);
      img->read(fullName,name,count,seconds);
      if(img->src.data)
	{
	  

	  Mat rot_mat(2, 3, CV_32FC1);
	  Mat src, warp_rotate_dst;
	  src = img->src;
	  Point center = Point(src.cols/2, src.rows/2);
	  double angle = 180.0;
	  double scale = 1.0;

	  rot_mat = getRotationMatrix2D(center, angle, scale);
	  warpAffine(src, warp_rotate_dst, rot_mat, src.size());
	  
	  img->src = warp_rotate_dst;

	  push(img);
	  lastLoaded=seconds;
	  count++;
	}
      else
	{
	  tempCount=count;
	  while(!img->src.data && tempCount-count<25)
	    {
	      tempSeconds=seconds;
	      while(!img->src.data && tempSeconds-seconds<25)
		{
		  tempSeconds++;
		  sprintf(name,"frame");
		  sprintf(fullName,"%sframe%06d-%10d.tiff",dir,tempCount,tempSeconds);
		  img->read(fullName,name,tempCount,tempSeconds);
		  tempSeconds++;
		}
	      tempCount++;
	    }
	  seconds=tempSeconds-1;
	  count=tempCount-1;
	}
      sprintf(name,"frame");
      sprintf(fullName,"%sframe%06d-%10d.tiff",dir,count,seconds);
    }
  stop();
  //delete img;
}

void WriteMod::WriteMats()
{
  Ptr<paolMat> img;
  img = pop();
  while(img!=NULL)
    {
      img->write();
      //delete img;
      img = pop();
    }
}

void WriteMod::WriteMats(std::string outDir)
{
  Ptr<paolMat> img;
  img = pop();
  while(img!=NULL)
    {
      std::cout<<"WriteMod:: about to write: "<<outDir<<std::endl;
      img->write(outDir);
      //delete img;
      img = pop();
    }
}

void WriteMod::WriteSlides(std::string outDir, std::string name, int start)
{

  fs::remove_all(name);
  fs::create_directory(name);

  std::ofstream log;
  std::ofstream difs;
  
  char logLoc[1024];
  char difsLoc[1024];
  
  std::strcat(logLoc,outDir.c_str());
  std::strcat(difsLoc,outDir.c_str());
  
  std::strcat(logLoc,"slides.txt");
  std::strcat(difsLoc,"difs.txt");
  
  log.open(logLoc, ios::out | ios::trunc);
  difs.open(difsLoc, ios::out | ios::trunc);
  
  log.close();
  difs.close();

  Ptr<paolMat> img;
  img = pop();
  int count;
  count = 0;

  
  while(img!=NULL)
    {
      img->name = name;
      img->count = count;
      log.open(logLoc, ios::out | ios::app);
      difs.open(difsLoc, ios::out | ios::app);
      
      char temp[256];
      std::string longName = name;
      sprintf(temp,"_%06d.png",count);
      longName.append(temp);
      
      log << longName <<" | " << (img->time - start) << "\n";
      difs << longName << " | " << (img->time - start) << " | " << img->difs << "\n";
      img->writeByCount(outDir);
      count++;

      log.close();
      difs.close();
      img = pop();
    }
}

void WriteMod::WriteMatsByCount(std::string outDir)
{
  Ptr<paolMat> img;
  img = pop();
  int count = -1;
  int second;
  //## Target FPS ////////////////////////////////////
  int tFPS = 15;
  //## Current FPS (reset every second) ///////////////
  int cFPS = 0;
  img->count = count;
  second = img->time;
  while(img!=NULL)
    {
      if( (second == img->time) && (cFPS >= tFPS) )
	std::cout<<"WriteMod:: Dropping frame, to many this second"<<std::endl;
      else if( (second == img->time) && (cFPS < tFPS) )
	{
	  count++;
	  std::cout<<"WriteMod:: about to write: "<<outDir<<std::endl;
	  img->count = count;
	  img->writeByCount(outDir);
	  img = pop();
	  
	  
	  cFPS++;
	}
      else if(second < img->time)
	{
	  while(cFPS < tFPS)
	    {
	      std::cout<<"WriteMod:: CFPS < 15 second < time"<<std::endl;
	      count++;
	      img->count = count;
	      std::cout<<"WriteMod:: about to write (duplicate): "<<outDir<<std::endl;
	      img->writeByCount(outDir);
	      cFPS++;
	    }
	  std::cout<<"WriteMod:: tFPS achieved moving to next second"<<std::endl;
      
	  second++;
	  cFPS = 0;
	}
      
    }
}



void WriteMod::WriteVideo(char* lable)
{
  Ptr<paolMat> img;
  char dir[256];
  char fullPath[256];
  char extention[5];
  int lastImgCount;
  int startTime;

  strcpy(dir, "outMedia/");
  strcpy(extention, "mpeg");
  startTime = 0;

  
  
  img = pop();
  if(img !=NULL)
    {
      lastImgCount = img->count;
      startTime = img->time;
      if(startTime<0)
	startTime = 0;
      
      sprintf(fullPath, "%s/%s%i06.%s", dir,lable,startTime,extention);
      VideoWriter outVideo(fullPath, CV_FOURCC('M','P','4','2'), 15,img->src.size(), true);
      while(img!=NULL)
	{
#ifdef _debug_
	  img->name = "Frame";
	  img->write();
#endif
	  outVideo << img->src;
	  //img->write();
	  //delete img;
	  lastImgCount = img->count;
	  img = pop();
	  
	}
    }
}

void WriteMod::WriteVideo()
{
  WriteVideo("unkown");
}

void WriteMod::WriteCompVideo(std::string outDir, int start)
{
  std::ofstream log;
  char logLoc[1024];
  std::strcat(logLoc,outDir.c_str());
  std::strcat(logLoc,"screenVideo.txt");
  log.open(logLoc, ios::out | ios::trunc);
  log.close();
  Ptr<paolMat> img;
  Ptr<paolMat> black;
  black = new paolMat();
  img = pop();
  int lastSecond;
  int count = -1;
  int second;
  //## Target FPS ////////////////////////////////////
  int tFPS = 15;
  //## Current FPS (reset every second) ///////////////
  int cFPS = 0;
  // img->count = count;
  // second = img->time;
  // lastSecond = start;
  // black->copy(img);
  // black->src = Scalar(0,0,0);
  while(img!=NULL)
    {
      img->count = count;
      second = img->time;
      lastSecond = start;
      black->copy(img);
      black->src = Scalar(0,0,0);
      
      //If more then 10 seconds have gone by
      if(second > lastSecond +10)
	{
	  log.open(logLoc, ios::out | ios::app);
	  log << "Blank " <<lastSecond;
	  for(lastSecond;lastSecond < second; lastSecond++)
	    for(int a = 0; a < tFPS; a++)
	      {
		count++;
		black->count = count;
		black->writeByCount(outDir);
	      }
	  lastSecond = second;
	  img->count = count;
	  log << " | "<< lastSecond << "\n";
	  log.close();
	}
      // FIXME!
      if( (second == img->time) && (cFPS >= tFPS) ) {
      	std::cout<<"WriteMod:: Dropping frame, to many this second"<<std::endl;
      	std::cout<<"Image Name: "<<img->name<<endl;
      	std::cout<<"Image Count: "<<img->count<<endl;
      	exit(1);
      }
      else if( (second == img->time) && (cFPS < tFPS) )
	{
	  count++;
	  std::cout<<"WriteMod:: about to write: "<<outDir<<std::endl;
	  img->count = count;
	  img->writeByCount(outDir);
	  img = pop();
	  if(img != NULL)
	    second = img->time;
	  cFPS++;
	}
      else if(second < img->time)
	{
	  while(cFPS < tFPS)
	    {
	      std::cout<<"WriteMod:: CFPS < 15 second < time"<<std::endl;
	      count++;
	      img->count = count;
	      std::cout<<"WriteMod:: about to write (duplicate): "<<outDir<<std::endl;
	      img->writeByCount(outDir);
	      cFPS++;
	    }
	  std::cout<<"WriteMod:: tFPS achieved moving to next second"<<std::endl;
      
	  second++;
	  lastSecond++;
	  cFPS = 0;
	}
      
    }
  log.close();
}
