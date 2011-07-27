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


using namespace cv;

paolMat::paolMat()
{
  name = "No Name";
  count = -1;
  time = -1;
  prof = Point(0,0);
  camera = Point(0,0);

};

paolMat::paolMat(Ptr<paolMat> m)
{
  
  src = m->src.clone();
  if((int)m->planes.size() > (int)planes.size())
    split();
  for(int i = 0; i < (int)m->planes.size(); i++)
    {
      m->planes[i].copyTo(planes[i]);
    };
  count = m->count;
  time = m->time;
  name = m->name;
  camera.x = m->camera.x;
  camera.y = m->camera.y;
  prof.x = m->prof.x;
  prof.y = m->prof.y;
};

paolMat::paolMat(paolMat* m)
{
  
  src = m->src.clone();
  if((int)m->planes.size() > (int)planes.size())
    split();
  for(int i = 0; i < (int)m->planes.size(); i++)
    {
      m->planes[i].copyTo(planes[i]);
    };
  count = m->count;
  time = m->time;
  name = m->name;
  camera.x = m->camera.x;
  camera.y = m->camera.y;
  prof.x = m->prof.x;
  prof.y = m->prof.y;
};

paolMat::~paolMat()
{
  src.~Mat();
  for(int i = 0; i < (int)planes.size(); i++)
    {
      planes[i].~Mat();
    };
  camera.~Point();
  prof.~Point();
};

void paolMat::copy(Ptr<paolMat> m)
{
  
  src = m->src.clone();
  if((int)m->planes.size() > (int)planes.size())
    split();
  for(int i = 0; i < (int)m->planes.size(); i++)
    {
      m->planes[i].copyTo(planes[i]);
    };
  count = m->count;
  time = m->time;
  name = m->name;
  camera.x = m->camera.x;
  camera.y = m->camera.y;
  prof.x = m->prof.x;
  prof.y = m->prof.y;
};

void paolMat::read(std::string fullName, std::string fileName,int countIn, int timeIn)
{
  name = fileName;
  src = imread(fullName);
  count=countIn;
  time=timeIn;
};

void paolMat::write()
{
  if(!src.empty())
    {
      char temp[256];
      std::string longName = "outMedia/";
      longName.append(name);
      sprintf(temp,"%06d-%010d.png",count,time);
      longName.append(temp);
      cv::imwrite(longName, src);
      std::cout<<longName<<std::endl;
    }else
    {
      std::cout<<"   Tried to write a empty src"<<std::endl;
    };
};

void paolMat::print()
{
  std::cout<<"     "<<name<<" time "<<time<<" count "<<count<<std::endl;

};

void paolMat::edges()
{
  split();
  cv::Mat temp(src);
  temp = Scalar(0,0,0);
  cv::vector<cv::Mat> tempPlanes;
  cv::split(temp, tempPlanes);
  int total;

  for (int y = 1; y < src.rows-1; y++)
    {
      uchar* sRT = planes[2].ptr<uchar>(y-1);
      uchar* sGT = planes[1].ptr<uchar>(y-1);
      uchar* sBT = planes[0].ptr<uchar>(y-1);

      uchar* sRM = planes[2].ptr<uchar>(y);
      uchar* sGM = planes[1].ptr<uchar>(y);
      uchar* sBM = planes[0].ptr<uchar>(y);

      uchar* sRB = planes[2].ptr<uchar>(y+1);
      uchar* sGB = planes[1].ptr<uchar>(y+1);
      uchar* sBB = planes[0].ptr<uchar>(y+1);

      uchar* tR = tempPlanes[2].ptr<uchar>(y);
      uchar* tG = tempPlanes[1].ptr<uchar>(y);
      uchar* tB = tempPlanes[0].ptr<uchar>(y);

      for (int x = 1; x < src.cols-1; x++)
	{
	  total=-2*sRT[x-1]-sRT[x]-sRM[x-1];
	  total+=sRM[x+1]+sRB[x]+2*sRB[x+1];

	  total+=-2*sGT[x-1]-sGT[x]-sGM[x-1];
	  total+=sGM[x+1]+sGB[x]+2*sGB[x+1];

	  total+=-2*sBT[x-1]-sBT[x]-sBM[x-1];
	  total+=sBM[x+1]+sBB[x]+2*sBB[x+1];

	  total=abs(total);
	  if(total>128)
	    tR[x]=255;
	  if(total>512)
	    tG[x]=255;
	  if(total>768)
	    tB[x]=255;
	};
    };
  cv::merge(tempPlanes, temp);
  temp.copyTo(src);
};

//This is a slow method for testing, not production//
void paolMat::invert()
{
  //cv::vector<cv::Mat> planes;
  //cv::split(src, planes);
  split();
  int temp;
  
  for (int y = 0; y < src.rows; y++)
    {
      uchar* red = planes[0].ptr<uchar>(y);
      uchar* green = planes[1].ptr<uchar>(y);
      uchar* blue = planes[2].ptr<uchar>(y);
      for (int x = 0; x < src.cols; x++)
	{
	  temp=red[x]+green[x]+blue[x];
	  if (temp>40)
	    temp=255;
	  red[x]=temp;
	  blue[x]=temp;
	  green[x]=temp;
	  /*	  if (temp>30)
	    green[x]=255;
	  else
	    green[x]=0;
	  */
	};
      
    };
  
  //cv::merge(planes, src);
  merge();
};

void paolMat::merge()
{
  if(planes.size() > 0)
    cv::merge(planes, src);
  else
    std::cout<<"Nothing to merge"<<std::endl;
};

void paolMat::createBackgroundImg(int kernalSize)
{
  cv::Point centerPoint(-1,-1);
  cv::blur(src, src, cv::Size(kernalSize,kernalSize), centerPoint, 1);
  name = "backgroundImg";
};

Ptr<paolMat> paolMat::returnCreateBackgroundImg(int kernalSize)
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->createBackgroundImg(kernalSize);
  return img;
};

void paolMat::improveInputImg(Ptr<paolMat> background)
{
  background->split();
  split();
  int temp;
  
  for (int channel = 0; channel <3; channel++)
    for (int y = 0; y < src.rows; y++)
      {
	uchar* inputPtr = planes[channel].ptr<uchar>(y);
	uchar* backgroundPtr = background->planes[channel].ptr<uchar>(y);
	
	for (int x = 0; x < src.cols; x++)
	  {
	    if(backgroundPtr[x] == 0)
	      backgroundPtr[x] = 1;
	    temp  = (inputPtr[x] * 255) / backgroundPtr[x];
	    if(temp > 255)
	      inputPtr[x] = 255;
	    else
	      inputPtr[x] = temp;
	  };

      };
  
  merge();
  name = "improvedImage";
};

Ptr<paolMat> paolMat::returnImproveInputImg(Ptr<paolMat> background)
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->improveInputImg(background);
  return img;
};

void paolMat::removeProf(){
  name = "noProf";
};

Ptr<paolMat> paolMat::returnRemoveProf()
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->removeProf();
  return img;
};

void paolMat::createContrast(){
  split();
  //int thresh;
  int temp;
  int ave;
  for (int y = 0; y < src.rows; y++)
    {
      uchar* RInPtr = planes[0].ptr<uchar>(y);		
      uchar* BInPtr = planes[1].ptr<uchar>(y);	
      uchar* GInPtr = planes[2].ptr<uchar>(y);
      
      for (int x = 0; x < src.cols; x++)
	{
	  ave = (RInPtr[x] + BInPtr[x] + GInPtr[x]) /3;
	  if(ave <240 || ((RInPtr[x] < 220) || (BInPtr[x] < 220) || (GInPtr[x] < 220)))
	    {
	      temp = RInPtr[x]-(255-RInPtr[x]);
	      if (temp < 0)
		temp = 0;
	      //else if (temp > 188)
	      //  temp = 255;
	      RInPtr[x] = temp;
	      
	      temp = BInPtr[x]-(255-BInPtr[x]);
	      if (temp < 0)
		temp = 0;
	      //else if (temp > 188)
	      //  temp = 255;
	      BInPtr[x] = temp;
	      
	      temp = GInPtr[x]-(255-GInPtr[x]);
	      if (temp < 0)
		temp = 0;
	      //else if (temp > 188)
	      //  temp = 255;
	      GInPtr[x] = temp;  
	    }else
	    {
	      RInPtr[x] = 255;
	      GInPtr[x] = 255;
	      BInPtr[x] = 255;
	    };
	};
    };
  merge();
  name = "ContrastImg";
};

Ptr<paolMat> paolMat::returnCreateContrast()
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->createContrast();
  return img;
};

Ptr<paolMat> paolMat::returnSharpen()
{
  
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->split();
  
  split();
  
  int v, temp;
  double kSharp;
  kSharp = 0.75;
  v =2;
  
  for (int channel = 0; channel <3; channel++)
    for (int y = v; y < (src.rows -v); y++)
      {
	uchar* topOldPtr = planes[channel].ptr<uchar>(y-v);
	uchar* middleOldPtr = planes[channel].ptr<uchar>(y);
	uchar* bottomOldPtr = planes[channel].ptr<uchar>(y+v);
	uchar* newPtr = img->planes[channel].ptr<uchar>(y);
	for (int x = v; x < (src.cols -v); x++)
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

  img->merge();
  img->name = "Sharp";
  //img->sharpen();
  return img;
};

void paolMat::sharpen(){
  Ptr<paolMat> img;
  img = this->returnSharpen();
  copy(img);
 
};

Ptr<paolMat> paolMat::returnShrink()
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  
  img->src = Scalar(0,0,0);
  img->split();
  split();
  int total;
  
  for (int channel = 0; channel <3; channel++)
    for (int y = 1; y < (src.rows -1); y++)
      {
	uchar* topOldPtr = planes[channel].ptr<uchar>(y-1);
	uchar* middleOldPtr = planes[channel].ptr<uchar>(y);
	uchar* bottomOldPtr = planes[channel].ptr<uchar>(y+1);
	uchar* newPtr = img->planes[channel].ptr<uchar>(y);
	
	for (int x = 1; x < (src.cols -1); x++)
	  {
	    if(middleOldPtr[x]>0)
	      {
		total = topOldPtr[x-1];
		total += topOldPtr[x];
		total += topOldPtr[x+1];
		
		total += middleOldPtr[x-1];
		total += middleOldPtr[x+1];
		
		total += bottomOldPtr[x-1];
		total += bottomOldPtr[x];
		total += bottomOldPtr[x+1];
		
		if(total>=1530)
		  newPtr[x] = 255; 
	      };
	  };
	
      };
  img->merge();
  img->name = "Shrink";
  return img;
};

void paolMat::shrink(){
  Ptr<paolMat> img;
  img = returnShrink();
  copy(img);
};


