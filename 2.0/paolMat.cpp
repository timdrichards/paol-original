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
  difs = -1;
  prof = Point(0,0);
  camera = Point(0,0);

};

paolMat::paolMat(Ptr<paolMat> m)
{
  
  src = m->src.clone();
  mask = m->mask.clone();
  count = m->count;
  time = m->time;
  name = m->name;
  difs = m->difs;
  camera.x = m->camera.x;
  camera.y = m->camera.y;
  prof.x = m->prof.x;
  prof.y = m->prof.y;
};

paolMat::paolMat(paolMat* m)
{
  
  src = m->src.clone();
  mask = m->mask.clone();
  count = m->count;
  time = m->time;
  name = m->name;
  difs = m->difs;
  camera.x = m->camera.x;
  camera.y = m->camera.y;
  prof.x = m->prof.x;
  prof.y = m->prof.y;
};

paolMat::~paolMat()
{
  src.~Mat();
  mask.~Mat();
  camera.~Point();
  prof.~Point();
};

void paolMat::copy(Ptr<paolMat> m)
{
  
  src = m->src.clone();
  mask = m->mask.clone();
  count = m->count;
  time = m->time;
  name = m->name;
  difs = m->difs;
  camera.x = m->camera.x;
  camera.y = m->camera.y;
  prof.x = m->prof.x;
  prof.y = m->prof.y;
};

void paolMat::read(std::string fullName, std::string fileName,int countIn, int timeIn)
{
  name = fileName;
  src = imread(fullName);
  mask = Mat::zeros(src.size(), src.type());
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

void paolMat::writeMask()
{
  if(!src.empty())
    {
      char temp[256];
      std::string longName = "outMedia/";
      longName.append("Mask-");
      longName.append(name);
      sprintf(temp,"%06d-%010d.png",count,time);
      longName.append(temp);
      cv::imwrite(longName, mask);
      std::cout<<longName<<std::endl;
    }else
    {
      std::cout<<"   Tried to write a empty mask"<<std::endl;
    };
};

void paolMat::print()
{
  std::cout<<"     "<<name<<" time "<<time<<" count "<<count<<std::endl;

};

void paolMat::edges()
{
  cv::Mat temp;
  temp = src.clone();
  int total;

  for (int y = 1; y < src.rows-1; y++)
    {
      for (int x = 1; x < src.cols-1; x++)
	{
	  for(int i = 0; i <3; i++)
	    {
	      total+= -2*temp.at<Vec3b>((y-1),(x-1))[i];
	      total+= -1*temp.at<Vec3b>((y-1),x)[i];
	      total+= -1*temp.at<Vec3b>(y,(x-1))[i];
	      total+= 2*temp.at<Vec3b>((y+1),(x+1))[i];
	      total+= 1*temp.at<Vec3b>((y+1),x)[i];
	      total+= 1*temp.at<Vec3b>(y,(x+1))[i];
	      
	    };

	  total=abs(total);
	  if(total>128)
	    src.at<Vec3b>(y,x)[2]=255;
	  if(total>512)
	    src.at<Vec3b>(y,x)[1]=255;
	  if(total>768)
	    src.at<Vec3b>(y,x)[0]=255;
	};
    };
};

//This is a slow method for testing, not production//
void paolMat::invert()
{
  int temp;
  
  for (int y = 0; y < src.rows; y++)
    {
      for (int x = 0; x < src.cols; x++)
	{
	  temp = src.at<Vec3b>(y,x)[0];
	  temp += src.at<Vec3b>(y,x)[1];
	  temp += src.at<Vec3b>(y,x)[2];
	  
	  if (temp>40)
	    temp=255;
	  src.at<Vec3b>(y,x)[0]=temp;
	  src.at<Vec3b>(y,x)[1]=temp;
	  src.at<Vec3b>(y,x)[2]=temp;	  
	};
    };
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
  int temp;
  int thresh = 5;
  
  
  for (int y = 0; y < src.rows; y++)
    {
      for (int x = 0; x < src.cols; x++)
	{
	  for (int channel = 0; channel <3; channel++)
	    {
	      temp=background->src.at<Vec3b>(y,x)[channel]-thresh;
	      if(temp <= 0)
		temp = 1;
	      temp  = (src.at<Vec3b>(y,x)[channel] * 255) / temp;
	      if(temp > 255)
		src.at<Vec3b>(y,x)[channel] = 255;
	      else
		src.at<Vec3b>(y,x)[channel] = temp;
	    };
	};
      
    };
  
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
  int temp;
  int ave;
  for (int y = 0; y < src.rows; y++)
    {
      for (int x = 0; x < src.cols; x++)
	{
	  ave = (src.at<Vec3b>(y,x)[0] + src.at<Vec3b>(y,x)[1] + src.at<Vec3b>(y,x)[2]) /3;
	  if(ave <240 ||
	     ((src.at<Vec3b>(y,x)[0] < 220) ||
	      (src.at<Vec3b>(y,x)[1] < 220) ||
	      (src.at<Vec3b>(y,x)[2] < 220)))
	    {
	      temp = src.at<Vec3b>(y,x)[0]-(255-src.at<Vec3b>(y,x)[0]);
	      if (temp < 0)
		temp = 0;
	      
	      src.at<Vec3b>(y,x)[0] = temp;
	      
	      temp = src.at<Vec3b>(y,x)[1]-(255-src.at<Vec3b>(y,x)[1]);
	      if (temp < 0)
		temp = 0;
	      
	      src.at<Vec3b>(y,x)[1] = temp;
	      
	      temp = src.at<Vec3b>(y,x)[2]-(255-src.at<Vec3b>(y,x)[2]);
	      if (temp < 0)
		temp = 0;
	      
	      src.at<Vec3b>(y,x)[2] = temp;  
	    }else
	    {
	      src.at<Vec3b>(y,x)[0] = 255;
	      src.at<Vec3b>(y,x)[2] = 255;
	      src.at<Vec3b>(y,x)[1] = 255;
	    };
	};
    };
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
    
  int v, temp;
  double kSharp;
  kSharp = 0.75;
  v =2;
  
  
  for (int y = v; y < (src.rows -v); y++)
    {
      for (int x = v; x < (src.cols -v); x++)
	{
	  for (int channel = 0; channel <3; channel++)
	    {
	      temp = (int)(((double)src.at<Vec3b>(y,x)[channel] -
			    ( (kSharp/4) * (double)(src.at<Vec3b>((y-v),x)[channel] +
						    src.at<Vec3b>(y,(x-v))[channel] +
						    src.at<Vec3b>(y,(x+v))[channel] +
						    src.at<Vec3b>((y+v),x)[channel] )))/
			   (1.0-kSharp));
	      
	      if(temp > 255)
		img->src.at<Vec3b>(y,x)[channel] = 255;
	      else if(temp < 0)
		img->src.at<Vec3b>(y,x)[channel] = 0;
	      else
		img->src.at<Vec3b>(y,x)[channel] = temp;	    
	    };
	  
	};
    };
  img->name = "Sharp";
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
  
  img->src = Scalar(255,255,255);
  
  int total;
  
  
  for (int y = 1; y < (src.rows -1); y++)
    for (int x = 1; x < (src.cols -1); x++)
      for (int channel = 0; channel <3; channel++)
	if(src.at<Vec3b>(y,x)[channel]<255)
	  {
	    
	    total = src.at<Vec3b>((y-1),(x-1))[channel];
	    total += src.at<Vec3b>((y-1),x)[channel];
	    total += src.at<Vec3b>((y-1),(x+1))[channel];
	    
	    total += src.at<Vec3b>(y,(x-1))[channel];
	    total += src.at<Vec3b>(y,(x+1))[channel];
	    
	    total += src.at<Vec3b>((y+1),(x-1))[channel];
	    total += src.at<Vec3b>((y+1),x)[channel];
	    total += src.at<Vec3b>((y+1),(x+1))[channel];
	    
	    if(total>=1530)
	      img->src.at<Vec3b>(y,x)[channel] = 255; 
	  };
  img->name = "Shrink";
  return img;
};

void paolMat::shrink(){
  Ptr<paolMat> img;
  img = returnShrink();
  copy(img);
};

void paolMat::difference(Ptr<paolMat> img, int thresh, int size, int maskBottom)
{
  bool diff;
  int numDiff;
  int surroundThresh = 50;
  int dist;
  bool first;
  int cenx;
  int ceny;
    
  mask = Scalar(0,0,0);
  
  numDiff = 0;
  first = true;
  dist = 0;
  for (int y = size; y < (src.rows-(size+1+maskBottom)); y++)
    {	
      for (int x = size; x < (src.cols-(size+1)); x++)
	{
	  diff = false;
	  for(int i = 0; i < 3; i++)
	    {
	      if(abs((double)img->src.at<Vec3b>(y,x)[i]-(double)src.at<Vec3b>(y,x)[i])>thresh)
		diff = true;
	    };
	  if(diff)
	    {
	      mask.at<Vec3b>(y,x)[1]=255;
	      for(int yy = y-size; yy < y+size; yy++)
		{
		  for(int xx = x-size; xx < x+size; xx++)
		    {
		      for(int ii = 0; ii < 3; ii++)
			{
			  if(abs(((double)(img->src.at<Vec3b>(y,x)[ii]))-(((double)(img->src.at<Vec3b>((x+1),y)[ii])))>surroundThresh))
			    diff = false;
			  if(abs(((double)(img->src.at<Vec3b>(y,x)[ii]))-(((double)(img->src.at<Vec3b>(x,(y+1))[ii])))>surroundThresh))
			    diff = false;
			};
		    };
		};
	    };
	  if(diff)
	    {
	      numDiff++;
	      mask.at<Vec3b>(y,x)[1]=0;
	      mask.at<Vec3b>(y,x)[2]=255;
	      if(first)
		{
		  first = false;
		  cenx = x;
		  ceny = y;
		};
	      dist+=sqrt(((x-cenx)*(x-cenx))+((y-ceny)*(y-ceny)));
	    };
	};
    };
  
  if((dist<10000)&&(maskBottom>0))
    difs = 0;
  else
    difs = numDiff;
};
