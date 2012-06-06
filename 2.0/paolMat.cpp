// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
#include <algorithm>
#include <cstdio>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "paolMat.h"

//#define _debug_

using namespace cv;

paolMat::paolMat()
{
  name = "No Name";
  count = -1;
  time = -1;
  difs = -1;
  prof = Point(0,0);
  camera = Point(0,0);
  lectFound = false;
  r =0;
  g =0;
  b =0;
}

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
  lectFound = m->lectFound;
  r = m->r;
  g = m->g;
  b = m->b;
}

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
  lectFound = m->lectFound;
  r = m->r;
  g = m->g;
  b = m->b;
}

paolMat::~paolMat()
{
  src.~Mat();
  mask.~Mat();
  camera.~Point();
  prof.~Point();
}

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
  lectFound = m->lectFound;
  r = m->r;
  g = m->g;
  b = m->b;
}

void paolMat::copyNoSrc(Ptr<paolMat> m)
{
  
  mask = m->mask.clone();
  count = m->count;
  time = m->time;
  name = m->name;
  difs = m->difs;
  camera.x = m->camera.x;
  camera.y = m->camera.y;
  prof.x = m->prof.x;
  prof.y = m->prof.y;
  lectFound = m->lectFound;
  r = m->r;
  g = m->g;
  b = m->b;
}

void paolMat::read(std::string fullName, std::string fileName,int countIn, int timeIn)
{
  name = fileName;
  src = imread(fullName);
  mask = Mat::zeros(src.size(), src.type());
  count=countIn;
  time=timeIn;
  if(src.data)
    std::cout<<"PaolMat:: Read: "<<fullName<<std::endl;
}

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
    }
}

void paolMat::write(std::string outDir)
{
  if(!src.empty())
    {
      char temp[256];
      std::string longName = outDir;
      longName.append(name);
      sprintf(temp,"%06d-%010d.png",count,time);
      longName.append(temp);
      cv::imwrite(longName, src);
      std::cout<<longName<<std::endl;
    }else
    {
      std::cout<<"   Tried to write a empty src"<<std::endl;
    }
}

void paolMat::writeByCount(std::string outDir)
{
  if(!src.empty())
    {
      char temp[256];
      std::string longName = outDir;
      longName.append(name);
      sprintf(temp,"_%06d.ppm",count);
      longName.append(temp);
      cv::imwrite(longName, src);
      std::cout<<longName<<std::endl;
    }else
    {
      std::cout<<"   Tried to write a empty src"<<std::endl;
    }
}


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
    }
}

void paolMat::print()
{
  std::cout<<"     "<<name<<" time "<<time<<" count "<<count<<std::endl;

}

void paolMat::edges()
{
  cv::Mat temp;
  temp = src.clone();
  src = Mat::zeros(src.size(),src.type());
  int total;

  for (int y = 1; y < src.rows-1; y++)
    {
      for (int x = 1; x < src.cols-1; x++)
	{
	  total=0;
	  for(int i = 0; i <3; i++)
	    {
	      total+= -2*temp.at<Vec3b>((y-1),(x-1))[i];
	      total+= -1*temp.at<Vec3b>((y-1),x)[i];
	      total+= -1*temp.at<Vec3b>(y,(x-1))[i];
	      total+= 2*temp.at<Vec3b>((y+1),(x+1))[i];
	      total+= 1*temp.at<Vec3b>((y+1),x)[i];
	      total+= 1*temp.at<Vec3b>(y,(x+1))[i];
	      
	    }

	  total=abs(total);
	  if(total>56)
	    src.at<Vec3b>(y,x)[2]=255;
	  //if(total>512)
	  //  src.at<Vec3b>(y,x)[1]=255;
	  //if(total>768)
	  //  src.at<Vec3b>(y,x)[0]=255;
	}
    }
  name="edges";
}

Ptr<paolMat> paolMat::returnEdges()
{
  Ptr<paolMat> out;
  out = new paolMat(this);
  out->edges();
  return out;

}

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
	}
    }
}

void paolMat::createBackgroundImg(int kernalSize)
{
  cv::Point centerPoint(-1,-1);
  cv::blur(src, src, cv::Size(kernalSize,kernalSize), centerPoint, 1);
  name = "backgroundImg";
}

Ptr<paolMat> paolMat::returnCreateBackgroundImg(int kernalSize)
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->createBackgroundImg(kernalSize);
  return img;
}

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
	    }
	}
      
    }
  
  name = "improvedImage";
}

Ptr<paolMat> paolMat::returnImproveInputImg(Ptr<paolMat> background)
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->improveInputImg(background);
  return img;
}

void paolMat::removeProf(Ptr<paolMat> oldImg){
  //int totalDiff;
  name = "noProf";
  connected();
  lectArea();
  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      if(mask.at<Vec3b>(y,x)[2]==255)
	for(int i = 0; i < 3; i++)
	  src.at<Vec3b>(y,x)[i] = oldImg->src.at<Vec3b>(y,x)[i];
  
}

Ptr<paolMat> paolMat::returnRemoveProf(Ptr<paolMat> oldImg)
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->removeProf(oldImg);
  return img;
}

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
	    }
	}
    }
  name = "ContrastImg";
}

Ptr<paolMat> paolMat::returnCreateContrast()
{
  Ptr<paolMat> img;
  img = new paolMat(this);
  img->createContrast();
  return img;
}

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
	    }
	  
	}
    }
  img->name = "Sharp";
  return img;
}

void paolMat::sharpen(){
  Ptr<paolMat> img;
  img = this->returnSharpen();
  copy(img);
 
}

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
	  }
  img->name = "Shrink";
  return img;
}

void paolMat::shrink(){
  Ptr<paolMat> img;
  img = returnShrink();
  copy(img);
}

void paolMat::difference(Ptr<paolMat> img, int thresh, int size, int maskBottom)
{
  bool diff;
  int numDiff;
  int surroundThresh = 50;
  int dist;
  bool first;
  int cenx;
  int ceny;
  int total;
    
  mask = Mat::zeros(mask.size(), mask.type());
  
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
	    }
	  if(diff)
	    {
	      //std::cout<<"First if dif size: "<<size<<std::endl;
	      //mask.at<Vec3b>(y,x)[1]=255;
	      for(int yy = y-size; yy < y+size; yy++)
		{
		  for(int xx = x-size; xx < x+size; xx++)
		    {
		      for(int ii = 0; ii < 3; ii++)
			{
			  if(abs(((double)(img->src.at<Vec3b>(yy,xx)[ii]))-(((double)(img->src.at<Vec3b>((yy+1),xx)[ii])))>surroundThresh))
			    diff = false;
			  if(abs(((double)(img->src.at<Vec3b>(yy,xx)[ii]))-(((double)(img->src.at<Vec3b>(yy,(xx+1))[ii])))>surroundThresh))
			    diff = false;
			}
		    }
		}
	    }
	  if(diff)
	    {
	      //std::cout<<"Second if diff"<<std::endl;
	      //numDiff++;
	      total = abs((double)img->src.at<Vec3b>(y,x)[0]-(double)src.at<Vec3b>(y,x)[0]) +
		abs((double)img->src.at<Vec3b>(y,x)[1]-(double)src.at<Vec3b>(y,x)[1]) +
		abs((double)img->src.at<Vec3b>(y,x)[2]-(double)src.at<Vec3b>(y,x)[2]);
	      if(total > 512)
		{
		  mask.at<Vec3b>(y,x)[0] = 255;
		}
	      if(total > 255)
		{
		  mask.at<Vec3b>(y,x)[1] = 255;
		  numDiff++;
		}
	      mask.at<Vec3b>(y,x)[2]=255;
	      if(first)
		{
		  first = false;
		  cenx = x;
		  ceny = y;
		}
	      //std::cout<<"Difference x: "<<x<<" cenx: "<<cenx<<" y:"<<y<<" ceny: "<<ceny<<std::endl;
	      dist+=sqrt(((x-cenx)*(x-cenx))+((y-ceny)*(y-ceny)));
	    }
	}
    }
  //std::cout<<"Difference dist: "<<dist<<std::endl;
  if((dist<10000)&&(maskBottom>0))
    difs = 0;
  else
    difs = numDiff;
}

void paolMat::differenceLect(Ptr<paolMat> inImg,int thresh, int size)
{
  bool diff;
  int numDiff;
  int surroundThresh = 50;
  int dist;
  bool first;
  int cenx;
  int ceny;
  int pixDif;
  
  mask = Mat::zeros(src.size(), src.type());
  
  numDiff = 0;
  first = true;
  dist = 0;
  for (int y = 0; y < (src.rows-size-1); y+= size)
    for (int x = 0; x < (src.cols-size-1); x+=size)
      {
	diff = false;
	pixDif=abs((double)inImg->src.at<Vec3b>(y,x)[0]-
		   (double)src.at<Vec3b>(y,x)[0])+
	  abs((double)inImg->src.at<Vec3b>(y,x)[1]-
	      (double)src.at<Vec3b>(y,x)[1])+
	  abs((double)inImg->src.at<Vec3b>(y,x)[2]-
	      (double)src.at<Vec3b>(y,x)[2]);
	if (pixDif>thresh)
	  {
	    mask.at<Vec3b>(y,x)[2] = 255;
	    diff = true;
	  }
	if(pixDif > 255)
	  pixDif = 255;
	mask.at<Vec3b>(y,x)[1] = 0;//pixDif;
	mask.at<Vec3b>(y,x)[0] = 0;
	   
	    
	
	if(diff)
	  {
	    
	    //  for(int a = 0; a <= size; a++)
	    //  for(int b = 0; b <= size; b++)
	    //mask.at<Vec3b>(y+a,x+b)[1]=255;
	    numDiff++;
	  }
	difs = numDiff;
      }
}

void paolMat::localizeSpeaker()
{

  int left = 0;
  int right = mask.cols-1;
  int top = 0;
  int x = 0;
  int y = 0;
  int count;
  int countIgnore = 10;

  if(difs > 750)
    {
      count = countIgnore;
      while(count >=  0 && (x < mask.cols))
	{
	  for(y = 0; y < mask.rows; y++)
	    {
	      if(mask.at<Vec3b>(y,x)[1] > 0)
		{
		  count--;
		  left = x;
		}
	    }
	  x++;
	}
      
      x = mask.cols-1;
      count = countIgnore;
      while(count >= 0 && (x >= 0))
	{
	  for(y = 0; y < mask.rows; y++)
	    {
	      if(mask.at<Vec3b>(y,x)[1] > 0)
		{
		  count--;
		  right = x;
		}
	    }
	  x--;
	}
      
      y=0;
      count = countIgnore;
      while(count >= 0 && (y < mask.cols))
	{
	  for(x = left; x <= right; x++)
	    {
	      if(mask.at<Vec3b>(y,x)[1] > 0)
		{
		  count--;
		  top = y;
		}
	    }
	  y++;
	}
      
      
      prof.x = (left+right) / 2;
      prof.y = top;
      std::cout<<"Prof at X: "<<prof.x<<" Y: "<<prof.y<<std::endl;
      lectFound = true;
#ifdef _debug_
      if(prof.x < 3)
	prof.x = 3;
      if(prof.x > (mask.cols-3))
	prof.x = mask.cols-4;
      if(prof.y < 3)
	prof.y = 3;
      if(prof.y > (mask.rows-3))
	prof.y = mask.rows-4;
      char difString[10];
      sprintf(difString, "%d", difs);
      //rectangle(src, Point(left,mask.rows-1), Point(right, top), Scalar(0,0,0), 1,8);
      //rectangle(mask, Point(left,mask.rows-1), Point(right, top), Scalar(255,0,0), 1,8);
      //rectangle(src, Point(prof.x-2,prof.y-2), Point(prof.x+2, prof.y+2), Scalar(0,0,0), 1,8);
      //rectangle(mask, Point(prof.x-2,prof.y-2), Point(prof.x+2, prof.y+2), Scalar(255,0,0), 1,8);
      //putText(src, difString, Point(100,100), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 5, 8);
      //putText(mask, difString, Point(100,100), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255,0,0), 5, 8);
#endif
    }else
    {
      prof.x = -1;
      prof.y = -1;
      lectFound = false;
#ifdef _debug_
      char difString[10];
      sprintf(difString, "%d", difs);
      //putText(mask, difString, Point(100,100), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255,0,0), 5, 8);
#endif
    }
}

void paolMat::decimateMask()
{
  int left,right,top,bottom;
  for(int x = 0; x < src.cols-1; x++)
    for(int y = 0; y < src.rows-1; y++)
      {
	if(mask.at<Vec3b>(y,x)[1]==255)
	  {
	    left=(x-50<0)? 0:x-50;
	    right=(x+50>mask.cols-1)? mask.cols-1:x+50;
	    top=(y-50<0)? 0:y-50;
	    bottom=(y+50>mask.rows-1)? mask.rows-1:y+50;
	    for(int xx = left; xx < right; xx++)
	      for(int yy = top; yy < bottom; yy++)
		{
		  if(mask.at<Vec3b>(y,x)[0]>0 || mask.at<Vec3b>(y,x)[2]>0)
		    mask.at<Vec3b>(y,x)[2]=128;
		}
	  }
      }
  for(int x = 0; x < src.cols-1; x++)
    for(int y = 0; y < src.rows-1; y++)
      {
	if(mask.at<Vec3b>(y,x)[2]==128)
	  mask.at<Vec3b>(y,x)[1]=255;
      }
}

void paolMat::decimateMask(int thresh)
{
  Ptr<paolMat> temp;
  temp = new paolMat(this);
  temp->mask = Scalar(0,0,0);
  

  int count;
  for(int x = 1; x < mask.cols-1; x++)
    for(int y = 1; y < mask.rows-1; y++)
      {
	count = 0;
	if(mask.at<Vec3b>(y-1,x-1)[2] > thresh)
	  count++;
	if(mask.at<Vec3b>(y-1,x)[2] > thresh)
	  count++;
	if(mask.at<Vec3b>(y-1,x+1)[2] > thresh)
	  count++;
	if(mask.at<Vec3b>(y+1,x-1)[2] > thresh)
	  count++;
	if(mask.at<Vec3b>(y+1,x)[2] > thresh)
	  count++;
	if(mask.at<Vec3b>(y+1,x+1)[2] > thresh)
	  count++;
	if(mask.at<Vec3b>(y,x-1)[2] > thresh)
	  count++;
	if(mask.at<Vec3b>(y,x+1)[2] > thresh)
	  count++;
	if(count >5)
	  temp->mask.at<Vec3b>(y,x)[2] = 255;
	else
	  temp->mask.at<Vec3b>(y,x)[0] = 0;
      }
  mask = temp->mask.clone();
}
void paolMat::growMask()
{
  Ptr<paolMat> temp;
  temp = new paolMat(this);
  temp->mask = Scalar(0,0,0);
  

  int total;
  for(int x = 1; x < mask.cols-1; x++)
    for(int y = 1; y < mask.rows-1; y++)
      {
	//y-1 ------------------------------
	//y-1,x-1
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y-1,x-1)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y-1,x-1)[2] = total;
	//y-1,x
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y-1,x)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y-1,x)[2] = total;
	//y-1,x+1
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y-1,x+1)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y-1,x+1)[2] = total;

	//y+1 ---------------------------------
	//y+1,x-1
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y+1,x-1)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y+1,x-1)[2] = total;
	//y+1,x
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y+1,x)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y+1,x)[2] = total;
	//y+1,x+1
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y+1,x+1)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y+1,x+1)[2] = total;

	//y ---------------------------------
	//yx-1
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y,x-1)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y,x-1)[2] = total;
	//y,x+1
	total = mask.at<Vec3b>(y,x)[2] +
	  mask.at<Vec3b>(y,x+1)[2];
	if(total > 255)
	  total = 255;
	temp->mask.at<Vec3b>(y,x+1)[2] = total;
	
      }
  mask = temp->mask.clone();
}

void paolMat::shrinkMask()
{
  Ptr<paolMat> temp;
  temp = new paolMat(this);
  temp->mask = Scalar(0,0,0);
  

  int total;
  for(int x = 1; x < mask.cols-1; x++)
    for(int y = 1; y < mask.rows-1; y++)
      {
	//y-1 ------------------------------
	//y-1,x-1
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y-1,x-1)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y-1,x-1)[2] = total;
	//y-1,x
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y-1,x)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y-1,x)[2] = total;
	//y-1,x+1
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y-1,x+1)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y-1,x+1)[2] = total;

	//y+1 ---------------------------------
	//y+1,x-1
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y+1,x-1)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y+1,x-1)[2] = total;
	//y+1,x
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y+1,x)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y+1,x)[2] = total;
	//y+1,x+1
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y+1,x+1)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y+1,x+1)[2] = total;

	//y ---------------------------------
	//yx-1
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y,x-1)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y,x-1)[2] = total;
	//y,x+1
	total = mask.at<Vec3b>(y,x)[2] -
	  mask.at<Vec3b>(y,x+1)[2];
	if(total < 0)
	  total = 0;
	temp->mask.at<Vec3b>(y,x+1)[2] = total;
	
      }
  mask = temp->mask.clone();
}


void paolMat::connected(){
  Ptr<paolMat> connect;
  connect = new paolMat(this);
  std::vector<int> cor;
  cor.resize(256*256*256,0);
  std::vector<int> corCount;
  corCount.resize(256*256*256,0);
  int current=1;
  cor[1]=1;
  int upCol,leftCol,col;
  
  mask.at<Vec3b>(0,0)[0]=0;
  mask.at<Vec3b>(0,0)[1]=0;
  mask.at<Vec3b>(0,0)[2]=0;

  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      {
	if (mask.at<Vec3b>(y,x)[0]>0 ||
	    mask.at<Vec3b>(y,x)[1]>0 ||
	    mask.at<Vec3b>(y,x)[2]>0){
	  if (x==0){
	    if (connect->src.at<Vec3b>(y-1,x)[0]>0 ||
		connect->src.at<Vec3b>(y-1,x)[1]>0 ||
		connect->src.at<Vec3b>(y-1,x)[2]>0){
	      connect->src.at<Vec3b>(y,x)[0]=connect->src.at<Vec3b>(y-1,x)[0];
	      connect->src.at<Vec3b>(y,x)[1]=connect->src.at<Vec3b>(y-1,x)[1];
	      connect->src.at<Vec3b>(y,x)[2]=connect->src.at<Vec3b>(y-1,x)[2];
	    } else {
	      connect->src.at<Vec3b>(y,x)[0]=current%256;
	      connect->src.at<Vec3b>(y,x)[1]=(current/256)%256;
	      connect->src.at<Vec3b>(y,x)[2]=(current/(256*256))%256;
	      current++;
	      cor[current]=current;
	    }
	  } else if(y==0){
	    if (connect->src.at<Vec3b>(y,x-1)[0]>0 ||
		connect->src.at<Vec3b>(y,x-1)[1]>0 ||
		connect->src.at<Vec3b>(y,x-1)[2]>0){
	      connect->src.at<Vec3b>(y,x)[0]=connect->src.at<Vec3b>(y,x-1)[0];
	      connect->src.at<Vec3b>(y,x)[1]=connect->src.at<Vec3b>(y,x-1)[1];
	      connect->src.at<Vec3b>(y,x)[2]=connect->src.at<Vec3b>(y,x-1)[2];
	    } else {
	      connect->src.at<Vec3b>(y,x)[0]=current%256;
	      connect->src.at<Vec3b>(y,x)[1]=(current/256)%256;
	      connect->src.at<Vec3b>(y,x)[2]=(current/(256*256))%256;
	      current++;
	      cor[current]=current;
	    }
	  } else {
	    if((connect->src.at<Vec3b>(y-1,x)[0]>0 ||
		connect->src.at<Vec3b>(y-1,x)[1]>0 ||
		connect->src.at<Vec3b>(y-1,x)[2]>0) &&
	       (connect->src.at<Vec3b>(y,x-1)[0]>0 ||
		connect->src.at<Vec3b>(y,x-1)[1]>0 ||
		connect->src.at<Vec3b>(y,x-1)[2]>0)){
	      upCol=connect->src.at<Vec3b>(y-1,x)[0];
	      upCol+=connect->src.at<Vec3b>(y-1,x)[1]*256;
	      upCol+=connect->src.at<Vec3b>(y-1,x)[2]*256*256;
	      leftCol=connect->src.at<Vec3b>(y,x-1)[0];
	      leftCol+=connect->src.at<Vec3b>(y,x-1)[1]*256;
	      leftCol+=connect->src.at<Vec3b>(y,x-1)[2]*256*256;
	      if(upCol<leftCol){
		cor[leftCol]=cor[upCol];
		connect->src.at<Vec3b>(y,x)[0]=connect->src.at<Vec3b>(y-1,x)[0];
		connect->src.at<Vec3b>(y,x)[1]=connect->src.at<Vec3b>(y-1,x)[1];
		connect->src.at<Vec3b>(y,x)[2]=connect->src.at<Vec3b>(y-1,x)[2];
	      } else {
		cor[upCol]=cor[leftCol];
		connect->src.at<Vec3b>(y,x)[0]=connect->src.at<Vec3b>(y,x-1)[0];
		connect->src.at<Vec3b>(y,x)[1]=connect->src.at<Vec3b>(y,x-1)[1];
		connect->src.at<Vec3b>(y,x)[2]=connect->src.at<Vec3b>(y,x-1)[2];
	      }
	    } else if (connect->src.at<Vec3b>(y-1,x)[0]>0 ||
		       connect->src.at<Vec3b>(y-1,x)[1]>0 ||
		       connect->src.at<Vec3b>(y-1,x)[2]>0){
	      connect->src.at<Vec3b>(y,x)[0]=connect->src.at<Vec3b>(y-1,x)[0];
	      connect->src.at<Vec3b>(y,x)[1]=connect->src.at<Vec3b>(y-1,x)[1];
	      connect->src.at<Vec3b>(y,x)[2]=connect->src.at<Vec3b>(y-1,x)[2];
	    } else if (connect->src.at<Vec3b>(y,x-1)[0]>0 ||
		       connect->src.at<Vec3b>(y,x-1)[1]>0 ||
		       connect->src.at<Vec3b>(y,x-1)[2]>0){
	      connect->src.at<Vec3b>(y,x)[0]=connect->src.at<Vec3b>(y,x-1)[0];
	      connect->src.at<Vec3b>(y,x)[1]=connect->src.at<Vec3b>(y,x-1)[1];
	      connect->src.at<Vec3b>(y,x)[2]=connect->src.at<Vec3b>(y,x-1)[2];
	    } else {
	      connect->src.at<Vec3b>(y,x)[0]=current%256;
	      connect->src.at<Vec3b>(y,x)[1]=(current/256)%256;
	      connect->src.at<Vec3b>(y,x)[2]=(current/(256*256))%256;
	      current++;
	      cor[current]=current;
	    }
	  }
	}else{
	  connect->src.at<Vec3b>(y,x)[0]=0;
	  connect->src.at<Vec3b>(y,x)[1]=0;
	  connect->src.at<Vec3b>(y,x)[2]=0;
	  
	}
      }

  for(int i=1;i<current;i++){
    cor[i]=cor[cor[i]];
  }
  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      {
	col=connect->src.at<Vec3b>(y,x)[0];
	col+=connect->src.at<Vec3b>(y,x)[1]*256;
	col+=connect->src.at<Vec3b>(y,x)[2]*256*256;
	col=cor[col];
	corCount[col]++;
	connect->src.at<Vec3b>(y,x)[0]=col%256;
	connect->src.at<Vec3b>(y,x)[1]=(col/256)%256;
	connect->src.at<Vec3b>(y,x)[2]=(col/(256*256))%256;
      }

  for(int i=1;i<current;i++){
    if(corCount[i]<10)
      cor[i]=0;
  }
  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      {
	col=connect->src.at<Vec3b>(y,x)[0];
	col+=connect->src.at<Vec3b>(y,x)[1]*256;
	col+=connect->src.at<Vec3b>(y,x)[2]*256*256;
	col=cor[col];
	connect->src.at<Vec3b>(y,x)[0]=col%256;
	connect->src.at<Vec3b>(y,x)[1]=(col/256)%256;
	connect->src.at<Vec3b>(y,x)[2]=(col/(256*256))%256;
      }
#ifdef _debug_
  connect->name="connect";
  connect->write();
#endif
  for(int i=1;i<current;i++){
    cor[i]=0;
  }
  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      {
	if(connect->src.at<Vec3b>(y,x)[0]>0 && abs(x-prof.x) < 100 && y > prof.y-50)
	  {
	    col=connect->src.at<Vec3b>(y,x)[0];
	    col+=connect->src.at<Vec3b>(y,x)[1]*256;
	    col+=connect->src.at<Vec3b>(y,x)[2]*256*256;
	    cor[col]=255;
	  }
      }
  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      {
	col=connect->src.at<Vec3b>(y,x)[0];
	col+=connect->src.at<Vec3b>(y,x)[1]*256;
	col+=connect->src.at<Vec3b>(y,x)[2]*256*256;
	col=cor[col];
	connect->src.at<Vec3b>(y,x)[0]=col%256;
	connect->src.at<Vec3b>(y,x)[1]=(col/256)%256;
	connect->src.at<Vec3b>(y,x)[2]=(col/(256*256))%256;
      }
#ifdef _debug_
  connect->name="connect2";
  connect->write();
#endif
  connect->src.copyTo(mask);
}


void paolMat::connected(int size){
  Ptr<paolMat> connect;
  connect = new paolMat(this);
  connect->src = Mat::zeros(src.size(), src.type());
  for(int x = size+1; x < src.cols-(size+2); x+=size)
    for(int y = size+1; y < src.rows-(size+2); y+=size)
      {
	int touch = 0;
	touch += mask.at<Vec3b>(y-size,x)[1]%255;
	touch += mask.at<Vec3b>(y+size,x)[1]%255;
	touch += mask.at<Vec3b>(y,x-size)[1]%255;
	touch += mask.at<Vec3b>(y,x+size)[1]%255;
	touch += mask.at<Vec3b>(y-size,x-size)[1]%255;
	touch += mask.at<Vec3b>(y-size,x+size)[1]%255;
	touch += mask.at<Vec3b>(y+size,x-size)[1]%255;
	touch += mask.at<Vec3b>(y+size,x+size)[1]%255;
	for(int xx = x; xx < x+size; xx++)
	    for(int yy = y; yy < y+size; yy++)
	      if(touch >= 1)
		{
		  connect->src.at<Vec3b>(yy,xx)[1] = 255;
		}
      }
	

  //#ifdef _debug_
  connect->name="connect2";
  connect->write();
  //#endif
  connect->src.copyTo(mask);

}


void paolMat::lectArea(){
  Ptr<paolMat> profile;
  profile = new paolMat(this);
  int x,y;  
  int border=5;
  int left,right;

  for(y = 0; y < src.rows; y++){
    //from left
    for(x = 0; (x < src.cols) && (mask.at<Vec3b>(y,x)[0]==0); x++)
      profile->mask.at<Vec3b>(y,x)[2]=0;
    if(x<src.cols-1)
      {
	//std::cout<<"y: "<<y<<" x: "<<x<<" cols: "<<src.cols<<std::endl;
	//x=(x-border<0)? 0:x-border;
	for(;x<src.cols;x++)
	  profile->mask.at<Vec3b>(y,x)[2]=255;
      }
    //from right
    for(x = src.cols-1; (x >= 0) && (mask.at<Vec3b>(y,x)[0]==0); x--)
      profile->mask.at<Vec3b>(y,x)[1]=0;
    if(x>1)
      {
	//std::cout<<"y: "<<y<<" x: "<<x<<" cols: "<<src.cols<<std::endl;
	//x=(x+border>=src.cols)? src.cols-1:x+border;
	for(;x>0;x--)
	  profile->mask.at<Vec3b>(y,x)[1]=255;
      }
  }
  for(x = 0; x < src.cols; x++)
    {
      //from top
      for(y = 0; (y < src.rows) && (mask.at<Vec3b>(y,x)[0]==0); y++)
	profile->mask.at<Vec3b>(y,x)[0]=0;
      if(y<=src.rows)
	{
	  //	  y=(y-border<0)? 0:y-border;
	  for(;y<src.rows;y++)
	    profile->mask.at<Vec3b>(y,x)[0]=255;
	}
    } 
#ifdef _debug_
  profile->name="profile";
  profile->writeMask();
#endif
  for(x = 0; x < src.cols; x++)
    for(y = 0; y < src.rows; y++)
      {
	if(profile->mask.at<Vec3b>(y,x)[0]+
	   profile->mask.at<Vec3b>(y,x)[1]+
	   profile->mask.at<Vec3b>(y,x)[2] >= 510)
	  {
	    profile->mask.at<Vec3b>(y,x)[0] = 0;//src.at<Vec3b>(y,x)[0];
	    profile->mask.at<Vec3b>(y,x)[1] = 0;//src.at<Vec3b>(y,x)[1];
	    profile->mask.at<Vec3b>(y,x)[2] = 255;//src.at<Vec3b>(y,x)[2];
	  }else
	  {
	    profile->mask.at<Vec3b>(y,x)[0] = 0;//src.at<Vec3b>(y,x)[0];
	    profile->mask.at<Vec3b>(y,x)[1] = 0;//src.at<Vec3b>(y,x)[1];
	    profile->mask.at<Vec3b>(y,x)[2] = 0;//src.at<Vec3b>(y,x)[2];
	  }
      } 
#ifdef _debug_
  profile->name="profileA";
  profile->writeMask();
#endif
  for(x = 0; x < src.cols; x++)
    {
      //from top
      for(y = 0; (y < src.rows) && (profile->mask.at<Vec3b>(y,x)[2]==0); y++)
	profile->mask.at<Vec3b>(y,x)[2]=0;
      if(y<src.rows-1)
	{
	  y=(y-border<0)? 0:y-border;
	  for(;y<src.rows;y++)
	    profile->mask.at<Vec3b>(y,x)[2]=255;
	}
    } 
  
  for(y = 0; y < src.rows; y++){
    //from left
    for(x = 0; (x < src.cols) && (profile->mask.at<Vec3b>(y,x)[2]==0); x++)
      profile->mask.at<Vec3b>(y,x)[2]=0;
    if(x<src.cols-1)
      {
	//std::cout<<"y: "<<y<<" x: "<<x<<" cols: "<<src.cols<<std::endl;
	left=(x-border<0)? 0:x-border;
	//from right
	
	for(x = src.cols-1; (x >= 0) && (profile->mask.at<Vec3b>(y,x)[2]==0); x--)
	  profile->mask.at<Vec3b>(y,x)[0]=0;
	if(x>1)
	  {
	    //std::cout<<"y: "<<y<<" x: "<<x<<" cols: "<<src.cols<<std::endl;
	    right=(x+border>=src.cols)? src.cols-1:x+border;
	    for(x=left;x<right;x++)
	      profile->mask.at<Vec3b>(y,x)[2]=255;
	  }
      }
  }
  
  /*
  for(x = 0; x < src.cols; x++)
    for(y = 0; y < src.rows; y++)
      {
	if(profile->mask.at<Vec3b>(y,x)[2] == 255)
	  {
	    profile->mask.at<Vec3b>(y,x)[0] = 0;//src.at<Vec3b>(y,x)[0];
	    profile->mask.at<Vec3b>(y,x)[1] = 0;//src.at<Vec3b>(y,x)[1];
	    profile->mask.at<Vec3b>(y,x)[2] = src.at<Vec3b>(y,x)[2];
	  }else
	  {
	    profile->mask.at<Vec3b>(y,x)[0] = src.at<Vec3b>(y,x)[0];
	    profile->mask.at<Vec3b>(y,x)[1] = src.at<Vec3b>(y,x)[1];
	    profile->mask.at<Vec3b>(y,x)[2] = src.at<Vec3b>(y,x)[2];
	  }
	  } */
#ifdef _debug_
  profile->name="profileB";
  profile->writeMask();
#endif
  profile->mask.copyTo(mask);
}

Ptr<paolMat> paolMat::cropFrame(int width, int height){
  Ptr<paolMat> outFrame;
  outFrame = new paolMat(this);
  cv::Point upperLeft;
  cv::Point lowerRight;
  int temp;
  
  upperLeft.x=camera.x-width/2;
  upperLeft.y=camera.y-height/2;
  lowerRight.x=camera.x+width/2-1;
  lowerRight.y=camera.y+height/2-1;

  if(upperLeft.x<0)
    {
      temp=0-upperLeft.x;
      upperLeft.x+=temp;
      lowerRight.x+=temp;
    }
  if(lowerRight.x>src.cols-1)
    {
      temp=src.cols-1-lowerRight.x;
      upperLeft.x+=temp;
      lowerRight.x+=temp;
    }
  if(upperLeft.y<0)
    {
      temp=0-upperLeft.y;
      upperLeft.y+=temp;
      lowerRight.y+=temp;
    }
  if(lowerRight.y>src.rows-1)
    {
      temp=src.rows-1-lowerRight.y;
      upperLeft.y+=temp;
      lowerRight.y+=temp;
    }

  outFrame->src = Mat(src, Rect(upperLeft.x, upperLeft.y, lowerRight.x-upperLeft.x, lowerRight.y - upperLeft.y));

  outFrame->name = "Cropped";
#ifdef _debug_
  outFrame->write();
#endif
  return outFrame;

  //now use upper and lower right to crop the frame
}

vector<int> paolMat::vertMaskHistogram()
{
  vector<int> hist;
  int vertCount;


  for(int x = 0; x < mask.cols; x++)
    {
      vertCount = 0;
      for(int y = 0; y < mask.rows; y++)
	{
	  if(mask.at<Vec3b>(y,x)[2] == 255)
	    vertCount ++;
	}
      hist.push_back(vertCount);
    }
  return hist;
}

vector<int> paolMat::horMaskHistogram()
{
  vector<int> hist;
  int horCount;


  for(int y = 0; y < mask.rows; y++)
    {
      horCount = 0;
      for(int x = 0; x < mask.cols; x++)
	{
	  if(mask.at<Vec3b>(y,x)[2] == 255)
	    horCount ++;
	}
      hist.push_back(horCount);
    }
  return hist;
}

void paolMat::decimateMaskByHistogram(int hThresh, int vThresh)
{
  vector<int> horHisto;
  vector<int> vertHisto;

  horHisto = horMaskHistogram();
  vertHisto = vertMaskHistogram();

  for(int x = 0; x < mask.cols; x++)
    for(int y = 0; y < mask.rows; y++)
      {
	if( mask.at<Vec3b>(y,x)[2] == 255 && 
	    ( (horHisto[y] > hThresh) && 
	      (vertHisto[x] > vThresh)))
	  mask.at<Vec3b>(y,x)[1] = 255;
	else
	  {
	    //mask.at<Vec3b>(y,x)[1] = 0;
	    //mask.at<Vec3b>(y,x)[0] = 0;
	    //mask.at<Vec3b>(y,x)[2] = 0;
	  }
      }
}

void paolMat::drift()
{
  int temp;
  int total;
  for(int x = 0; x < src.cols -1; x++)
    for(int y = 0; y < src.rows -1; y++)
      {
	//Clear any previous mask values
	mask.at<Vec3b>(y,x)[0] = 0;
	mask.at<Vec3b>(y,x)[1] = 0;
	mask.at<Vec3b>(y,x)[2] = 0;

	//Get difference of x+1
	temp = abs( src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y,x+1)[0] )+
	  abs( src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y,x+1)[1] )+
	  abs( src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y,x+1)[2] );
	total = temp;
	if(temp > 255)
	  temp = 255;
	mask.at<Vec3b>(y,x)[0] = temp;

	//Get difference of y+1
	temp = abs( src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y+1,x)[0] )+
	  abs( src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y+1,x)[1] )+
	  abs( src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y+1,x)[2] );
	total+=temp;
	if(temp > 255)
	  temp = 255;

	//Threshold for differences from previous pixel
	if(total > 50)
	  total = 255;
	mask.at<Vec3b>(y,x)[1]=temp;
	mask.at<Vec3b>(y,x)[2]=total;
      }
}

void paolMat::driftWAverage()
{
  average();
  int temp;
  int total;
  for(int x = 0; x < src.cols -1; x++)
    for(int y = 0; y < src.rows -1; y++)
      {
	temp = abs( src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y,x+1)[0] )+
	  abs( src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y,x+1)[1] )+
	  abs( src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y,x+1)[2] );
	total = temp;
	if(temp > 255)
	  temp = 255;
	if(
	   (abs( src.at<Vec3b>(y,x)[0] - b) < abs(src.at<Vec3b>(y,x+1)[0] -b)) &&
	   (abs( src.at<Vec3b>(y,x)[1] - g) < abs(src.at<Vec3b>(y,x+1)[1] -g)) &&
	   (abs( src.at<Vec3b>(y,x)[2] - r) < abs(src.at<Vec3b>(y,x+1)[2] -r)) )
	  mask.at<Vec3b>(y,x)[0] = temp;
	else
	  {
	    mask.at<Vec3b>(y,x)[0] = temp;
	    mask.at<Vec3b>(y,x+1)[0] = temp;
	  } 
	
	temp = abs( src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y+1,x)[0] )+
	  abs( src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y+1,x)[1] )+
	  abs( src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y+1,x)[2] );
	total+=temp;
	if(temp > 255)
	  temp = 255;
	//Threshold for differences from previous pixel
	if(total > 50)
	  total = 255;
	
	if(
	   (abs( src.at<Vec3b>(y,x)[0] - b) < abs(src.at<Vec3b>(y+1,x)[0] -b)) &&
	   (abs( src.at<Vec3b>(y,x)[1] - g) < abs(src.at<Vec3b>(y+1,x)[1] -g)) &&
	   (abs( src.at<Vec3b>(y,x)[2] - r) < abs(src.at<Vec3b>(y+1,x)[2] -r)) )
	  mask.at<Vec3b>(y,x)[1] = temp;
	else
	  {
	    mask.at<Vec3b>(y,x)[1] = temp;
	    mask.at<Vec3b>(y+1,x)[1] = temp;
	  }
	mask.at<Vec3b>(y,x)[2]=total;
      }
}


void paolMat::sweepMask()
{
  //Sweep down then up, then left, then right, filling in gaps smaller then thresh
  //if the color of the source in gap matched the color of the source at the mask
  
  cv::Mat temp;
  temp = src.clone();
  temp = Mat::zeros(src.size(),src.type());
  int thresh;
  thresh = 5;
  
  
  for(int x = 0; x < src.cols -1; x++)
    {
      int y = 0;
      int last = 0;
      while(y < src.rows-1)
	{
	  if(last == 0)
	    {
	      if(mask.at<Vec3b>(y,x)[2] > 20)
		last = 1;
	    }
	  else if((last > 0) && (last < thresh))
	    {
	      if( (
		   abs(src.at<Vec3b>(y-last,x)[0] - src.at<Vec3b>(y,x)[0])+
		   abs(src.at<Vec3b>(y-last,x)[1] - src.at<Vec3b>(y,x)[1])+
		   abs(src.at<Vec3b>(y-last,x)[2] - src.at<Vec3b>(y,x)[2]) )< 100 )
		{
		  for(int i = 0; i < last; i++)
		    {
		      //std::cout<<"Found something to sweep"<<std::endl;
		      mask.at<Vec3b>(y-last+i,x)[0] = 255;
		      mask.at<Vec3b>(y-last+i,x)[1] = 255;
		      mask.at<Vec3b>(y-last+i,x)[2] = 255; 
		    }
		  last = 0;
		}
	      else
		{
		  last++;
		}
	    }
	  
	  else if(last > thresh)
	    last = 0;

	  y++;
	}
    }
}

void paolMat::difference(Ptr<paolMat> img)
{
  for( int y = 0; y < img->src.rows; y++)
    for(int x = 0; x < img->src.cols; x++)
      {
	  mask.at<Vec3b>(y,x)[0] = abs( src.at<Vec3b>(y,x)[0] - img->src.at<Vec3b>(y,x)[0]);
	  mask.at<Vec3b>(y,x)[1] = abs( src.at<Vec3b>(y,x)[1] - img->src.at<Vec3b>(y,x)[1]);
	  mask.at<Vec3b>(y,x)[2] = abs( src.at<Vec3b>(y,x)[2] - img->src.at<Vec3b>(y,x)[2]);
      }
}

void paolMat::maskDifference(Ptr<paolMat> img)
{
  for( int y = 0; y < img->src.rows; y++)
    for(int x = 0; x < img->src.cols; x++)
      {
	  mask.at<Vec3b>(y,x)[0] = abs( mask.at<Vec3b>(y,x)[0] - img->mask.at<Vec3b>(y,x)[0]);
	  mask.at<Vec3b>(y,x)[1] = abs( mask.at<Vec3b>(y,x)[1] - img->mask.at<Vec3b>(y,x)[1]);
	  mask.at<Vec3b>(y,x)[2] = abs( mask.at<Vec3b>(y,x)[2] - img->mask.at<Vec3b>(y,x)[2]);
      }
}

void paolMat::intensityMask(int thresh)
{
  int last;
  int pTotal;

  for( int y = 0; y < src.rows-1; y++)
    {
      last = 0;
      for(int x = 0; x < src.cols-1; x++)
	{
	  pTotal = 0;
	  //Set the mask to black
	  	  
	  mask.at<Vec3b>(y,x)[0] = 
	    (
	     abs(src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y,x)[1]) +
	     abs(src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y,x)[2]) );
	  mask.at<Vec3b>(y,x)[1] = 
	    (
	     abs(src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y,x)[0]) +
	     abs(src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y,x)[2]) );
	  mask.at<Vec3b>(y,x)[2] = 
	    (
	     abs(src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y,x)[0]) +
	     abs(src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y,x)[1]) );

	  pTotal = ( mask.at<Vec3b>(y,x)[0] +
		     mask.at<Vec3b>(y,x)[1] +
		     mask.at<Vec3b>(y,x)[2] ); 
	  if( pTotal < thresh )
	    {
	      mask.at<Vec3b>(y,x)[0] = 0;
	      mask.at<Vec3b>(y,x)[0] = 0;
	      mask.at<Vec3b>(y,x)[0] = 0;
	    }
	  if( pTotal < thresh*2)
	    mask.at<Vec3b>(y,x)[0] = 255;
	  if( pTotal < thresh*3)
	    mask.at<Vec3b>(y,x)[1] = 150;
	  if( pTotal < thresh*4)
	    mask.at<Vec3b>(y,x)[1] = 255;
	  if( pTotal < thresh*5)
	    mask.at<Vec3b>(y,x)[2] = 150;
	  if( pTotal < thresh*6)
	    mask.at<Vec3b>(y,x)[2] = 255;
	}
    }
}

void paolMat::maskToWhite(int thresh)
{
  Ptr<paolMat> temp;
  temp = new paolMat(this);
  int red;
  int white;
  int yellow;
  for(int y = 4; y < src.rows-5; y++)
    for(int x = 4; x < src.cols-5; x++)
      {
	red = 0;
	white = 0;
	yellow = 0;
	for(int yy = y-5; yy < y +5; yy++)
	  for(int xx = x-5; xx < x+5; xx++)
	    {
	      if(mask.at<Vec3b>(yy,xx)[2] == 255)
		red++;
	      if( (mask.at<Vec3b>(yy,xx)[1] == 255) &&
		  (mask.at<Vec3b>(yy,xx)[2] == 255) )
		yellow++;
	      if( (mask.at<Vec3b>(yy,xx)[1] == 255) &&
		  (mask.at<Vec3b>(yy,xx)[2] == 255) &&
		  (mask.at<Vec3b>(yy,xx)[3] == 255) )
		white++;
	    }
	if(white > 255)
	  white = 255;
	temp->mask.at<Vec3b>(y,x)[0] = white;
	if(yellow > 255)
	  yellow = 255;
	temp->mask.at<Vec3b>(y,x)[1] = yellow;
	if(red > 255)
	  red = 255;
	temp->mask.at<Vec3b>(y,x)[2] = red;
      }
  for(int y = 0; y < 5; y++)
    for(int x = 0; x <src.cols; x++)
      {
	temp->mask.at<Vec3b>(y,x)[0] = 255;
	temp->mask.at<Vec3b>(y,x)[1] = 255;
	temp->mask.at<Vec3b>(y,x)[2] = 255;
      }
  for(int y = src.rows-5; y < src.rows; y++)
    for(int x = 0; x <src.cols; x++)
      {
	temp->mask.at<Vec3b>(y,x)[0] = 255;
	temp->mask.at<Vec3b>(y,x)[1] = 255;
	temp->mask.at<Vec3b>(y,x)[2] = 255;
      }
  for(int x = 0; x < 5; x++)
    for(int y = 0; y <src.rows; y++)
      {
	temp->mask.at<Vec3b>(y,x)[0] = 255;
	temp->mask.at<Vec3b>(y,x)[1] = 255;
	temp->mask.at<Vec3b>(y,x)[2] = 255;
      }
  for(int x = src.cols-5; x < src.cols; x++)
    for(int y = 0; y <src.rows; y++)
      {
	temp->mask.at<Vec3b>(y,x)[0] = 255;
	temp->mask.at<Vec3b>(y,x)[1] = 255;
	temp->mask.at<Vec3b>(y,x)[2] = 255;
      }
  
  int r,g;
  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      {
	/*
	if( (abs(temp->mask.at<Vec3b>(y,x)[0] - temp->mask.at<Vec3b>(y,x)[1]) +
	     abs(temp->mask.at<Vec3b>(y,x)[0] - temp->mask.at<Vec3b>(y,x)[2]) )
	    < 40)
	  {
	    mask.at<Vec3b>(y,x)[0] = 255;
	    mask.at<Vec3b>(y,x)[1] = 255;
	    mask.at<Vec3b>(y,x)[2] = 255;
	  }
	else
	  {
	*/
	    mask.at<Vec3b>(y,x)[0] = 2*temp->mask.at<Vec3b>(y,x)[0];
	    mask.at<Vec3b>(y,x)[1] = 2*temp->mask.at<Vec3b>(y,x)[1];
	    mask.at<Vec3b>(y,x)[2] = 2*temp->mask.at<Vec3b>(y,x)[2];
	    //}
      }
}

void paolMat::average()
{
  int pixels;
  pixels = src.rows * src.cols;
  for(int x = 0; x < src.cols; x++)
    for(int y = 0; y < src.rows; y++)
      {
	b += src.at<Vec3b>(y,x)[0];
	g += src.at<Vec3b>(y,x)[1];
	r += src.at<Vec3b>(y,x)[2];
      }
  b/=pixels;
  g/=pixels;
  r/=pixels;
  std::cout<<"Image average b: "<<b<<" g: "<<g<<" r: "<<r<<std::endl;
}

void paolMat::sweepDown()
{

  vector<int> hist;
  hist = vertMaskHistogram();
  Ptr<paolMat> temp;
  temp = new paolMat(this);
  temp->mask = Scalar(0,0,0);
  
  for(int x = 0; x < src.cols; x++)
    {
      bool hit;
      hit = false;
      for(int y = 0; y < src.rows; y++)
	{
	  if(mask.at<Vec3b>(y,x)[2] == 255)
	    hit = true;
	  
	  if(hit == true)
	    temp->mask.at<Vec3b>(y,x)[1] = 255;
	}
    }
  for(int y = 0; y < src.rows; y++)
    {
      bool hit;
      hit = false;
      for(int x = 0; x < src.cols; x++)
	{
	  if(mask.at<Vec3b>(y,x)[2] == 255)
	    hit = true;
	   if(hit == true)
	    temp->mask.at<Vec3b>(y,x)[0] = 255;
	}
    }

  for(int y = 0; y < src.rows; y++)
    {
      bool hit;
      hit = false;
      for(int x = src.cols-1; x >-1; x--)
	{
	  if(mask.at<Vec3b>(y,x)[2] == 255)
	    hit = true;
	   if(hit == true)
	    temp->mask.at<Vec3b>(y,x)[2] = 255;
	}
    }
   mask = temp->mask.clone();
   int pTotal;
   for(int x = 0; x < mask.cols; x++)
     if(hist[x] < 3)
       for(int y = 0; y < mask.rows; y++)
	 {
	   mask.at<Vec3b>(y,x)[0] = 0;
	   mask.at<Vec3b>(y,x)[1] = 0;
	   mask.at<Vec3b>(y,x)[2] = 0;
	 }
}

void paolMat::blackSrcByMask()
{
  int pTotal;
  for(int y = 0; y < mask.rows; y++)
    for(int x = 0; x < mask.cols; x++)
      {
	pTotal = (mask.at<Vec3b>(y,x)[0] +
		  mask.at<Vec3b>(y,x)[1] +
		  mask.at<Vec3b>(y,x)[2] );
	if(pTotal > 512)
	  {
	    src.at<Vec3b>(y,x)[0] = 0;
	    src.at<Vec3b>(y,x)[1] = 0;
	    src.at<Vec3b>(y,x)[2] = 0;
	  }
	
      }
  
}

 
void paolMat::blur(int size)
{
  int tempR, tempB, tempG, area;
  //blur size is pixels adjacent i.e. 1 would be a 3x3 square centered on each pixel
  area = (size *2+1)*(size *2+1);
  
  Ptr<paolMat> temp;
  temp = new paolMat(this);
  temp->src = Scalar(0,0,0);
  
  for(int y = size; y < mask.rows - size; y++)
    for(int x = size; x < mask.cols -size; x++)
      {
	tempB = 0;
	tempG = 0;
	tempR = 0;
	for(int yy = y-size; yy <= y+size; yy++)
	  for(int xx = x-size; xx <= x+size; xx++)
	    {
	      tempR+=src.at<Vec3b>(yy,xx)[2];
	      tempG+=src.at<Vec3b>(yy,xx)[1];
	      tempB+=src.at<Vec3b>(yy,xx)[0];
	    }
	tempR /=area;
	tempG /=area;
	tempB /=area;
	temp->src.at<Vec3b>(y,x)[2] = tempR;
	temp->src.at<Vec3b>(y,x)[1] = tempG;
	temp->src.at<Vec3b>(y,x)[0] = tempB;
	
      }
  src = temp->src.clone();
}

void paolMat::pDrift()
{
  int temp,total;
  int toggle,thresh;
  int redC,greenC,blueC,red,green,blue,back,forward;

  mask = Scalar(0,0,0);

  for(int y = 1; y < src.rows -1; y++)
    for(int x = 1; x < src.cols -1; x++)
      {
	temp = (
		//y,x+1
		abs(src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y,x+1)[0])+
		abs(src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y,x+1)[1])+
		abs(src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y,x+1)[2])+
		//y,x-1
		abs(src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y,x-1)[0])+
		abs(src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y,x-1)[1])+
		abs(src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y,x-1)[2])
		);
	if(temp > 255)
	  temp = 255;
	mask.at<Vec3b>(y,x)[2] = temp;
	total = temp;

	temp = (
		//y+1,x
		abs(src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y+1,x)[0])+
		abs(src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y+1,x)[1])+
		abs(src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y+1,x)[2])+
		//y-1,
		abs(src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y-1,x)[0])+
		abs(src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y-1,x)[1])+
		abs(src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y-1,x)[2])
		);
	if(temp > 255)
	  temp = 255;
	total+=temp;
	if(total > 255)
	  total = 255;
	mask.at<Vec3b>(y,x)[1] = temp;
	mask.at<Vec3b>(y,x)[0] = total;
      }
  
}

void paolMat::grow(int blueThresh, int size)
{
  for(int y = size; y < src.rows - size; y++)
    for(int x = size ; x < src.cols - size; x++)
      if(mask.at<Vec3b>(y,x)[0] > blueThresh)
	for(int yy = y-size; yy <= y+size;yy++)
	  for(int xx = x-size; xx <= x+size; xx++)
	    mask.at<Vec3b>(yy,xx)[2] = 255;
  
  for(int y = 0; y < src.rows; y++)
    for(int x = 0; x < src.cols; x++)
      mask.at<Vec3b>(y,x)[0] = mask.at<Vec3b>(y,x)[2];
  
}

void paolMat::shrink(int blueThresh, int size)
{
  Ptr<paolMat> temp;
  temp = new paolMat(this);
  temp->mask = Scalar(0,0,0);

  int total,area;
  area=(2*size+1)*(2*size+1);
  
  for(int y = size; y < src.rows - size; y++)
    for(int x = size ; x < src.cols - size; x++)
      if(mask.at<Vec3b>(y,x)[0] > blueThresh)
	{
	  total = 0;
	  for(int yy = y-size; yy<=y+size; yy++)
	    for(int xx = x-size; xx<=x+size; xx++)
	      if(mask.at<Vec3b>(yy,xx)[0] > blueThresh)
		total++;
	  if(total>=area-1)
	    temp->mask.at<Vec3b>(y,x)[0] = 255;
	}
  mask = temp->mask.clone();
}

//threshedDifference, only where both masks blue > 30
void paolMat::threshedDifference(Ptr<paolMat> old)
{
  int r,g,b, ave, difference;

  for(int y = 0; y < src.rows; y++)
    for(int x = 0; x < src.cols; x++)
      if(mask.at<Vec3b>(y,x)[0] > 30 && old->mask.at<Vec3b>(y,x)[0] > 30)
	{
	  b = abs(old->src.at<Vec3b>(y,x)[0] - src.at<Vec3b>(y,x)[0]);
	  g = abs(old->src.at<Vec3b>(y,x)[1] - src.at<Vec3b>(y,x)[1]);
	  r = abs(old->src.at<Vec3b>(y,x)[2] - src.at<Vec3b>(y,x)[2]);
	  
	  if(b+g+r > 40)
	    {
	      mask.at<Vec3b>(y,x)[1] = 255;
	      mask.at<Vec3b>(y,x)[2] = 0;	      
	    }
	  else
	    {
	      mask.at<Vec3b>(y,x)[1] = 0;
	      mask.at<Vec3b>(y,x)[2] = 255;
	    }
	    	  
	}
      else
	{
	  mask.at<Vec3b>(y,x)[1] = 0;
	  mask.at<Vec3b>(y,x)[2] = 0;
	}
}

void paolMat::getCombine(Ptr<paolMat> img)
{
  for(int y = 0; y < mask.rows; y++)
    for(int x = 0; x < mask.cols; x++)
      if(img->mask.at<Vec3b>(y,x)[0] != 0 )
	src.at<Vec3b>(y,x) = img->src.at<Vec3b>(y,x);
}

void paolMat::blackMaskByMask(Ptr<paolMat> img)
{
  for(int y = 0; y < mask.rows; y++)
    for(int x = 0; x < mask.cols; x++)
      if( img->mask.at<Vec3b>(y,x)[0] +
	  img->mask.at<Vec3b>(y,x)[1] +
	  img->mask.at<Vec3b>(y,x)[2] != 0 )
	{
	  mask.at<Vec3b>(y,x)[2] = 0;
	}
}

void paolMat::updateBackground(Ptr<paolMat> alt, Ptr<paolMat> img)
{
  for(int y = 0; y < mask.rows; y++)
    for(int x = 0; x < mask.cols; x++)
      if( 
	 alt->mask.at<Vec3b>(y,x)[0] +
	 alt->mask.at<Vec3b>(y,x)[1] +
	 alt->mask.at<Vec3b>(y,x)[2] == 0 )
	{
	  if(img->mask.at<Vec3b>(y,x)[2] != 0)
	    src.at<Vec3b>(y,x) = img->src.at<Vec3b>(y,x);
	  else if(img->mask.at<Vec3b>(y,x)[1] +
		  img->mask.at<Vec3b>(y,x)[2] == 0)
	    {
	      src.at<Vec3b>(y,x)[0] = 255;
	      src.at<Vec3b>(y,x)[1] = 255;
	      src.at<Vec3b>(y,x)[2] = 255;
	    }
	  mask.at<Vec3b>(y,x)[0] = 0;
	  mask.at<Vec3b>(y,x)[1] = 0;
	  mask.at<Vec3b>(y,x)[2] = 0;
	}
      else
	{
	  mask.at<Vec3b>(y,x)[0] = 255;
	}
  
}

void paolMat::cleanBackground(Ptr<paolMat> img)
{
  Ptr<paolMat> result;
  result = new paolMat(img);
  result->src = Scalar(255,255,255);
  result->mask = Scalar(0,0,0);

  int start,sim,dif,r,g,b,temp,tempOld,wr,wg,wb,rOff,gOff,bOff,oRange,range,rOut,gOut,bOut;

  for(int y = 0; y < src.rows; y++)
    for(int x = 0; x < src.cols; x++)
      if( src.at<Vec3b>(y,x)[0] !=255 ||
	  src.at<Vec3b>(y,x)[1] !=255 ||
	  src.at<Vec3b>(y,x)[2] !=255 )
	{
	  start = x;
	  sim = 1000;
	  dif = 0;
	  for(;x < width && ( src.at<Vec3b>(y,x)[0] !=255 ||
			      src.at<Vec3b>(y,x)[1] !=255 ||
			      src.at<Vec3b>(y,x)[2] !=255 ); x++)
	    {
	      end = x;
	      r = img->src.at<Vec3b>(y,x)[2];
	      g = img->src.at<Vec3b>(y,x)[1];
	      b = img->src.at<Vec3b>(y,x)[0];
	      temp = 255 * 3 -(r+g+b);
	      temp /= 3;
	      if(temp < sim)
		sim = temp;
	      if(temp > dif)
		dif = temp;
	    }
	  tempOld = ( img->src.at<Vec3b>(y,start)[0] +
		      img->src.at<Vec3b>(y,start)[1] +
		      img->src.at<Vec3b>(y,start)[2] );
	  temp = ( img->src.at<Vec3b>(y,end)[0] +
		   img->src.at<Vec3b>(y,end)[1] +
		   img->src.at<Vec3b>(y,end)[2] );
	  
	  if(temp > tempOld)
	    {
	      wr = img->src.at<Vec3b>(y,start)[2];
	      wg = img->src.at<Vec3b>(y,start)[1];
	      wb = img->src.at<Vec3b>(y,start)[0];	      
	    }
	  else
	    {
	      wr = img->src.at<Vec3b>(y,end)[2];
	      wg = img->src.at<Vec3b>(y,end)[1];
	      wb = img->src.at<Vec3b>(y,end)[0];	      
	    }
	  rOff = 255 - wr;
	  gOff = 255 - wg;
	  bOff = 255 - wb;

	  temp = 255 * 3 - (wr+wg+wb);
	  temp /=3;

	  oRange = abs(temp-dif);
	  range = oRange+temp;

	  for(int xx = start; xx <= end; xx++)
	    {
	      if(oRange == 0)
		{
		  rOut = 0;
		  gOut = 0;
		  bOut = 0;
		}
	      else
		{
		  rOut = ( (255 - img->src.at<Vec3b>(y,xx)[2]) - rOff) * range/oRange;
		  gOut = ( (255 - img->src.at<Vec3b>(y,xx)[1]) - gOff) * range/oRange;
		  bOut = ( (255 - img->src.at<Vec3b>(y,xx)[0]) - bOff) * range/oRange;

		  if(rOut > 255)
		    rOut = 255;
		  else if(rOut < 0)
		    rOut = 0;

		  if(gOut > 255)
		    gOut = 255;
		  else if(gOut < 0)
		    gOut = 0;

		  if(bOut > 255)
		    bOut = 255;
		  else if(bOut < 0)
		    bOut = 0;		
		}
	      result->src.at<Vec3b>(y,xx)[0] = 255 - bOut;
	      result->src.at<Vec3b>(y,xx)[1] = 255 - gOut;
	      result->src.at<Vec3b>(y,xx)[2] = 255 - rOut;
	    }
	}
  //RESUME HERE
}
