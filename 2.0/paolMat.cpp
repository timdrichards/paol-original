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

paolMat::paolMat(paolMat* m)
{
  
  src = m->src.clone();
  for(int i = 0; i < m->planes.size(); i++)
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
  for(int i = 0; i < planes.size(); i++)
    {
      planes[i].~Mat();
    };
  camera.~Point();
  prof.~Point();
};
