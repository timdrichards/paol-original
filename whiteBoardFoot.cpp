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
  Ptr<paolMat> temp;
  temp = new paolMat();
  temp = pop();
  

  Ptr<paolMat> oldImg;
  oldImg = new paolMat;
  oldImg = temp->crop(400,1400,3600,1356);
  
  Ptr<paolMat> img;
  img = new paolMat();
  img->copy(oldImg);
  img->name = "newImg";
  
  while(img != NULL)
    {      
      oldImg->differenceDarken(img);
      
      
      //if we are making a slide
      if(true)
	{
	  img->copyNoSrc(oldImg);
	  oldImg->name = "foot-differenceDarken";
	  oldImg->write();
	}
      temp = pop();
      img = temp->crop(400,1400,3600,1356);
    }
      
  stop();
}

