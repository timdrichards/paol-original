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


#define _debug_

using namespace cv;

PMList::PMList()
{
  listSize = 150;
  producerRunning = true;
  size = 0;
  current = 0;
  oldest = 0;
};

PMList::PMList(int size)
{
  listSize = size;
  producerRunning = true;
  size = 0;
  current = 0;
  oldest = 0;
};

PMList::~PMList()
{
  for(int i =0; i<listSize;i++)
    {
      if(PaolMats[i] != NULL)
	{
	  PaolMats[i]->~paolMat();
	  PaolMats[i] = NULL;
	};
    };
};

int PMList::push(paolMat* inPM)
{
  listLock.lock();
  std::cout<<"listLocked1"<<std::endl;
  if(size<listSize)
    {
    PaolMats[current] = inPM;
    current++;
    current%=listSize;
    size++;
    listLock.unlock();
    std::cout<<"listUNLocked1a"<<std::endl;
    }
  else
    {
      listLock.unlock();
      std::cout<<"listUNLocked1b"<<std::endl;
      boost::this_thread::sleep(boost::posix_time::seconds(1));
      push(inPM);
    };
  delete inPM;
  //listLock.unlock();
  return 0;
};

paolMat* PMList::pop()
{
  paolMat* toReturn;
  boost::mutex::scoped_lock lock(listLock);
  std::cout<<"listLocked2"<<std::endl;
  if(size>0)
    {
      
      toReturn = PaolMats[oldest];
      delete PaolMats[oldest];
      PaolMats[oldest] = NULL;
      oldest++;
      size--;
      oldest%=listSize;
      return toReturn;
    }
  else if(producerRunning)
    {
      lock.unlock();
      std::cout<<"listUNLocked2b"<<std::endl;
      boost::this_thread::sleep(boost::posix_time::seconds(1));
      return pop();
    };
  //delete toReturn;
  return NULL;  
};

void PMList::stop()
{
  boost::mutex::scoped_lock lock(listLock);
  producerRunning = false;
};

Buffer::Buffer()
{
  

};

Buffer::~Buffer()
{
  for(int i=0; i<(int)consumerLists.size(); i++)
    //delete consumerLists[i];
    consumerLists[i]->~PMList();
  //consumerLists.~vector();
};

int Buffer::push(paolMat* inPM)
{
  boost::mutex::scoped_lock lock(bufferLock);
  for(int i=0; i < (int)consumerLists.size(); i++)
    {
      paolMat* heap;
      heap = new paolMat(inPM);
      consumerLists[i]->push(heap);
    };
  delete inPM;
  return 0;
};

paolMat* Buffer::pop(int id)
{
  return consumerLists[id]->pop();
};

void Buffer::stop()
{
  boost::mutex::scoped_lock lock(bufferLock);
  for(int i = 0; i <(int)consumerLists.size(); i++)
    consumerLists[i]->stop();
};

int Buffer::registerConsumer()
{
  boost::mutex::scoped_lock lock(bufferLock);
  PMList* aList;
  aList = new PMList();
  consumerLists.push_back(aList);
  return consumerLists.size()-1;
};
