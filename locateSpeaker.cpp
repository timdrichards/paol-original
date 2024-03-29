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
#include "genericProcess.h"
#include "locateSpeaker.h"

//#define _debug_

using namespace cv;

void LocateSpeaker::run()
{
  Ptr<paolMat> img;
  Ptr<paolMat> cleanImg;
  Ptr<paolMat> bgImg;
  Ptr<paolMat> lastImg;

  lastImg = pop();
  
  if(lastImg != NULL)
    {
      img = pop();
      cleanImg = new paolMat(img);
      
      while(img != NULL)
	{
	  cleanImg->copy(img);
	  //bgImg = img->returnCreateBackgroundImg(25);
	  img->differenceLect(lastImg,150,1);
	  img->name="differenceLect";
	  //img->writeMask();
	  //img->improveInputImg(bgImg);
	  
	  
	  //img->connected(10);
#ifdef _debug_
	  //img->name="connectedDiffSpeaker";
	  //img->writeMask();
#endif
	  //img->localizeSpeaker();
#ifdef _debug_
	  //img->name="localizeSpeaker";
	  //img->writeMask();
	  img->name = "Speaker";
	  //img->write();
#endif
	  
	  //img->camera = img->prof;
	  //img->cropFrame(640,480);
	  
	  cleanImg->copyNoSrc(img);
	  push(cleanImg);
	  
	  lastImg = img;
	  std::cout<<"LocateSpeaker:: Frame :"<<img->count<<std::endl;
	  img = pop();
	}
    }
  stop();
}

