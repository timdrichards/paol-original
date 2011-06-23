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

#include "buffer.h"


#define _debug_

using namespace cv;
using namespace boost;


//////////paolMat////////////////////////

paolMat::paolMat()
{
  cv::Mat null;
  src = null;
  count = -1;
  time = -1;
  name = "No Name";
  prof = Point(0,0);
  camera = Point(0,0);
};

paolMat::~paolMat()
{
  src.release();
};

void paolMat::print()
{
  char temp[256];
  std::string longName = "outMedia/";
  longName.append(name);
  sprintf(temp, "%06d-%010d.png",count,time);
  longName.append(temp);
  cv::imwrite(longName, src);
  //std::cout<<longName<<std::endl;
  
};


void paolMat::copy(paolMat m)
{
  src.release();
  m.src.copyTo(src);
  count = m.count;
  time = m.time;
  name = m.name;
  planes = m.planes;
  prof = m.prof;
  camera = m.camera;
};

void paolMat::copy(paolMat* m)
{
  src.release();
  m->src.copyTo(src);
  count = m->count;
  time = m->time;
  name = m->name;
  planes = m->planes;
  prof = m->prof;
  camera = m->camera;

};

void paolMat::edges()
{
  split();
  cv::Mat temp(src);
  temp = Scalar(0,0,0);
  cv::vector<cv::Mat> tempPlanes;
  cv::split(temp, tempPlanes);
  int total;
  
  for (int y = 1; y < src.rows-1; y++)
    {
      uchar* sRT = planes[2].ptr<uchar>(y-1);
      uchar* sGT = planes[1].ptr<uchar>(y-1);
      uchar* sBT = planes[0].ptr<uchar>(y-1);

      uchar* sRM = planes[2].ptr<uchar>(y);
      uchar* sGM = planes[1].ptr<uchar>(y);
      uchar* sBM = planes[0].ptr<uchar>(y);

      uchar* sRB = planes[2].ptr<uchar>(y+1);
      uchar* sGB = planes[1].ptr<uchar>(y+1);
      uchar* sBB = planes[0].ptr<uchar>(y+1);

      uchar* tR = tempPlanes[2].ptr<uchar>(y);
      uchar* tG = tempPlanes[1].ptr<uchar>(y);
      uchar* tB = tempPlanes[0].ptr<uchar>(y);

      for (int x = 1; x < src.cols-1; x++)
	{
	  total=-2*sRT[x-1]-sRT[x]-sRM[x-1];
	  total+=sRM[x+1]+sRB[x]+2*sRB[x+1];

	  total+=-2*sGT[x-1]-sGT[x]-sGM[x-1];
	  total+=sGM[x+1]+sGB[x]+2*sGB[x+1];

	  total+=-2*sBT[x-1]-sBT[x]-sBM[x-1];
	  total+=sBM[x+1]+sBB[x]+2*sBB[x+1];

	  total=abs(total);
	  if(total>128)
	    tR[x]=255;
	  if(total>512)
	    tG[x]=255;
	  if(total>768)
	    tB[x]=255;
	};
    };
  cv::merge(tempPlanes, temp);
  temp.copyTo(src);
};

void paolMat::read(std::string fileName,int countIn, int timeIn)
{
  src = imread(fileName);
  name = fileName;
  count=countIn;
  time=timeIn;
};

void paolMat::merge()
{
  //  std::cout<<"PaolMat Merge called, planes num is: "<<planes.size()<<std::endl;
  if(planes.size() > 0)
    {
      //std::cout<<"Merging"<<std::endl;
      cv::merge(planes, src);
    }else{
    //std::cout<<"Error merging"<<std::endl;
  };
};

//This is a slow method for testing, not production//
void paolMat::invert()
{
  cv::vector<cv::Mat> planes;
  cv::split(src, planes);
  int temp;
  
  for (int y = 0; y < src.rows; y++)
    {
      uchar* red = planes[0].ptr<uchar>(y);
      uchar* green = planes[1].ptr<uchar>(y);
      uchar* blue = planes[2].ptr<uchar>(y);
      for (int x = 0; x < src.cols; x++)
	{
	  temp=red[x]+green[x]+blue[x];
	  if (temp>40)
	    temp=255;
	  red[x]=temp;
	  blue[x]=temp;
	  green[x]=temp;
	  /*	  if (temp>30)
	    green[x]=255;
	  else
	    green[x]=0;
	  */
	};
      
    };
  
  cv::merge(planes, src);
};

void paolMat::createBackgroundImg(int kernalSize)
{
  cv::Point centerPoint(-1,-1);
  cv::blur(src, src, cv::Size(kernalSize,kernalSize), centerPoint, 1);
  name = "backgroundImg";
};

void paolMat::improveInputImg(paolMat background)
{
  background.split();
  split();
  int temp;
  
  for (int channel = 0; channel <3; channel++)
    for (int y = 0; y < src.rows; y++)
      {
	uchar* inputPtr = planes[channel].ptr<uchar>(y);
	uchar* backgroundPtr = background.planes[channel].ptr<uchar>(y);
	
	for (int x = 0; x < src.cols; x++)
	  {
	    
	    temp  = (inputPtr[x] * 255) / backgroundPtr[x];
	    if(temp > 255)
	      inputPtr[x] = 255;
	    else
	      inputPtr[x] = temp;
	  };

      };
  
  merge();
  name = "improvedImage";
};
void paolMat::removeProf()
{
  name = "noProf";
};
void paolMat::createContrast()
{
  split();
  //int thresh;
  int temp;
  int ave;
  for (int y = 0; y < src.rows; y++)
      {
	uchar* RInPtr = planes[0].ptr<uchar>(y);		
	uchar* BInPtr = planes[1].ptr<uchar>(y);	
	uchar* GInPtr = planes[2].ptr<uchar>(y);
			
	for (int x = 0; x < src.cols; x++)
	  {
	    ave = (RInPtr[x] + BInPtr[x] + GInPtr[x]) /3;
	    if(ave <240 || ((RInPtr[x] < 220) || (BInPtr[x] < 220) || (GInPtr[x] < 220)))
	      {
		temp = RInPtr[x]-(255-RInPtr[x]);
		if (temp < 0)
		  temp = 0;
		//else if (temp > 188)
		//  temp = 255;
		RInPtr[x] = temp;

		temp = BInPtr[x]-(255-BInPtr[x]);
		if (temp < 0)
		  temp = 0;
		//else if (temp > 188)
		//  temp = 255;
		BInPtr[x] = temp;

		temp = GInPtr[x]-(255-GInPtr[x]);
		if (temp < 0)
		  temp = 0;
		//else if (temp > 188)
		//  temp = 255;
		GInPtr[x] = temp;
	       
	      }else
	      {
		RInPtr[x] = 255;
		GInPtr[x] = 255;
		BInPtr[x] = 255;

	      };
	    
	  };

      };
  merge();
  name = "ContrastImg";
};


void paolMat::sharpen()
{
  cv::vector<cv::Mat> oldPlane;
  split();
  cv::split(src, oldPlane);

  int v, temp;
  double kSharp;
  kSharp = 0.75;
  v =2;
  
  for (int channel = 0; channel <3; channel++)
    for (int y = v; y < (src.rows -v); y++)
      {
	uchar* topOldPtr = oldPlane[channel].ptr<uchar>(y-v);
	uchar* middleOldPtr = oldPlane[channel].ptr<uchar>(y);
	uchar* bottomOldPtr = oldPlane[channel].ptr<uchar>(y+v);
	uchar* newPtr = planes[channel].ptr<uchar>(y);
	
	for (int x = v; x < (src.cols -v); x++)
	  {
	    temp = (int)(((double)middleOldPtr[x] - ( (kSharp/4) * (double)(topOldPtr[x]+middleOldPtr[x-v]+middleOldPtr[x+v]+bottomOldPtr[x])))/(1.0-kSharp));
	    if(temp > 255)
	      newPtr[x] = 255;
	    else if(temp < 0)
	      newPtr[x] = 0;
	    else
	      newPtr[x] = temp;	    
	  };

      };

  merge();
  name = "Sharp";

};

void paolMat::shrink()
{
  paolMat outImg;
  src.copyTo(outImg.src);
  outImg.src = Scalar(0,0,0);
  outImg.split();
  split();
  int total;

   for (int channel = 0; channel <3; channel++)
    for (int y = 1; y < (src.rows -1); y++)
      {
	uchar* topOldPtr = planes[channel].ptr<uchar>(y-1);
	uchar* middleOldPtr = planes[channel].ptr<uchar>(y);
	uchar* bottomOldPtr = planes[channel].ptr<uchar>(y+1);
	uchar* newPtr = outImg.planes[channel].ptr<uchar>(y);
	
	for (int x = 1; x < (src.cols -1); x++)
	  {
	    if(middleOldPtr[x]>0)
	      {
		total = topOldPtr[x-1];
		total += topOldPtr[x];
		total += topOldPtr[x+1];
		
		total += middleOldPtr[x-1];
		total += middleOldPtr[x+1];
		
		total += bottomOldPtr[x-1];
		total += bottomOldPtr[x];
		total += bottomOldPtr[x+1];
	      
		if(total>=1530)
		  newPtr[x] = 255; 
	      };
	  };
	
      };
   outImg.merge();
   outImg.src.copyTo(src);
};

//////////Frame Linked List /////////////////////////////////////

frameListItem::frameListItem(paolMat* newFrame)
{
  //frame = new PaolMat;
  frame = newFrame;
};

frameListItem::~frameListItem()
{
  frame->src.release();
};

FrameLinkedList::FrameLinkedList()
{
  size = 0;
  producerRunning=true;
};

FrameLinkedList::~FrameLinkedList()
{
  //oldest->~frameListItem();
  //newest->~frameListItem();

};

void FrameLinkedList::push(paolMat* frame)
{
  boost::posix_time::seconds sleepTime(1);
  boost::mutex::scoped_lock lock(listLock);
  #ifndef _debug_
  std::cout<<"FrameLinkedList:: I have the lock"<<std::endl;
  #endif
  //Store image locally
  //paolMat newFrame;
  //newFrame = frame;
  
  //Create a pointer object to a frameListItem
  frameListItem* newItem;
  //Set the pointer to a new frameListItem (Making the new item above the scope of push
  newItem = new frameListItem(frame);
  #ifndef _debug_
  std::cout<<"FrameLinkedList:: New list item allocated"<<std::endl;
  #endif

  if (size == 0)
    {
#ifndef _debug_
      //std::cout<<"FrameLinkedList:: List size is zero, I must be the first!"<<std::endl;
#endif
      oldest = newItem;
      newest = newItem;
    }else if(size > 150)
    {
      boost::this_thread::sleep(sleepTime);
    }else
	{
      #ifndef _debug_
      //std::cout<<"FrameLinkedList:: I'm not the first, setting next to newItem"<<std::endl;
      #endif
      newest->next = newItem;
    };
  #ifndef _debug_
  ////std::cout<<"FrameLinkedList:: Done with house keeping, copying image memory"<<std::endl;
  #endif
  newest = newItem;
  ////std::cout<<"FrameLinkedList:: Newest is new item"<<std::endl;
  // newItem->frame = frame;
  #ifndef _debug_
  //std::cout<<"FrameLinkedList:: Read in new image, incrementing size"<<std::endl;
  #endif
  size++;
};


/////////////////////////////////////
//Method waits until pop conditions are met, and then pops and image to the consumer
// Pop conditions are:
//     If the producer is still running don't pop unless
//       there are more then two list elements in order to
//       preserve the linked list
//     If the producer is NOT running specificly pop the 
//       last two images destroying the list
//     Else sleep for sleepTime
////////////////////////////////////
paolMat* FrameLinkedList::pop()
{
  bool moreThanTwo = false;
  paolMat* toPop;
  toPop = new paolMat;
  int readSize;
  boost::posix_time::seconds sleepTime(1);
  //Enter loop for the first time no matter what
  //Loop where you grab a frame from the linked list, or wait for a frame to be available
  while(!moreThanTwo)
    {
      //Scoped so that the mutex will be released while the thread sleeps
      {
	boost::mutex::scoped_lock lock(listLock);
	readSize = size;
	#ifndef _debug_
	std::cout<<"FrameLinkedList:: About to check IF statments, producerRunning: "<<producerRunning<<std::endl;
	std::cout<<"FrameLinkedList:: readSize is: "<<readSize<<std::endl;
	#endif

      }
      boost::posix_time::seconds sleepTime(1);
      if (readSize > 2)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  moreThanTwo = true;
	  toPop = oldest->frame;
	  frameListItem* toDelete;
	  toDelete = oldest;
	  oldest = toDelete->next;
	  delete toDelete;
	  #ifndef _debug_
	  std::cout<<"LinkedList::More then two frames size is: "<<size<<std::endl;
	  #endif
	  size--;
	 
	  //std::cout<<"\n\n  LINKED LIST POP:: Count, seconds: "<<toPop.count<<" ,"<<toPop.time<<std::endl;
  
	  return toPop;
	}else if ( (readSize == 2) && !producerRunning)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  toPop = oldest->frame;
	  frameListItem* toDelete;
	  toDelete = oldest;
	  oldest = toDelete->next;
	  delete toDelete;
	  #ifndef _debug_
	  std::cout<<"LinkedList::two frames in list size is: "<<size<<std::endl;
	  #endif
	  size--;
	  return toPop;
	}else if ((readSize == 1) && !producerRunning)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  toPop = oldest->frame;
	  //frameListItem* toDelete;
	  delete oldest;
	  #ifndef _debug_
	  std::cout<<"LinkedList::One Frame in list, size is:: "<<size<<std::endl;
	  #endif
	  size--;
	  return toPop;
	}else if ((readSize == 0) && !producerRunning)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  #ifndef _debug_
	  std::cout<<"No Frames in list size is: "<<size<<std::endl;
	  #endif
	  paolMat* null;
	  null = new paolMat;
	  return null;
	  //this.~FrameLinkedList();
	}else
	{
	  #ifndef _debug_
	  std::cout<<"FrameLinkedList:: Waiting for more then 2 in list, sleeping, size is: "<<readSize<<std::endl;
	  std::cout<<"FrameLinkedList:: Released the lock"<<std::endl;
	  #endif
	  boost::this_thread::sleep(sleepTime);
	};
    };
  
};


////////////////////////////////////////////////////////////////////////////////////////////
//Buffer Class /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


Buffer::Buffer()
{
  boost::mutex::scoped_lock lock(bufferLock);
  consumerLists.clear();
  producerRunning=true;
  //std::cout<<"Buffer:: Buffer created"<<std::endl;
};

Buffer::~Buffer()
{
  for(int i = 0; i < consumerLists.size(); i++)
    delete consumerLists[i];
  //delete bufferLock;

};

int Buffer::registerConsumer()
{
  boost::mutex::scoped_lock lock(bufferLock);
  consumerLists.push_back(new FrameLinkedList);
  return consumerLists.size()-1;

};

void Buffer::push(paolMat* frame)
{
  //std::cout<<"1Buffer, recieved image to push, about to lock"<<std::endl;
  boost::mutex::scoped_lock lock(bufferLock);
  //std::cout<<"2Buffer, recieved image to push, about to lock"<<std::endl;
  for(unsigned int i=0; i<consumerLists.size(); i++)
    {
      //std::cout<<" 3Buffer, recieved image to push, pushing to list "<<i<<std::endl;
      consumerLists[i]->push(frame);
      //std::cout<<" 4Buffer, recieved image to push, pushing to list "<<i<<std::endl;
    };

};

void Buffer::push(paolMat frame)
{
  paolMat* toPass;
  toPass = new paolMat;
  toPass->copy(frame);
  push(toPass);
};

paolMat* Buffer::pop(int consumerID)
{
  paolMat* temp;
  //std::cout<<"BufferPop 1"<<std::endl;
  temp = consumerLists[consumerID]->pop();
  if (temp->src.data)
    return temp;
  else
    {
      temp->src=Scalar(0,0,0,0);
      consumerLists[consumerID]->stop();
      return temp;
    };
};


void Buffer::stop()
{
  producerRunning = false;
  boost::mutex::scoped_lock lock(bufferLock);
  for(unsigned int i=0; i<consumerLists.size(); i++)
    {
      consumerLists[i]->stop();
    };


}
