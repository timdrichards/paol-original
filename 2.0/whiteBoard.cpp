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


void WhiteBoardProcess::run()
{
  Ptr<paolMat> img;
  Ptr<paolMat> bgImg;
  Ptr<paolMat> oldCleanImg;
  Ptr<paolMat> oldOrigImg;
  Ptr<paolMat> oldTemp;
  std::vector< Ptr<paolMat> > frames;
  img = pop();
  if(img != NULL){
    oldCleanImg = new paolMat(img);
    oldTemp = new paolMat(img);
    oldOrigImg = new paolMat(img);
    //oldCleanImg->src = Mat(img->src.size(), img->src.type(), Scalar(255,255,255));
    bgImg = oldCleanImg->returnCreateBackgroundImg(25);
    oldCleanImg->improveInputImg(bgImg);
  };
  while(img != NULL)
    {
      if(img->prof.x != -1 && img->prof.y != -1)
	{
	  
	  //img->connected();
	  //img->lectArea();
	  img->removeProf(oldOrigImg);
	  oldTemp->copy(img);
#ifdef _debug_
	  //img->decimateMask();
	  
	  //img->writeMask();
	  //img->write();
#endif
	  bgImg = img->returnCreateBackgroundImg(25);
	  img->improveInputImg(bgImg);
#ifdef _debug_
	  //bgImg->write();
	  //img->write();
#endif
	  img->difference(oldCleanImg, 50, 0, 0);
	  if(img->difs > 30)
	    {
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
	      
	      push(img);
	    };
	};
      img = pop();
    };
  stop();
};
