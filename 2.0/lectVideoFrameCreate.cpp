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
#define border 100
#define maxMoveSpeed 40
#define minMoveSpeed 5
#define _debug_

using namespace cv;



void LectVideoFrameCreate::run()
{
  Ptr<paolMat> previous;
  Ptr<paolMat> current;
  Ptr<paolMat> img;
  Ptr<paolMat> cropped;
  bool moveX, moveY;
  int diff;
  moveX = true;
  moveY = true;
  previous = pop();
  previous->camera = previous->prof;
  current = pop();
  cropped = previous->cropFrame(outWidth, outHeight);
  push(cropped);
  while(current != NULL)
    {
      current->camera = previous->camera;
      if(current->prof.x == -1 || current->prof.y == -1)
	{
	  current->prof = previous->prof;
	};
      if(abs(previous->camera.x - current->prof.x) < minMoveSpeed)
	{
	  moveX = false;
	};
      if(abs(previous->camera.y - current->prof.y) < minMoveSpeed)
	{
	  moveY = false;
	};

      diff = current->prof.x - previous->camera.x;
      if(abs(diff) > (outWidth/2 - border))
	{
	  current->camera.x += diff/2;
	  moveX = true;
	}else if(moveX)
	{
	  current->camera.x += (diff/diff) * minMoveSpeed;
	};

      diff = current->prof.y - previous->camera.y;
      if(abs(diff) > (outHeight/2 - border))
	{
	  current->camera.y += diff/2;
	  moveY = true;
	}else if(moveY)
	{
	  current->camera.y += (diff/diff) * minMoveSpeed;
	};
#ifdef _debug_
      std::cout<<"LectVideoFrameCreate prof x="<<current->prof.x<<" y="<<current->prof.y<<" camera x="<<current->camera.x<<" y="<<current->camera.y<<std::endl;      
#endif
      cropped = current->cropFrame(outWidth, outHeight);
      push(cropped);
      previous->copy(current);
      current = pop();
    };
  stop();
};

