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
  Ptr<paolMat> bgImg;
  Ptr<paolMat> oldCleanImg;
  Ptr<paolMat> oldOrigImg;
  Ptr<paolMat> oldTemp;
  Ptr<paolMat> blurred;
  Ptr<paolMat> lastImg;
  std::vector< Ptr<paolMat> > frames;
  img = pop();
  if(img != NULL){
    oldCleanImg = new paolMat(img);
    oldTemp = new paolMat(img);
    oldOrigImg = new paolMat(img);
    //oldCleanImg->src = Mat(img->src.size(), img->src.type(), Scalar(255,255,255));
    bgImg = oldCleanImg->returnCreateBackgroundImg(25);
    oldCleanImg->improveInputImg(bgImg);
    blurred = new paolMat(img);
    img->intensityMask(40);
    lastImg = new paolMat(img);
  };
  while(img != NULL)
    {
      std::cout<<"WhiteBoardProc:: frame "<<img->count<<" had "<<img->difs<<" differences "<<std::endl;
      //std::cout<<"WhiteBoardProcess::run loop head"<<std::endl;
      if(true) //this should be img->diffs > 20000, but debugging yo
	{
	  //img->average();
	  img->driftWAverage();
	  img->name = "driftAverage";
	  img->writeMask();
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
	    };
	  */
	};
      
      //for(int i = 0; i < skip; i++)
      std::cout<<"WhiteBoardProc: about to pop"<<std::endl;
      img = pop();
      std::cout<<"WhiteBoardProc: popped"<<std::endl;
    };
  stop();
};



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
