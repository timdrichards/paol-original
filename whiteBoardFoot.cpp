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
#include "whiteBoardFoot.h"

#define _debug_

using namespace cv;


void WhiteBoardFoot::run()
{
  Ptr<paolMat> stable;
  stable = new paolMat();
  stable = pop();

  Ptr<paolMat> current;
  current = new paolMat(stable);

  stable->maskGrowRed(20);
  
  while(current != NULL)
    {   

      stable->countDiffsMasked(current);
      std::cout<<"Foot:: stable:"<<current->count<<" had "<<stable->difs<<" difs"<<std::endl;
      if(stable->difs > 2000)
	{
	  stable->name = "slide";
	  push(stable);
	  //stable->copy(current);
	  stable->finalWBUpdate(current);
	  stable->maskGrowRed(20);
	  stable->name = "FinalWBUpdategrown";
	  //stable->writeMask();
	}
      else
	{
	  stable->differenceDarken(current);
	}
      current = pop();
    }
      
  stop();
}

