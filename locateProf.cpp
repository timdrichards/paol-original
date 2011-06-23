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
#include <cstdlib>
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

#define _debug_


locationData::locationData(paolMat in, double scale)
{
  
  //std::cout<<"LocationData Constructor 1"<<std::endl;
  big.copy(in);
  //std::cout<<"LocationData Constructor 2"<<std::endl;
  small.copy(in);
  
  //std::cout<<"LocationData Constructor 2"<<std::endl;
  if (big.src.data)
    {     
      //std::cout<<"LocationData Constructor 3"<<std::endl;
      small.src.release();
      cv::Mat temp;
      cv::resize(big.src, small.src, Size(), 1/scale, 1/scale, INTER_NEAREST);
      //Sobel(temp, small.src, 3, 1, 1, 3, 2, 0, BORDER_DEFAULT);
      small.edges();
      //std::cout<<"LocationData Constructor 4"<<std::endl;
      small.name = "Sobel";
      //small.print();
      small.split();
      //std::cout<<"LocationData Constructor 5"<<std::endl;
    };
  //std::cout<<"LocationData Constructor 6"<<std::endl;
};

void locationData::copy(locationData m)
{
  big.copy(m.big);
  small.copy(m.small);
};


void LocateProf::setup(int size, int scaleIn, int rBoxIn)
{
  frameBufferSize = size;
  //locationData emptyImg(pop(), scale);
  //frameBuffer.resize(frameBufferSize, emptyImg);
  paolMat* temp;
  temp = new paolMat;
  bool isData=true;
  rBox = rBoxIn;
  moveX = true;
  moveY = true;
  camera = Point(0,0);
  frameBuffer.push_back(locationData(*temp, scaleIn));
  delete temp;
  for( int i = 1; i<frameBufferSize && isData; i++)
    {
      temp = pop();
      frameBuffer.push_back(locationData(*temp, scaleIn));
      delete temp;
      isData=(frameBuffer[i].big.src.data);
      if(!isData){
	frameBuffer.pop_back();
	frameBufferSize=frameBuffer.size();
      }
    };

  current = 0;
  newest = frameBufferSize-1;
  scale = scaleIn;
};

bool LocateProf::newFrame()
{
  //std::cout<<"New frame 1"<<std::endl;
  locationData temp(*pop(), scale);
  //std::cout<<"New frame 2"<<std::endl;
  if (temp.big.src.data)
    {
      //std::cout<<"New frame 3"<<std::endl;
      frameBuffer[current].copy(temp);
      current++;
      newest++;
      current%=frameBufferSize;
      newest%=frameBufferSize;
      //std::cout<<"New frame 4"<<std::endl;
      return true;
    }else
    {
      //std::cout<<"New frame 5 (false)"<<std::endl;
      return false;
    };
  
  //std::cout<<"New frame 6 (done)"<<std::endl;
};

void LocateProf::createDifference()
{
  int totalC;//,totalN;
  //  paolMat temp;
  //temp.copy(frameBuffer[current].small);
  //temp.src = cv::Mat::zeros(frameBuffer[current].small.src.rows, frameBuffer[current].small.src.cols, CV_8U);
  difference.copy(frameBuffer[current].small);
  difference.split();
  //int count = 0;
  for(int y=0;y < difference.src.rows; y++)
    {
      //std::cout<<"1"<<std::endl;
      uchar* DrPtr = difference.planes[2].ptr<uchar>(y);      
      uchar* DgPtr = difference.planes[1].ptr<uchar>(y);      
      uchar* DbPtr = difference.planes[0].ptr<uchar>(y); 
      
      uchar* CrPtr = frameBuffer[current].small.planes[2].ptr<uchar>(y);      
      //uchar* CgPtr = frameBuffer[current].small.planes[0].ptr<uchar>(y);      
      //uchar* CbPtr = frameBuffer[current].small.planes[0].ptr<uchar>(y);      
      
      uchar* NrPtr = frameBuffer[newest].small.planes[2].ptr<uchar>(y);      
      //uchar* NgPtr = frameBuffer[newest].small.planes[0].ptr<uchar>(y);      
      //uchar* NbPtr = frameBuffer[newest].small.planes[0].ptr<uchar>(y);      
      for (int x = 0; x < difference.src.cols; x++)
	{
	  //count++;
	  //std::cout<<count<<std::endl;
	  //std::cout<<"2"<<std::endl;
	  //	  totalC = CrPtr[x]+CbPtr[x]+CgPtr[x];
	  //totalN = NrPtr[x]+NbPtr[x]+NgPtr[x];
	  
	  //if(totalC>100 && totalN>100){  
	  //std::cout<<"000"<<std::endl;
	  totalC = abs(CrPtr[x]-NrPtr[x]);
	  
	  DrPtr[x]= totalC;
	  DgPtr[x]= totalC;
	  DbPtr[x]= totalC;
	  
	  //} else {
	  
	  //std::cout<<"255"<<std::endl;
	  //DrPtr[x]=255;
	  //DgPtr[x]=255;
	  //DbPtr[x]=255;
	};
    };
  
  difference.merge();
  //  frameBuffer[current].small.merge();
  //  cv::absdiff(frameBuffer[current].small.src, frameBuffer[newest].small.src, difference.src);
  difference.name = "Difference";
  difference.shrink();
  //frameBuffer[current].small.name="dif";
  //frameBuffer[current].small.print();
  //difference.print();
  
};

void LocateProf::findProf()
{
  difference.split();
  //cv::rectangle roi;
  int top=difference.src.rows-1;
  int left=difference.src.cols-1;
  int right=0;
  int total;
  int count=0;
  char countString[256];
  
  for(int y=0;y < difference.src.rows; y++)
    {
      uchar* rPtr = difference.planes[0].ptr<uchar>(y);      
      uchar* gPtr = difference.planes[1].ptr<uchar>(y);      
      uchar* bPtr = difference.planes[2].ptr<uchar>(y);      
      for (int x = 0; x < difference.src.cols; x++)
	{
	  //	  rPtr[x]=255;
	  total = (rPtr[x]+bPtr[x]+gPtr[x])/3;
	  if(total>100)
	    {  
	      count++;
	      if(y<top)
		top=y;
	      if(x<left)
		left=x;
	      if(x>right)
		right=x;
	    };
	};
    };
  
  //rectangle(difference.src, Point(left, top), Point(right, difference.src.rows-1), Scalar(255,255,255, 255), 2,8, 0);
  //std::cout<<"returnPoints X: "<<(left+right)/2<<" top: "<<top<<std::endl;
  //std::cout<<"  returnPoints X: "<<((left+right)/2)*scale<<" top: "<<top*scale<<std::endl;
  sprintf(countString,"%d",count);
  cv::putText(frameBuffer[current].big.src,countString,Point(600,300), FONT_HERSHEY_PLAIN, 3, Scalar(0,255,0,255), 1, 8, false);
  if(((right - left)<100) && (count>200))
    {
      curLoc.x = ((left+right)/2)*scale;
      curLoc.y = (top+10)*scale;
    };
};

void LocateProf::fixLocation()
{
  if(moveX)
    {
      if(abs(camera.x - curLoc.x)<rBox/2)
	moveX = false;
      else
	camera.x += ((curLoc.x - camera.x) * 40) / 160 +2;
    }else
    {
      if(abs(camera.x - curLoc.x)>rBox)
	{
	  moveX = true;
	  camera.x += ((curLoc.x - camera.x) * 40) / 160 +2;
	};
    };
  if(moveY)
    {
      if(abs(camera.y - curLoc.y)<rBox/4)
	moveY = false;
      else
	camera.y += ((curLoc.y - camera.y) / abs(curLoc.y - camera.y)) * 2;
    }else
    {
      if(abs(camera.y - curLoc.y)>rBox)
	{
	  moveY = true;
	  camera.y += ((curLoc.y - camera.y) / abs(curLoc.y - camera.y)) * 2;
	};
    };
  frameBuffer[current].big.prof = curLoc;
  frameBuffer[current].big.camera = camera;
  //std::cout<<"Current Point X: "<<frameBuffer[current].big.camera.x<<" y: "<<frameBuffer[current].big.camera.y<<" Current: "<<current<<std::endl;
};

void LocateProf::run()
{
  setup(15,4,50);
  
  
  //int top;

  //  paolMat img;
  
  //img.copy(pop());

  while(newFrame())
    {
      //std::cout<<"0"<<std::endl;
      createDifference();
      findProf();
      
      //difference.print();
      //std::cout<<"1"<<std::endl;
      fixLocation();
      //std::cout<<"2"<<std::endl;
      std::cout<<"Frame Count: "<<frameBuffer[current].big.count<<std::endl;
      conBuffer->push(frameBuffer[current].big);
      //std::cout<<"3"<<std::endl;
    };
  //std::cout<<"4"<<std::endl;
  paolMat nullImage;
  conBuffer->push(nullImage);
  conBuffer->stop();
  
};



void Accumulate::run()
{
  paolMat sharp;
  setup(12);
  while(keepgoing)
    {
      insert();
      if(keepgoing){
	sharp.copy(outImg);
	sharp.createContrast();
	sharp.sharpen();
	//sharp.print();
	conBuffer->push(outImg);
	outImg.name = "accumulate";
	outImg.print();
	std::cout<<"Acc image count is: "<<outImg.count<<std::endl;
	//std::cout<<"Acc should have printed here"<<std::endl;
      }
    };
  conBuffer->stop();
};


void Accumulate::setup(int bufferSizeIn)
{
  //std::cout<<"Acc:: 0"<<std::endl;
  current = 1;
  keepgoing = true;
  bufferSize = bufferSizeIn;
  paolMat empty;
  //std::cout<<"Acc:: 1"<<std::endl;
  buffer.resize(bufferSize,empty);
  //std::cout<<"Acc:: 2"<<std::endl;
  buffer[0].copy( pop() );
  buffer[0].split();
  //std::cout<<"Acc:: 3"<<std::endl;
  outImg.copy(buffer[0]);
  //std::cout<<"Acc:: 4"<<std::endl;
  outImg.name = "accumulated";
  //std::cout<<"Acc:: 5"<<std::endl;
  
  
  
  /*  
  big.resize(buffer[0].src.rows);
  for(int i =0; i<buffer[0].src.rows; i++)
    {
      big[i].resize(buffer[0].src.cols);
      for(int j=0; j<3; j++)
	big[i][j].resize(3,0);
    };
  */
  //std::cout<<"Acc:: 6"<<std::endl;
  for(int i = 1; i<bufferSize ; i++)
    {
    buffer[i].copy(buffer[0]);
    buffer[i].split();
    //std::cout<<"Acc:: 7"<<std::endl;
    };
  //std::cout<<"Acc:: 8"<<std::endl;
  addPaol(buffer[0]);
  //std::cout<<"Acc:: 9"<<std::endl;
  /*
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
  */
  //std::cout<<"Acc:: 10"<<std::endl;
  outImg.copy(buffer[0]);
  //std::cout<<"Acc finnished setup"<<std::endl;
};

void Accumulate::addPaol(paolMat inImg)
{
  current++;
  
  if(current >= bufferSize)
    current = 0;

  //std::cout<<"0  AddPaol called"<<std::endl;
  
  buffer[current].copy(inImg);
  //std::cout<<"1  AddPaol called"<<std::endl;
  buffer[current].split();
  //std::cout<<"3  AddPaol called"<<std::endl;
  //  std::cout<<"1  AddPaol called"<<std::endl;
  /*
  for (int y = 0; y < buffer[current].src.rows; y++)
    {
      uchar* rPtr = buffer[current].planes[0].ptr<uchar>(y);
      uchar* gPtr = buffer[current].planes[1].ptr<uchar>(y);
      uchar* bPtr = buffer[current].planes[2].ptr<uchar>(y);
      std::cout<<"2  AddPaol called"<<std::endl;
      
      for (int x = 0; x < buffer[current].src.cols; x++)
	{
	  std::cout<<" Colors Red: "<<(int)rPtr[x]<<" Green: "<<(int)gPtr[x]<<" Blue: "<<(int)bPtr[x]<<std::endl;
	  std::cout<<" Big Red: "<<big[y][x][0]<<" Green: "<<big[y][x][1]<<" Blue: "<<big[y][x][2]<<std::endl;
	  big[y][x][0] += (int)rPtr[x];
	  big[y][x][1] += (int)gPtr[x];
	  big[y][x][2] += (int)bPtr[x];
	  std::cout<<"3  AddPaol called"<<std::endl;
	};
      
    };
  std::cout<<"4  AddPaol called"<<std::endl;
  */
  //std::cout<<"4  AddPaol called"<<std::endl;
  updateOutput();
  //std::cout<<"5  AddPaol called"<<std::endl;
  //  //std::cout<<"2  AddPaol called"<<std::endl;
  
  //  std::cout<<"3  AddPaol called"<<std::endl;
};

void Accumulate::subtractPaol()
{
  /*
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
  */
};

void Accumulate::updateOutput()
{
  int thresh=10;
  bool same;
  //std::cout<<"0  updateOutput called"<<std::endl;
  outImg.split();
  for (int p=0;p<3;p++)
    {
      //std::cout<<"1  updateOutput called"<<std::endl;
    for (int y = 0; y < outImg.src.rows; y++)
      {
	//std::cout<<"1.1  updateOutput called"<<std::endl;
	uchar* ptrOut = outImg.planes[p].ptr<uchar>(y);
	//std::cout<<"1.2  updateOutput called"<<std::endl;
	uchar* ptrCur = buffer[current].planes[p].ptr<uchar>(y);
	//std::cout<<"2  updateOutput called"<<std::endl;
	for (int x = 0; x < outImg.src.cols; x++)
	  {
	    //std::cout<<"2.1 "<<buffer[current].count<<" "<<y<<"/"<<outImg.src.rows<<" "<<x<<"/"<<outImg.src.cols<<" "<<p<<std::endl;
	    if(abs(ptrOut[x]-ptrCur[x])>thresh){
	      //std::cout<<"2.01  updateOutput called"<<std::endl;
	      if(ptrCur[x]>=245){
		//std::cout<<"2.02 updateOutput called"<<std::endl;
		ptrOut[x]=ptrCur[x];
	      }else {
		//std::cout<<"2.2 "<<buffer[current].count<<" "<<y<<"/"<<outImg.src.rows<<" "<<x<<"/"<<outImg.src.cols<<" "<<p<<std::endl;
		//temp=0;
		//std::cout<<"2.03 updateOutput called"<<std::endl;
		same=true;
		for (int z=0;z<bufferSize;z++){
		  //std::cout<<"2.04 updateOutput called"<<std::endl;
		  //std::cout<<"2.041 "<<buffer.size()<<" "<<buffer[z].planes.size()<<std::endl;
		  uchar* ptrTemp = buffer[z].planes[p].ptr<uchar>(y);
		  //temp+=ptrTemp[x];
		  
		  //temp/=bufferSize;
		  if(abs(ptrCur[x]-ptrTemp[x])>thresh)
		    //std::cout<<"2.05 updateOutput called"<<std::endl;
		    same=false;
		};
		//std::cout<<"2.3  updateOutput called"<<std::endl;
		if(same)
		  ptrOut[x]=ptrCur[x];
		//std::cout<<"2.4  updateOutput called"<<std::endl;
	      };
	    };
	  };
      };
    };
  //std::cout<<"3  updateOutput called"<<std::endl;
  outImg.merge();
  outImg.count = buffer[current].count;
  outImg.time = buffer[current].time;
};

void Accumulate::insert()
{
  subtractPaol();
  paolMat temp;
  temp.copy(pop());
  if(temp.src.data)
    addPaol(temp);
  else
    keepgoing = false;
};
