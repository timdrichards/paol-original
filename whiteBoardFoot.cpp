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
#include "buffer.h"
#include "module.h"
#include "whiteBoardFoot.h"

#define _debug_

using namespace cv;


void WhiteBoardFoot::run()
{
  Ptr<paolMat> temp;
  temp = new paolMat();
  temp = pop();
  

  Ptr<paolMat> oldImg;
  oldImg = new paolMat;
  oldImg = temp->crop(400,1400,3600,1356);

  Ptr<paolMat> tempOld;
  tempOld = new paolMat();
  tempOld = temp->crop(400,1400,3600,1356);
  
  Ptr<paolMat> img;
  img = new paolMat();
  img->copy(oldImg);
  img->name = "newImg";
  
  while(img != NULL)
    {      
      tempOld->differenceDarken(img);
      tempOld->name = "Foot-WDEH";
      tempOld->writeMask();
      tempOld->maskGrowRed(20);
      tempOld->name = "Foot-redGrow";
      tempOld->writeMask();
      tempOld->countDiffsMasked(oldImg);
      std::cout<<"Foot:: "<<tempOld->difs<<" difs"<<std::endl;
      //if we are making a slide
      if(tempOld->difs > 1000)
	{
	  img->copyNoSrc(temp);
	  oldImg->copy(tempOld);
	  oldImg->name = "foot-differenceDarken";
	  oldImg->write();
	}
      temp = pop();
      img = temp->crop(400,1400,3600,1356);
      tempOld = temp->crop(400,1400,3600,1356);
    }
      
  stop();
}

