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
  //src.create(1,1,1);
  name = "";
  count = -1;
  time = -1;
};

paolMat::paolMat(paolMat* m)
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
  //src.~Mat();
  for(int i = 0; i < (int)planes.size(); i++)
    {
      planes[i].~Mat();
    };
  camera.~Point();
  prof.~Point();
};

void paolMat::read(std::string fileName,int countIn,int timeIn)
{
  src = imread(fileName);
  name = fileName;
  count = countIn;
  time = timeIn;
};
