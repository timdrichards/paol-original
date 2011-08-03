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
#include "module.h"
#include "computerDistribute.h"

#define _debug_
#define bottomMask 115
#define thresholdDiff .0005
#define repeat 3//number of consecutive stamble images necessary to consider stable

using namespace cv;

ComputerDistribute::ComputerDistribute(Buffer* in, Buffer* compProcIn, Buffer* compMovieIn, Buffer* tempDisplayIn) : Module(in,compProc,150)
{
  compProc = compProcIn;
  compMovie = compMovieIn;
  tempDisplay = tempDisplayIn;
};

void ComputerDistribute::run()
{
  int countStable;
  double percentDifference;
  Ptr<paolMat> current;
  Ptr<paolMat> previous;

  previous = pop();
  if(previous != NULL){
    current = pop();
  };
  countStable=0;
  
  while(current != NULL)
    {
      if(previous->src.rows==current->src.rows &&
	 previous->src.cols==current->src.cols)
	{
	  current->difference(previous, 100, 0, bottomMask); 
	  percentDifference=(double)current->difs/(double)(current->src.rows*current->src.cols);
	} else
	{
	  percentDifference=1.0;
	};
#ifdef _debug_
      //percentDifference = 0.5;
      std::cout<<"Dif% :"<<percentDifference<<" countStable: "<<countStable<<" current->difs: "<<current->difs<<std::endl;
#endif
      if(percentDifference>=thresholdDiff)
	{
	  if(countStable>repeat)//was stable going to unstable
	    {
#ifdef _debug_
	      previous->name = "ForCompProc";
#endif
	      compProc->push(previous);//send to computer process
	    } else
	    {//there was a change
#ifdef _debug_
	      current->name = "ForCompMovie";
#endif
	      compMovie->push(current);//send to movie creation
	    };
	  countStable=0;
	} else {
	countStable++;
	if(countStable==repeat)
	  {//if it is stable but just started stable
#ifdef _debug_
	    current->name = "ForTempDisplay";
#endif
	    tempDisplay->push(current);//send to real time display
	  }
      };
      previous->copy(current);
      current=pop();
    };
  //save last image
#ifdef _debug_
  previous->name = "ForCompProc";
#endif
  compProc->push(previous);
  {
    boost::mutex::scoped_lock lock(modLock);
    keepRunning = false;
  }
  //stop();
  compProc->stop();
  compMovie->stop();
  tempDisplay->stop();
};
