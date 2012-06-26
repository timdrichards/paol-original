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
  Ptr<paolMat> oldImg;
  oldImg = pop();
  
  
  Ptr<paolMat> img;
  img = new paolMat();
  img->copy(oldImg);
  img->name = "newImg";
  
  while(img != NULL)
    {
      oldImg->copy(img);
      img = pop();
      img->difference(oldImg);
      img->name = "foot-diffs";
      img->writeMask();
      img->edges();
      oldImg->edges();
      img->name = "foot-edges";
      img->writeMask();
      img->maskDifference(oldImg);
      img->name = "foot-edgesDifferences";
      img->writeMask();
      img->name = "foot-final";
      //push(img);
    }
      
  stop();
}

