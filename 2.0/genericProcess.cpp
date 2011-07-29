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
#include "genericProcess.h"

#define _debug_

using namespace cv;

void GenericProcess::run()
{
  Ptr<paolMat> img;
  img = pop();
  while(img != NULL)
    {
      //Do stuff to it here....
      
      push(img);
      img = pop();
    };
  stop();
};

void GenericProcess::speedtest1()
{
  Ptr<paolMat> img;
  img = pop();
  while(img != NULL)
    {
      //Do stuff to it here....
      img->split();
      for (int y = 0; y < img->src.rows; y++)
	{
	  uchar* inputPtr0 = img->planes[0].ptr<uchar>(y);
	  uchar* inputPtr1 = img->planes[1].ptr<uchar>(y);
	  uchar* inputPtr2 = img->planes[2].ptr<uchar>(y);
	  for (int x = 0; x < img->src.cols; x++)
	    {
	      inputPtr0[x] = (inputPtr0[x] + 10)%254;
	      inputPtr1[x] = (inputPtr1[x] + 10)%254;
	      inputPtr2[x] = (inputPtr2[x] + 10)%254;
	    };
	};
      
      img->merge();
      push(img);
      img = pop();
    };
  stop();
};

void GenericProcess::speedtest2()
{
  Ptr<paolMat> img;
  img = pop();
  while(img != NULL)
    {
      for (int y = 0; y < img->src.rows; y++)
	for (int x = 0; x < img->src.cols; x++)
	  {
	    img->src.at<Vec3b>(y,x)[0]=((int)img->src.at<Vec3b>(y,x)[0] + 10)%254;
	    img->src.at<Vec3b>(y,x)[0]=((int)img->src.at<Vec3b>(y,x)[1] + 10)%254;
	    img->src.at<Vec3b>(y,x)[0]=((int)img->src.at<Vec3b>(y,x)[2] + 10)%254;
	  };
      
      
      push(img);
      img = pop();
    };
  stop();
};
