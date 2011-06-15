// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/multi_array.hpp>
#include <cassert>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
//#include <algorithm>
#include <cstdio>
#include <cctype>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"

#include "paolMat.h"

#include "buffer.h"
#include "producer.h"
#include "consumer.h"
#include "process.h"
#include "whiteBoardProcess.h"
#include "locateProf.h"

using namespace cv;
using namespace std;

////#define _debug_


void LocateProf::run()
{
  paolMat inImg;
  paolMat outImg;
  inImg.copy(pop());
  cv::Mat edges(inImg.src.rows, inImg.src.cols, inImg.src.depth());
  cv::Mat prof(inImg.src.rows, inImg.src.cols, CV_BGR2GRAY);
  cv::vector<cv::Mat> edgePlane;
  cv::vector<cv::Mat> profPlane;
  int top;
  
  HOGDescriptor hog;
  hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

  while(inImg.src.data)
    {
      outImg.copy(inImg);
      output->push(inImg);
      
      // Dilate the image so there are less edges to detect
      outImg.name = "dilated";
      cv::dilate(inImg.src, outImg.src, cv::Mat(), cv::Point(-1,-1), 20, 1, cv::morphologyDefaultBorderValue());
      outImg.print();
      //Make the image black and white so canny can draw lines on it
      cv::cvtColor(outImg.src, edges, CV_BGR2GRAY);
      cv::Canny(edges, outImg.src, 0, 50, 3, false);
      outImg.name = "dilated_Canny";
      outImg.print();
      

      cv::split(outImg.src, edgePlane);
      cv::split(inImg.src, profPlane);
      //Find the profesor by looking for horizontal lines with nothing below them.
      int p;
      int channel = 0;
      int profEdge[inImg.src.cols];
      int bottom = (7*(inImg.src.cols/8));
      int roof = inImg.src.cols/8;
      
      for (int y = 0; y <inImg.src.rows; y++)
	{
	  uchar* edgePtr = edgePlane[channel].ptr<uchar>(y);
	  for(int x = 0; x < inImg.src.cols; x++)
	    {
	      if (edgePtr[x] > 0)
		{
		  profEdge[x] = y;
		  std::cout<<"Found edge at col: "<<y<<" row: "<<x<<std::endl;
		};
	    };

	};
      for (int y = 0; y <inImg.src.rows; y++)
	{
	  uchar* profPtr = profPlane[channel].ptr<uchar>(y);
	  for(int x = 0; x < inImg.src.cols; x++)
	    {
	      if (profEdge[x]>=y)
		{
		  profPtr[x] = 255;
		  //std::cout<<"bottom at: "<<y<<" at: "<<x<<std::endl;
		};
	    };

	};
      
      cv::merge(profPlane, prof);

      cv::imwrite("profLines.png", prof);

      prof.copyTo(outImg.src);
      outImg.name = "profCheck";
      outImg.print();

      inImg.copy(pop());
      
    };
  paolMat nullImage;
  output->push(nullImage);
  output->stop();
  
};

void test::run()
{

  paolMat background;
  paolMat working;
  
  
  working.copy(pop());

  while(working.src.data)
    {
      
      background.copy(working);
      
      background.createBackgroundImg(25);
      background.print();
  
      working.improveInputImg(background);
      working.print();
      
      working.createContrast();
      working.print();
  
      working.sharpen();
      working.print();
      
      output->push(working);
      working.copy(pop());
    };
  output->stop();  

  /*
  paolMat inImgNew;
  paolMat inImgMid;
  paolMat inImgOld;
  paolMat difImgOld;
  paolMat difImgNew;
  paolMat outImg;

  inImgOld.copy(pop());
  inImgMid.copy(pop());
  inImgNew.copy(pop());
  difImgOld.copy(inImgOld);
  difImgNew.copy(inImgOld);
  outImg.copy(inImgOld);
  output->push(inImgOld);

  cv::absdiff(inImgOld.src, inImgMid.src, difImgOld.src);
  
  while(inImgOld.src.data && inImgNew.src.data)
    {
      difImgNew.copy(inImgNew);
      cv::absdiff(inImgMid.src, inImgNew.src, difImgNew.src);
      cv::absdiff(difImgOld.src, difImgNew.src, outImg.src);
      
      difImgOld.invert();
      difImgOld.name = "dif";
      difImgOld.print();

      outImg.invert();
      outImg.name = "difDiff";
      outImg.print();
      
      output->push(inImgNew);
      inImgOld.copy(inImgMid);
      inImgMid.copy(inImgNew);
      inImgNew.copy(pop());

      difImgOld.copy(difImgNew);

      outImg.copy(inImgOld);
    };
  paolMat nullImg;
  output->push(nullImg);
  output->stop();
  */
};

/*

  
  while(inImg.src.data)
    {
    
      vector<Rect> found, found_filtered;
      double t = (double)getTickCount();
      // run the detector with default parameters. to get a higher hit-rate
      // (and more false alarms, respectively), decrease the hitThreshold and
      // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
      int can = inImg.src.channels();
      hog.detectMultiScale(inImg.src, found, 0, Size(8,8), Size(128,128), 1.05, 1);
      t = (double)getTickCount() - t;
      printf("tdetection time = %gms\n", t*1000./cv::getTickFrequency());
      cout<<"Found: "<<found.size()<<endl;
      size_t i, j;

      for( i = 0; i < found.size(); i++ )
	{
	  Rect r = found[i];
	  for( j = 0; j < found.size(); j++ )
	    if( j != i && (r & found[j]) == r)
	      break;
	  if( j == found.size() )
	    found_filtered.push_back(r);
	}

      for( i = 0; i < found_filtered.size(); i++ )
	{
	  Rect r = found_filtered[i];
	  // the HOG detector returns slightly larger rectangles than the real objects.
	  // so we slightly shrink the rectangles to get a nicer output.
	  r.x += cvRound(r.width*0.1);
	  r.width = cvRound(r.width*0.8);
	  r.y += cvRound(r.height*0.07);
	  r.height = cvRound(r.height*0.8);
	  rectangle(inImg.src, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
	}
      inImg.name = "HOG";
      inImg.print();
      output->push(inImg);  



 */

void Accumulate::run()
{
  setup(5);
  while(buffer[current].src.data)
    {
      insert();
      output->push(outImg);
      outImg.name = "accumulate";
      outImg.print();
    };
  output->stop();
};


void Accumulate::setup(int bufferSizeIn)
{
  current = 1;
  bufferSize = bufferSizeIn;
  buffer.resize(bufferSize);
  
  buffer[0].copy( pop() );
  outImg.copy(buffer[0]);
  outImg.name = "accumulated";
  
  
  for(int i = 1; i<bufferSize ; i++)
    addPaol(buffer[0]);
  
  
  big.resize(buffer[0].src.rows);
  for(int i =0; i<buffer[0].src.rows; i++)
    {
      big[i].resize(buffer[0].src.cols);
      for(int j=0; j<3; j++)
	big[i][j].resize(3);
    };

  outImg.split();
  for (int y = 0; y < outImg.src.rows; y++)
    {
      uchar* rPtr = outImg.planes[0].ptr<uchar>(y);
      uchar* gPtr = outImg.planes[1].ptr<uchar>(y);
      uchar* bPtr = outImg.planes[2].ptr<uchar>(y);
      
      for (int x = 0; x < outImg.src.cols; x++)
	{
	  rPtr[x] = big[y][x][0]/bufferSize;
	  gPtr[x] = big[y][x][1]/bufferSize;
	  bPtr[x] = big[y][x][2]/bufferSize;
	};
      
    };
  outImg.merge();
};

void Accumulate::addPaol(paolMat inImg)
{
  buffer[current].copy(inImg);
  buffer[current].split();
  for (int y = 0; y < buffer[current].src.rows; y++)
    {
      uchar* rPtr = buffer[current].planes[0].ptr<uchar>(y);
      uchar* gPtr = buffer[current].planes[1].ptr<uchar>(y);
      uchar* bPtr = buffer[current].planes[2].ptr<uchar>(y);
      
      for (int x = 0; x < buffer[current].src.cols; x++)
	{
	  big[y][x][0] += rPtr[x];
	  big[y][x][1] += gPtr[x];
	  big[y][x][2] += bPtr[x];
	};
      
    };
  
  updateOutput();
  current++;
  if(current >= bufferSize)
    current = 0;
};

void Accumulate::subtractPaol()
{

  for (int y = 0; y < buffer[current].src.rows; y++)
    {
      uchar* rPtr = buffer[current].planes[0].ptr<uchar>(y);
      uchar* gPtr = buffer[current].planes[1].ptr<uchar>(y);
      uchar* bPtr = buffer[current].planes[2].ptr<uchar>(y);
      
      for (int x = 0; x < buffer[current].src.cols; x++)
	{
	  big[y][x][0] -= rPtr[x];
	  big[y][x][1] -= gPtr[x];
	  big[y][x][2] -= bPtr[x];
	};
      
    };
};

void Accumulate::updateOutput()
{
  int thresh=5;
  bool same;
  for (int p=0;p<3;p++)
    for (int y = 0; y < outImg.src.rows; y++)
      {
	uchar* ptrOut = outImg.planes[p].ptr<uchar>(y);
	uchar* ptrCur = buffer[current].planes[p].ptr<uchar>(y);
	
	for (int x = 0; x < buffer[current].src.cols; x++)
	  {
	    if(abs(ptrOut[x]-ptrCur[x])>thresh){
	      //temp=0;
	      same=true;
	      for (int z=0;z<bufferSize;z++){
		uchar* ptrTemp = buffer[z].planes[p].ptr<uchar>(y);
		//temp+=ptrTemp[x];
		
		//temp/=bufferSize;
		if(abs(ptrCur[x]-ptrTemp[x])>thresh)
		  same=false;
	      };
	      if(same)
		ptrOut[x]=ptrCur[x];
	    };
	  };
      };
  outImg.merge();
  outImg.count = buffer[current].count;
  outImg.time = buffer[current].time;
};

void Accumulate::insert()
{
  subtractPaol();
  addPaol(pop());
};
