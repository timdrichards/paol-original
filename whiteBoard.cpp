// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
#include <cstdio>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "paolMat.h"
#include "buffer.h"
#include "module.h"
#include "whiteBoard.h"

#define _debug_

using namespace cv;


void WhiteBoardProcess::run(int skip)
{
  Ptr<paolMat> img;
  img = pop();
  
  Ptr<paolMat> background;
  background = new paolMat(img);
  background->name = "background";
  background->src = Scalar(255,255,255);

  img->blur(1);
  img->name = "blurred";
  //img->write();
  
  Ptr<paolMat> alt;
  alt = new paolMat(img);
  alt->copy(img);

  img->pDrift();
  img->name = "pDrift";
  //img->writeMask();

  img->grow(30,3);
  img->name = "grow";
  //img->writeMask();
  
  Ptr<paolMat> old;
  old = new paolMat(img);
  old->name = "old";
  while(img != NULL)
    {
      std::cout<<"WhiteBoardProc:: frame "<<img->count<<" had "<<img->difs<<" differences "<<std::endl;
      if(true) //this should be img->diffs > 20000, but debugging yo
	{

	  alt->copy(img);
	  alt->differenceLect(old,150,1);

	  	  
	  img->blur(1);
	  
	  img->pDrift();
	  
	  img->grow(30,3);
	  
	  old->name = "debug/old";
	  //old->writeMask();
	  img->name = "debug/img";
	  //img->writeMask();
	  img->threshedDifference(old);
	  img->name = "debug/threshedDifference";
	  //img->writeMask();

	  
	  alt->decimateMask(254);
	  alt->decimateMask(254);
	  //alt->writeMask();
	  alt->sweepDown();
	  //alt->writeMask();
	  

	  background->copyNoSrc(img);
	  background->updateBackground(alt,img);
	  background->name = "background-ruff";
	  //background->write();
	  //background->writeMask();
	  background->cleanBackground(img);
	  background->name = "background-clean";
	  //background->write();
	  old->copy(img);
	  push(background);

	}
      
      std::cout<<"WhiteBoardProc: about to pop"<<std::endl;
      img = pop();
      std::cout<<"WhiteBoardProc: popped"<<std::endl;
    }
  stop();
}
