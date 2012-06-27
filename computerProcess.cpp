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
#include "computerProcess.h"

#define _debug_
#define bottomMask 115
#define thresholdDiff .0005
#define repeat 3//number of consecutive stamble images necessary to consider stable

using namespace cv;


void ComputerProcess::run()
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
      if(previous->src.rows==current->src.rows && previous->src.cols==current->src.cols)
	{
	  current->difference(previous, 100, 0, bottomMask); 
	  percentDifference=(double)current->difs/(double)(current->src.rows*current->src.cols);
	} else
	{
	  percentDifference=1;
	};
      
#ifdef _debug_
      std::cout<<"compProc PercentDifference: "<<percentDifference<<" threshold: "<<thresholdDiff<<std::endl;
#endif
      if(percentDifference>=thresholdDiff)
	{
	  previous->name = "slide";
	  push(previous);//send to write
	}; 
      previous->copy(current);
      current=pop();
    };
  //save last image
  if(previous != NULL)
    {
      previous->name = "slide";
      push(previous);
    }
  stop();
};
