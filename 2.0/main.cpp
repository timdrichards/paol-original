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
#include "opencv/cvaux.h"
//Our Libs

#include "paolMat.h"
#include "buffer.h"
#include "module.h"

using namespace cv;

int main(int arc, char** argv)
{
  paolMat* img;
  img = new paolMat;
  img->src = imread("../test.jpg");
  img->name = "Bob";
  img->count = 1;
  img->time = 1;

  paolMat* newImg;
  newImg = new paolMat(img);


  Buffer* testBuffer;
  testBuffer = new Buffer;

  Buffer* secondBuffer;
  secondBuffer = new Buffer;

  //testBuffer->registerConsumer();
  

  Module mod(testBuffer, secondBuffer, 150);

  testBuffer->push(newImg);
  testBuffer->push(img);

  testBuffer->stop();
  mod.nullRun();

  delete img;
  delete newImg;
  std::cout<<"Images gone"<<std::endl;
  delete testBuffer;
  delete secondBuffer;
  std::cout<<"Buffer deleted"<<std::endl;
  return 0;

};

/*
 Buffer aBuffer;
  aBuffer.registerConsumer();
  aBuffer.registerConsumer();
  std::cout<<"Debug 0.0"<<std::endl;
  aBuffer.push(img);
  std::cout<<"Debug 0.1"<<std::endl;
  aBuffer.push(newImg);
  std::cout<<"Debug 0.2"<<std::endl;
  aBuffer.stop();
  std::cout<<"Debug 0.3"<<std::endl;
  delete img;
  std::cout<<"Debug 0.4"<<std::endl;
  //delete newImg;
  std::cout<<"Debug 0.5"<<std::endl;

  paolMat imgd(aBuffer.pop(0));
  std::cout<<"Debug 0.51"<<std::endl;
  imgd.~paolMat();
  std::cout<<"Debug 0.52"<<std::endl;
  paolMat imgc(aBuffer.pop(0));
  std::cout<<"Debug 0.53"<<std::endl;
  imgc.~paolMat();
  std::cout<<"Debug 0.54"<<std::endl;
  paolMat imgb(aBuffer.pop(1));
  std::cout<<"Debug 0.55"<<std::endl;
  imgb.~paolMat();
  std::cout<<"Debug 0.56"<<std::endl;
  paolMat imgB(aBuffer.pop(1));
  


  std::cout<<"Debug 0.6"<<std::endl;
  imwrite("image1a.jpg", imgB.src);
  std::cout<<"Debug 0.7"<<std::endl;
  imgB.~paolMat();
  // imwrite("image1b.jpg", aBuffer.pop(0)->src);

  //imwrite("image2a.jpg", aBuffer.pop(1)->src);
  //imwrite("image2b.jpg", aBuffer.pop(1)->src);

  */
