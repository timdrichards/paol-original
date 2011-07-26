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

};

paolMat::paolMat(Ptr<paolMat> m)
{
  
  src = m->src.clone();
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

/*namespace cv
{
  template<> inline void Ptr<paolMat>::delete_obj()
  {
    obj->~paolMat();
  };
  };*/
