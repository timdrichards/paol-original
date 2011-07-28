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
  Ptr<paolMat> lastImg;
  Ptr<paolMat> difference;
  img = pop();
  if(img != NULL){
    lastImg = new paolMat(img);
    bgImg = lastImg->returnCreateBackgroundImg(25);
    lastImg->improveInputImg(bgImg);
    lastImg->removeProf();
  };
  while(img != NULL)
    {
      bgImg = img->returnCreateBackgroundImg(25);
      img->improveInputImg(bgImg);
#ifdef _debug_
      bgImg->write();
      img->write();
#endif
      img->removeProf();
#ifdef _debug_
      img->write();
#endif
      difference=lastImg->returnDifference(img,50,5,0);
      lastImg->copy(img);
#ifdef _debug_
      difference->write();
#endif
      img->createContrast();
#ifdef _debug_
      img->write();
#endif
      img->sharpen();
#ifdef _debug_
      img->write();
#endif
      
      push(img);
      img = pop();
    };
  stop();
};
