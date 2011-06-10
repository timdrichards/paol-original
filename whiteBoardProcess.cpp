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

#include "buffer.h"
#include "producer.h"
#include "consumer.h"
#include "process.h"
#include "whiteBoardProcess.h"

using namespace cv;
using namespace boost;

//#define _debug_

void WhiteBoardProcess::run()
{
  inputImg = pop();
  backgroundImg = inputImg;
  while(inputImg.data)
    {
#ifndef _debug_
      std::cout<<"Processor:: loop head"<<std::endl;
#endif
      createBackgroundImg(25);
#ifndef _debug_
      std::cout<<"Processor:: Background done"<<std::endl;
#endif
      createImprovedInputImg();
#ifndef _debug_
      std::cout<<"Processor:: Created Improved Input Img"<<std::endl;
#endif
      removeProf();
#ifndef _debug_
      std::cout<<"Processor:: Removed Prof"<<std::endl;
#endif
      //Remove from stream one we detirmine if we are keeping the image or not
      createContrastImprovedInputImg();
#ifndef _debug_
      std::cout<<"Processor:: Created contrast img"<<std::endl;
#endif
      output->push(improvedInputImgNoProfContrast);
#ifndef _debug_
      std::cout<<"Processor:: Pushed improved img"<<std::endl;
#endif

      sharpenContrastImprovedInputImg();
      output->push(improvedInputImgNoProfContrastSharp);
#ifndef _debug_
      std::cout<<"Processor:: pushed contrast improved img"<<std::endl;
#endif

      inputImg = pop();
    };
#ifndef _debug_
  std::cout<<"Processor:: Loop Ended"<<std::endl;
#endif
  cv::Mat nullImage;
  output->push(nullImage);
  output->stop();
#ifndef _debug_
  std::cout<<"Processor:: Null image passed"<<std::endl;
#endif
};

void WhiteBoardProcess::createBackgroundImg(int kernalSize)
{
  cv::Point centerPoint(-1,-1);
  cv::blur(inputImg, backgroundImg, cv::Size(kernalSize,kernalSize), centerPoint, 1);

};

void WhiteBoardProcess::createImprovedInputImg()
{
  cv::split(inputImg, inputPlanes);
  cv::split(backgroundImg, backgroundPlanes);
  improvedInputImg = inputImg;
  cv::split(improvedInputImg, improvedPlanes);
  int temp;
  for (int channel = 0; channel <3; channel++)
    for (int y = 0; y < inputImg.rows; y++)
      {
	uchar* inputPtr = inputPlanes[channel].ptr<uchar>(y);
	uchar* backgroundPtr = backgroundPlanes[channel].ptr<uchar>(y);
	uchar* improvedPtr = improvedPlanes[channel].ptr<uchar>(y);
	for (int x = 0; x < inputImg.cols; x++)
	  {
	    
	    temp  = (inputPtr[x] * 255) / backgroundPtr[x];
	    if(temp > 255)
	      improvedPtr[x] = 255;
	    else
	      improvedPtr[x] = temp;
	  };

      };
  
  cv::merge(improvedPlanes, improvedInputImg);
};

void WhiteBoardProcess::removeProf()
{

  improvedInputImgNoProf = improvedInputImg;
  improvedPlanesNoProf = improvedPlanes;

};

void WhiteBoardProcess::createContrastImprovedInputImg()
{
  improvedPlanesNoProfContrast = improvedPlanesNoProf;
  int thresh;
  int temp;
  int ave;
  for (int y = 0; y < inputImg.rows; y++)
      {
	uchar* RInPtr = improvedPlanesNoProf[0].ptr<uchar>(y);
	uchar* RContrastPtr = improvedPlanesNoProfContrast[0].ptr<uchar>(y);
	
	uchar* BInPtr = improvedPlanesNoProf[1].ptr<uchar>(y);
	uchar* BContrastPtr = improvedPlanesNoProfContrast[1].ptr<uchar>(y);

	uchar* GInPtr = improvedPlanesNoProf[2].ptr<uchar>(y);
	uchar* GContrastPtr = improvedPlanesNoProfContrast[2].ptr<uchar>(y);
	
		
	for (int x = 0; x < inputImg.cols; x++)
	  {
	    ave = (RInPtr[x] + BInPtr[x] + GInPtr[x]) /3;
	    if(ave <240 || ((RInPtr[x] < 220) || (BInPtr[x] < 220) || (GInPtr[x] < 220)))
	      {
		temp = RInPtr[x]-(255-RInPtr[x]);
		if (temp < 0)
		  temp = 0;
		//else if (temp > 188)
		//  temp = 255;
		RContrastPtr[x] = temp;

		temp = BInPtr[x]-(255-BInPtr[x]);
		if (temp < 0)
		  temp = 0;
		//else if (temp > 188)
		//  temp = 255;
		BContrastPtr[x] = temp;

		temp = GInPtr[x]-(255-GInPtr[x]);
		if (temp < 0)
		  temp = 0;
		//else if (temp > 188)
		//  temp = 255;
		GContrastPtr[x] = temp;
	       
	      }else
	      {
		RContrastPtr[x] = 255;
		GContrastPtr[x] = 255;
		BContrastPtr[x] = 255;


	      };
	    
	  };

      };
  cv::merge(improvedPlanesNoProfContrast, improvedInputImgNoProfContrast);
};


void WhiteBoardProcess::sharpenContrastImprovedInputImg()
{
  
  improvedInputImgNoProfContrast.copyTo(improvedInputImgNoProfContrastSharp);
  split(improvedInputImgNoProfContrastSharp, improvedPlanesNoProfContrastSharp);

  cv::vector<cv::Mat> oldPlane;
  cv::vector<cv::Mat> newPlane;
  cv::Mat img;
  int v, temp;
  double kSharp;
  kSharp = 0.75;
  v =2;
  img = improvedInputImgNoProfContrast;

  oldPlane = improvedPlanesNoProfContrast;
  newPlane = improvedPlanesNoProfContrastSharp;

  for (int channel = 0; channel <3; channel++)
    for (int y = v; y < (img.rows -v); y++)
      {
	uchar* topOldPtr = oldPlane[channel].ptr<uchar>(y-v);
	uchar* middleOldPtr = oldPlane[channel].ptr<uchar>(y);
	uchar* bottomOldPtr = oldPlane[channel].ptr<uchar>(y+v);
	uchar* newPtr = newPlane[channel].ptr<uchar>(y);
	
	for (int x = v; x < (inputImg.cols -v); x++)
	  {
	    temp = (int)(((double)middleOldPtr[x] - ( (kSharp/4) * (double)(topOldPtr[x]+middleOldPtr[x-v]+middleOldPtr[x+v]+bottomOldPtr[x])))/(1.0-kSharp));
	    if(temp > 255)
	      newPtr[x] = 255;
	    else if(temp < 0)
	      newPtr[x] = 0;
	    else
	      newPtr[x] = temp;	    
	  };

      };


  merge(newPlane, improvedInputImgNoProfContrastSharp);

};
