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
      if(previous.rows==current.rows && previous.cols=current.cols)
	{
	  current->difference(previous, 100, 0, bottomMask); 
	  percentDifference=current->difs/(current.rows*current.cols);
	} else
	{
	  percentDifference=1;
	};
   
      if(percentDif>=thresholdDiff)
	{
	  if(countStable>repeat)//was stable going to unstable
	    {
	      compProc->push(previous);//send to computer process
	    } else
	    {//there was a change
	      compMovie->push(current);//send to movie creation
	    };
	  countStable=0;
	} else {
	countStable++;
	if(countStable==repeat)
	  {//if it is stable but just started stable
	    tempDisplay->push(current);//send to real time display
	  }
      };
      previous=current;
      current=pop();
    };
  //save last image
  compProc->push(previous);
  stop();
};
