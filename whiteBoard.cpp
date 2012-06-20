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
  img->write();
  
  Ptr<paolMat> alt;
  alt = new paolMat(img);
  alt->copy(img);

  img->pDrift();
  img->name = "pDrift";
  img->writeMask();

  img->grow(30,3);
  img->name = "grow";
  img->writeMask();
  
  //img->shrink(30,3);
  //img->name = "shrink";
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
	  
	  //img->shrink(30,3);
	  	  
	  old->name = "debug/old";
	  old->writeMask();
	  img->name = "debug/img";
	  img->writeMask();
	  img->threshedDifference(old);
	  img->name = "debug/threshedDifference";
	  img->writeMask();

	  
	  alt->decimateMask(254);
	  alt->decimateMask(254);
	  alt->writeMask();
	  alt->sweepDown();
	  alt->writeMask();
	  

	  background->copyNoSrc(img);
	  background->updateBackground(alt,img);
	  background->name = "background-ruff";
	  background->write();
	  background->writeMask();
	  background->cleanBackground(img);
	  background->name = "background-clean";
	  background->write();
	  old->copy(img);

	}
      
      std::cout<<"WhiteBoardProc: about to pop"<<std::endl;
      img = pop();
      std::cout<<"WhiteBoardProc: popped"<<std::endl;
    }
  stop();
}



/*

	  
	  //img->connected();
	  //img->lectArea();
	  std::cout<<"WhiteBoardProc:: about to remove prof"<<std::endl;
	  img->removeProf(oldOrigImg);
	  oldTemp->copy(img);
#ifdef _debug_
	  //img->decimateMask();
	  
	  //img->writeMask();
	  //img->write();
#endif
	  std::cout<<"WhiteBoardProc:: about to create backgroundImg"<<std::endl;
	  bgImg = img->returnCreateBackgroundImg(25);
	  std::cout<<"WhiteBoardProc:: about to improveInputImg"<<std::endl;
	  img->improveInputImg(bgImg);
#ifdef _debug_
	  //bgImg->write();
	  //img->write();
#endif
	  std::cout<<"WhiteBoardProc:: about to differenceLect(oldCleanImg, 50,10)"<<std::endl;
	   img->differenceLect(oldCleanImg, 50, 10);
*/


//img->intensityMask(20);
	  //img->name = "intensity";
	  //img->writeMask();
	  
	  //img->maskToWhite(20);
	  //img->name = "maskToWhite";
	  //img->writeMask();
	  
	  //blurred->copyNoSrc(img);
	  //GaussianBlur(img->src,blurred->src,Size(0,0),5,5,BORDER_DEFAULT);
	  
	  //blurred->name = "blurred";
	  //blurred->write();
	  //img->difference(blurred);
	  //img->name = "DiffOfBlur";
	  //img->writeMask();
	  //img->drift();
	  //img->name = "Drift";
	  //img->writeMask();
	  //img->sweepMask();
	  //img->name = "Sweep";
	  //img->writeMask();
	  //img->decimateMaskByHistogram(100,100);
	  //img->name = "HistoDecimated";
	  //img->writeMask();
	  /*
	    if(img->difs > 30)
	    {
	    std::cout<<"WhiteBoardProc:: Enough diffs to make a slide"<<std::endl;
	    oldOrigImg->copy(oldTemp);
	    oldCleanImg->copy(img);
	    img->createContrast();
	    #ifdef _debug_
	    //img->write();
	    #endif
	    img->sharpen();
	    #ifdef _debug_
	    //img->write();
	    #endif
	    img->name="WBSlide";
	    push(img);
	    std::cout<<"WhiteBoardProc:: Pushed wb slide"<<std::endl;
	    }
	  */
