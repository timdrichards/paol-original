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

using namespace cv;
using namespace boost;


//////////Frame Linked List /////////////////////////////////////

FrameLinkedList::FrameLinkedList()
{
  size = 0;
};


void FrameLinkedList::push(cv::Mat frame)
{
  boost::mutex::scoped_lock lock(listLock);
  std::cout<<"FrameLinkedList:: I have the lock"<<std::endl;
  //Store image locally
  cv::Mat newFrame = frame;
  
  //Create a pointer object to a frameListItem
  frameListItem* newItem;
  //Set the pointer to a new frameListItem (Making the new item above the scope of push
  newItem = new frameListItem;

  std::cout<<"FrameLinkedList:: New list item allocated"<<std::endl;
  
  if (size == 0)
    {
      std::cout<<"FrameLinkedList:: List size is zero, I must be the first!"<<std::endl;
      oldest = newItem;
      newest = newItem;
    }else
    {
      std::cout<<"FrameLinkedList:: I'm not the first, setting next to newItem"<<std::endl;
      newest->next = newItem;
    };
  
  std::cout<<"FrameLinkedList:: Done with house keeping, copying image memory"<<std::endl;
  
  newest = newItem;
  newItem->frame = newFrame;
  std::cout<<"FrameLinkedList:: Read in new image, incrementing size"<<std::endl;
  size++;
};


cv::Mat FrameLinkedList::pop()
{
  bool moreThanTwo = false;
  cv::Mat toPop;
  while(!moreThanTwo)
    {
      int readSize;
      //Scoped so that the mutex will be released while the thread sleeps
      {
	boost::mutex::scoped_lock lock(listLock);
	readSize = size;
      }
      boost::posix_time::seconds sleepTime(2);
      if (readSize > 2)
	{
	  boost::mutex::scoped_lock lock(listLock);
	  moreThanTwo = true;
	  toPop = oldest->frame;
	  frameListItem* toDelete;
	  toDelete = oldest;
	  oldest = toDelete->next;
	  delete toDelete;
	  return toPop;
	}else
	{
	  std::cout<<"FrameLinkedList:: Waiting for more then 2 in list, sleeping, size is: "<<readSize<<std::endl;
	  std::cout<<"FrameLinkedList:: Released the lock"<<std::endl;
	  boost::this_thread::sleep(sleepTime);
	};
      
    };
  boost::mutex::scoped_lock lock(listLock);
  size--;
  return toPop;
};


////////////////////////////////////////////////////////////////////////////////////////////
//Buffer Class /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


Buffer::Buffer()
{
  boost::mutex::scoped_lock lock(bufferLock);
  consumerLists.clear();
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
