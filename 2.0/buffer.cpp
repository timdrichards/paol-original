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
  PaolMats.resize(150);
  producerRunning = true;
  size = 0;
  current = 0;
  oldest = 0;
};

PMList::~PMList()
{
  
};

int PMList::push(paolMat* inPM)
{
  listLock.lock();
  std::cout<<"listLocked1"<<std::endl;
  if(size<PaolMats.size())
    {
    PaolMats[current] = inPM;
    current++;
    current%=PaolMats.size();
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
      oldest++;
      size--;
      return PaolMats[(oldest-1)%PaolMats.size()];
    }
  else if(producerRunning)
    {
      lock.unlock();
      std::cout<<"listUNLocked2b"<<std::endl;
      boost::this_thread::sleep(boost::posix_time::seconds(1));
      return pop();
    };

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

};

int Buffer::push(paolMat* inPM)
{
  boost::mutex::scoped_lock lock(bufferLock);
  for(int i=0; i < consumerLists.size(); i++)
    {
      paolMat* heap;
      heap = new paolMat(inPM);
      consumerLists[i]->push(heap);
    };
  delete inPM;
};

paolMat* Buffer::pop(int id)
{
  return consumerLists[id]->pop();
};

void Buffer::stop()
{
  boost::mutex::scoped_lock lock(bufferLock);
  for(int i = 0; i <consumerLists.size(); i++)
    consumerLists[i]->stop();
};

int Buffer::registerConsumer()
{
  boost::mutex::scoped_lock lock(bufferLock);
  PMList* aList;
  aList = new PMList;
  consumerLists.push_back(aList);
  return consumerLists.size()-1;
};