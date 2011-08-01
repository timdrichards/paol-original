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
#include "ac.h"

#define _debug_

using namespace cv;

void AC::run()
{
  Ptr<paolMat> img1,img2,img3,img4,img5,img6,img7,img8,img9,img10;
  Ptr<paolMat> stable;
  int count;
  int thresh = 100;

  img1 = pop();
  img2 = pop();
  img3 = pop();
  img4 = pop();
  img5 = pop();
  img6 = pop();
  img7 = pop();
  img8 = pop();
  img9 = pop();
  img10 = pop();
  
  
  
  stable = new paolMat(img1);
  stable->src = Mat(img1->src.size(), img1->src.type(), Scalar(255,255,255));

  while(img10 != NULL)
    {
      for(int y = 0; y< img1->src.rows-1; y++)
	{
	  for(int x = 0; x< img1->src.cols-1; x++)
	    {
	      count = 0;
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img1->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img2->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img3->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img4->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img5->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img6->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img7->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img8->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img9->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      for(int chan = 0; chan < 3; chan ++)
		{
		  if(abs(img10->src.at<Vec3b>(y,x)[chan] - stable->src.at<Vec3b>(y,x)[chan]) > thresh)
		    count ++;
		};
	      
	      if(count > 22)
		{
		  for(int chan = 0; chan < 3; chan ++)
		    {
		      stable->src.at<Vec3b>(y,x)[chan] = img5->src.at<Vec3b>(y,x)[chan];
		    };
		};
	    };
	};
      
      stable->copyNoSrc(img1);
      img1->copy(img2);
      img2->copy(img3);
      img3->copy(img4);
      img4->copy(img5);
      img5->copy(img6);
      img6->copy(img7);
      img7->copy(img8);
      img8->copy(img9);
      img9->copy(img10);
      img10 = pop();
            
      push(stable);
      stable->name="AC";
      stable->write();
    };
  stop();
};

