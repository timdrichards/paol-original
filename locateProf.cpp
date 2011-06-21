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


locationData::locationData(paolMat in, double scale)
{
  big.copy(in);
  small.copy(in);
  if (big.src.data)
    {     
      small.src.release();
      cv::Mat temp;
      cv::resize(big.src, small.src, Size(), 1/scale, 1/scale, INTER_NEAREST);
      //Sobel(temp, small.src, 3, 1, 1, 3, 2, 0, BORDER_DEFAULT);
      small.edges();
      
      small.name = "Sobel";
      small.print();
      small.split();
    };
};


void LocateProf::setup(int size, int scaleIn)
{
  frameBufferSize = size;
  //locationData emptyImg(pop(), scale);
  //frameBuffer.resize(frameBufferSize, emptyImg);
  paolMat temp;
  bool isData=true;
  frameBuffer.push_back(locationData(temp, scaleIn));
  
  for( int i = 1; i<frameBufferSize && isData; i++)
    {
      frameBuffer.push_back(locationData(pop(), scaleIn));
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
  locationData temp(pop(), scale);
  
  if (temp.big.src.data)
    {
      frameBuffer[current] = temp;
      current++;
      newest++;
      current%=frameBufferSize;
      newest%=frameBufferSize;
      return true;
    }else
    {
      return false;
    };
};

void LocateProf::createDifference()
{
  int totalC,totalN;
  //  paolMat temp;
  //temp.copy(frameBuffer[current].small);
  //temp.src = cv::Mat::zeros(frameBuffer[current].small.src.rows, frameBuffer[current].small.src.cols, CV_8U);
  difference.copy(frameBuffer[current].small);
  difference.split();
  int count = 0;
  for(int y=0;y < difference.src.rows; y++)
    {
      //std::cout<<"1"<<std::endl;
      uchar* DrPtr = difference.planes[2].ptr<uchar>(y);      
      uchar* DgPtr = difference.planes[1].ptr<uchar>(y);      
      uchar* DbPtr = difference.planes[0].ptr<uchar>(y); 
      
      uchar* CrPtr = frameBuffer[current].small.planes[1].ptr<uchar>(y);      
      uchar* CgPtr = frameBuffer[current].small.planes[0].ptr<uchar>(y);      
      uchar* CbPtr = frameBuffer[current].small.planes[0].ptr<uchar>(y);      
      
      uchar* NrPtr = frameBuffer[newest].small.planes[1].ptr<uchar>(y);      
      uchar* NgPtr = frameBuffer[newest].small.planes[0].ptr<uchar>(y);      
      uchar* NbPtr = frameBuffer[newest].small.planes[0].ptr<uchar>(y);      
      for (int x = 0; x < difference.src.cols; x++)
	{
	  //count++;
	  //std::cout<<count<<std::endl;
	  //std::cout<<"2"<<std::endl;
	  //	  totalC = CrPtr[x]+CbPtr[x]+CgPtr[x];
	  //totalN = NrPtr[x]+NbPtr[x]+NgPtr[x];
	  
	  //if(totalC>100 && totalN>100){  
	  //std::cout<<"000"<<std::endl;
	  
	  
	  DrPtr[x]=abs(CrPtr[x]-NrPtr[x]);
	  DgPtr[x]=abs(CrPtr[x]-NrPtr[x]);
	  DbPtr[x]=abs(CrPtr[x]-NrPtr[x]);
	  
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
	      if(y<top)
		top=y;
	      if(x<left)
		left=x;
	      if(x>right)
		right=x;
	    };
	};
    };
  
  rectangle(difference.src, Point(left, top), Point(right, difference.src.rows-1), Scalar(255,255,255, 255), 2,8, 0);
  std::cout<<"Prof found at: Top: "<<top<<" Left: "<<left<<" Right: "<<right<<std::endl;
  //difference.merge();
};

void LocateProf::run()
{
  setup(15,4);
  
  //int top;

  //  paolMat img;
  
  //img.copy(pop());

  while(newFrame())
    {
      createDifference();
      //findProf();
      difference.print();
      //img.edges();
      //img.name = "edges";
      //img.print();
     
      //img.copy(pop());
    };
  paolMat nullImage;
  conBuffer->push(nullImage);
  conBuffer->stop();
  
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
      
      //      working.createContrast();
      //      working.print();
  
      //      working.sharpen();
      //      working.print();
      
      conBuffer->push(working);
      working.copy(pop());
    };
  conBuffer->stop();  

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
  conBuffer->push(inImgOld);

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
      
      conBuffer->push(inImgNew);
      inImgOld.copy(inImgMid);
      inImgMid.copy(inImgNew);
      inImgNew.copy(pop());

      difImgOld.copy(difImgNew);

      outImg.copy(inImgOld);
    };
  paolMat nullImg;
  conBuffer->push(nullImg);
  conBuffer->stop();
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
      conBuffer->push(inImg);  



 */

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
	sharp.print();
	conBuffer->push(outImg);
	outImg.name = "accumulate";
	outImg.print();
      }
    };
  conBuffer->stop();
};


void Accumulate::setup(int bufferSizeIn)
{
  current = 1;
  keepgoing = true;
  bufferSize = bufferSizeIn;
  paolMat empty;
  buffer.resize(bufferSize,empty);
  
  buffer[0].copy( pop() );
  outImg.copy(buffer[0]);
  outImg.name = "accumulated";
  
  
  
  
  /*  
  big.resize(buffer[0].src.rows);
  for(int i =0; i<buffer[0].src.rows; i++)
    {
      big[i].resize(buffer[0].src.cols);
      for(int j=0; j<3; j++)
	big[i][j].resize(3,0);
    };
  */
  for(int i = 1; i<bufferSize ; i++)
    buffer[i].copy(buffer[0]);
  addPaol(buffer[0]);
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
  outImg.copy(buffer[0]);
};

void Accumulate::addPaol(paolMat inImg)
{
  current++;
  
  if(current >= bufferSize)
    current = 0;

  //  std::cout<<"0  AddPaol called"<<std::endl;
  buffer[current].copy(inImg);
  buffer[current].split();
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
  updateOutput();
  //  std::cout<<"2  AddPaol called"<<std::endl;
  
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
  //  std::cout<<"0  updateOutput called"<<std::endl;
  for (int p=0;p<3;p++)
    for (int y = 0; y < outImg.src.rows; y++)
      {
	uchar* ptrOut = outImg.planes[p].ptr<uchar>(y);
	uchar* ptrCur = buffer[current].planes[p].ptr<uchar>(y);
	//std::cout<<"2  updateOutput called"<<std::endl;
	for (int x = 0; x < outImg.src.cols; x++)
	  {
	    //std::cout<<"2.1 "<<buffer[current].count<<" "<<y<<"/"<<outImg.src.rows<<" "<<x<<"/"<<outImg.src.cols<<" "<<p<<std::endl;
	    if(abs(ptrOut[x]-ptrCur[x])>thresh){
	      if(ptrCur[x]>=245){
		ptrOut[x]=ptrCur[x];
	      }else {
		//std::cout<<"2.2 "<<buffer[current].count<<" "<<y<<"/"<<outImg.src.rows<<" "<<x<<"/"<<outImg.src.cols<<" "<<p<<std::endl;
		//temp=0;
		same=true;
		for (int z=0;z<bufferSize;z++){
		  uchar* ptrTemp = buffer[z].planes[p].ptr<uchar>(y);
		  //temp+=ptrTemp[x];
		  
		  //temp/=bufferSize;
		  if(abs(ptrCur[x]-ptrTemp[x])>thresh)
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
  //   std::cout<<"3  updateOutput called"<<std::endl;
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
