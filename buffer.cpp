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


//////////Frame Linked List /////////////////////////////////////

FrameLinkedList::FrameLinkedList()
{
  size = 0;
  producerRunning=true;
};


void FrameLinkedList::push(cv::Mat frame)
{
  boost::mutex::scoped_lock lock(listLock);
  #ifndef _debug_
  std::cout<<"FrameLinkedList:: I have the lock"<<std::endl;
  #endif
  //Store image locally
  cv::Mat newFrame = frame;
  
  //Create a pointer object to a frameListItem
  frameListItem* newItem;
  //Set the pointer to a new frameListItem (Making the new item above the scope of push
  newItem = new frameListItem;
  
  #ifndef _debug_
  std::cout<<"FrameLinkedList:: New list item allocated"<<std::endl;
  #endif

  if (size == 0)
    {
      #ifndef _debug_
      std::cout<<"FrameLinkedList:: List size is zero, I must be the first!"<<std::endl;
      #endif
      oldest = newItem;
      newest = newItem;
    }else
    {
      #ifndef _debug_
      std::cout<<"FrameLinkedList:: I'm not the first, setting next to newItem"<<std::endl;
      #endif
      newest->next = newItem;
    };
  #ifndef _debug_
  std::cout<<"FrameLinkedList:: Done with house keeping, copying image memory"<<std::endl;
  #endif
  newest = newItem;
  newItem->frame = newFrame;
  #ifndef _debug_
  std::cout<<"FrameLinkedList:: Read in new image, incrementing size"<<std::endl;
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
cv::Mat FrameLinkedList::pop()
{
  bool moreThanTwo = false;
  cv::Mat toPop;
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
	  frameListItem* toDelete;
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
	  cv::Mat null;
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

void Buffer::push(cv::Mat frame)
{
  boost::mutex::scoped_lock lock(bufferLock);
  for(unsigned int i=0; i<consumerLists.size(); i++)
    {
      consumerLists[i]->push(frame);
    };

};

cv::Mat Buffer::pop(int consumerID)
{
  return consumerLists[consumerID]->pop();
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
