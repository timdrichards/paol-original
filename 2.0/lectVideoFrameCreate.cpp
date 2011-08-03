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
#include "lectVideoFrameCreate.h"

#define outWidth 640
#define outHeight 480
#define maxMoveSpeed 40
#define minMoveSpeed 2
#define _debug_

using namespace cv;



void LectVideoFrameCreate::run()
{
  Ptr<paolMat> previous;
  Ptr<paolMat> current;
  img = pop();
  while(img != NULL)
    {
      //Do stuff to it here....
      
      push(img);
      img = pop();
    };
  stop();
};

