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
  //std::cout<<"PaolMat:: = operator called"<<std::endl;
  m.src.copyTo(src);
  //std::cout<<"PaolMat:: 1 image "<<(src.data)<<std::endl;
  //imwrite("media/copiedImage.png", src);
  //std::cout<<"PaolMat:: 1.2 image had data? "<<(!(!(m.src.data)))<<std::endl;
  //imwrite("media/copiedImage2.png", m.src);
  //std::cout<<"PaolMat:: 1.3 m image had data? "<<(!(!(m.src.data)))<<std::endl;
  count = m.count;
  //std::cout<<"PaolMat:: 2  count: "<<count<<"m.count "<<m.count<<std::endl;
  time = m.time;
  //std::cout<<"PaolMat:: 3  time: "<<time<<" m.time "<<m.time<<std::endl;
  name = m.name;
  //std::cout<<"PaolMat:: = operator complete name:"<<name<<std::endl;
};

void paolMat::read(std::string fileName,int countIn, int timeIn)
{
  src = imread(fileName);
  name = fileName;
  count=countIn;
  time=timeIn;

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
  int thresh;
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


//////////Frame Linked List /////////////////////////////////////

frameListItem::frameListItem(paolMat newFrame)
{
  frame.copy(newFrame);
};


FrameLinkedList::FrameLinkedList()
{
  size = 0;
  producerRunning=true;
};


void FrameLinkedList::push(paolMat frame)
{
  boost::mutex::scoped_lock lock(listLock);
  #ifndef _debug_
  //std::cout<<"FrameLinkedList:: I have the lock"<<std::endl;
  #endif
  //Store image locally
  //paolMat newFrame;
  //newFrame = frame;
  
  //Create a pointer object to a frameListItem
  frameListItem* newItem;
  //std::cout<<"FrameLinkedList:: frameListItem pointer created"<<std::endl;
  //Set the pointer to a new frameListItem (Making the new item above the scope of push
  newItem = new frameListItem(frame);
  //std::cout<<"\n\nFLL:: frame count, time "<<frame.count<<" "<<frame.time<<std::endl;
      
  //std::cout<<"FrameLinkedList:: frameListItem pointer set to new item"<<std::endl;
  #ifndef _debug_
  //std::cout<<"FrameLinkedList:: New list item allocated"<<std::endl;
  #endif

  if (size == 0)
    {
      #ifndef _debug_
      //std::cout<<"FrameLinkedList:: List size is zero, I must be the first!"<<std::endl;
      #endif
      oldest = newItem;
      newest = newItem;
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
paolMat FrameLinkedList::pop()
{
  bool moreThanTwo = false;
  paolMat toPop;
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
	//std::cout<<"FrameLinkedList:: About to check IF statments, producerRunning: "<<producerRunning<<std::endl;
	//std::cout<<"FrameLinkedList:: readSize is: "<<readSize<<std::endl;
	#endif

      }
      boost::posix_time::seconds sleepTime(1);
      if (readSize > 2)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  moreThanTwo = true;
	  toPop.copy(oldest->frame);
	  frameListItem* toDelete;
	  toDelete = oldest;
	  oldest = toDelete->next;
	  delete toDelete;
	  #ifndef _debug_
	  //std::cout<<"LinkedList::More then two frames size is: "<<size<<std::endl;
	  #endif
	  size--;
	 
	  //std::cout<<"\n\n  LINKED LIST POP:: Count, seconds: "<<toPop.count<<" ,"<<toPop.time<<std::endl;
  
	  return toPop;
	}else if ( (readSize == 2) && !producerRunning)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  toPop.copy(oldest->frame);
	  frameListItem* toDelete;
	  toDelete = oldest;
	  oldest = toDelete->next;
	  delete toDelete;
	  #ifndef _debug_
	  //std::cout<<"LinkedList::two frames in list size is: "<<size<<std::endl;
	  #endif
	  size--;
	  return toPop;
	}else if ((readSize == 1) && !producerRunning)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  toPop.copy(oldest->frame);
	  frameListItem* toDelete;
	  delete oldest;
	  #ifndef _debug_
	  //std::cout<<"LinkedList::One Frame in list, size is:: "<<size<<std::endl;
	  #endif
	  size--;
	  return toPop;
	}else if ((readSize == 0) && !producerRunning)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  #ifndef _debug_
	  //std::cout<<"No Frames in list size is: "<<size<<std::endl;
	  #endif
	  paolMat null;
	  return null;
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
  std::cout<<"Buffer:: Buffer created"<<std::endl;
};

int Buffer::registerConsumer()
{
  boost::mutex::scoped_lock lock(bufferLock);
  consumerLists.push_back(new FrameLinkedList);
  return consumerLists.size()-1;

};

void Buffer::push(paolMat frame)
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

paolMat Buffer::pop(int consumerID)
{
  paolMat temp;
  temp.copy(consumerLists[consumerID]->pop());
  //std::cout<<"\n\n  BUFFER POP:: Count, seconds: "<<temp.count<<" ,"<<temp.time<<std::endl;
  return temp;
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
