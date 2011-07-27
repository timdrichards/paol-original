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
  PaolMats.resize(150, NULL);
  producerRunning = true;
  size = 0;
  current = 0;
  oldest = 0;
};

PMList::~PMList()
{
  /*
  Ptr<paolMat> img;
  while(!PaolMats.empty())
    {
    
      if(PaolMats.back() != NULL)
	{
	  PaolMats.back()->print();
	  delete PaolMats.back();
	}
       PaolMats.pop_back();
      
    }
*/
};

int PMList::push(Ptr<paolMat> inPM)
{
  listLock.lock();
  if(size<(int)PaolMats.size())
    {
    PaolMats[current] = inPM;
    current++;
    current%=(int)PaolMats.size();
    size++;
    listLock.unlock();
    }
  else
    {
      listLock.unlock();
      boost::this_thread::sleep(boost::posix_time::seconds(1));
      push(inPM);
    };
  //listLock.unlock();
  return 0;
};

Ptr<paolMat> PMList::pop()
{
  //Ptr<paolMat> toReturn;
  boost::mutex::scoped_lock lock(listLock);
  if(size>0)
    {
      oldest++;
      size--;
      return PaolMats[(oldest-1)%PaolMats.size()];
    }
  else if(producerRunning)
    {
      lock.unlock();
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

void PMList::print()
{
  using namespace std;
  cout<<endl<<"  PMList Print::"<<endl;
  cout<<"  Current: "<<current<<" oldest: "<<oldest<<" size: "<<size<<endl;
  cout<<"  PaolMats.size(): "<<PaolMats.size()<<endl;
  for(int i = 0; i < (int)PaolMats.size(); i++)
    {
      if(PaolMats[i] != NULL)
	PaolMats[i]->print();
    };
  cout<<endl<<endl;
};

Buffer::Buffer()
{
  

};

Buffer::~Buffer()
{
  
  /*while (!consumerLists.empty())
    {
      //consumerLists[0].erase();
      delete[] consumerLists.back();
      consumerLists.pop_back();
      }*/
  //for(int i = 0; i < (int)consumerLists.size();i++)
  //  consumerLists[i]->~PMList();
  //consumerLists.~vector();
};

int Buffer::push(Ptr<paolMat> inPM)
{
  boost::mutex::scoped_lock lock(bufferLock);
  for(int i=0; i < (int)consumerLists.size(); i++)
    {
      Ptr<paolMat> heap;
      heap = new paolMat(inPM);
      consumerLists[i]->push(heap);
    };
  
  return 0;
};

Ptr<paolMat> Buffer::pop(int id)
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
  
  Ptr<PMList> aList;
  aList = new PMList;
  consumerLists.push_back(aList);
  return consumerLists.size()-1;

};

using namespace std;
void Buffer::print()
{
  cout<<endl<<"BUFFER PRINT::"<<endl;
  cout<<" consumerLists.size(): "<<consumerLists.size()<<endl;
  for(int i = 0; i < (int)consumerLists.size(); i++)
    {
      cout<<" Consumer List #"<<i<<endl;
      consumerLists[i]->print();
    };
  cout<<endl<<endl<<endl;


};
