// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
#include <cstdio>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
//Our Libs
#include "paolMat.h"
#include "buffer.h"
#include "module.h"
#include "genericProcess.h"
#include "computerDistribute.h"
#include "computerProcess.h"

void computerPipeline(char* outDir, char* dir, char* first)
{

  char out[1024];
  char vout[1024];
  char Cdir[1024];
  char firstSlide[1024];
  char Cname[1024];
  
  std::strcpy(out, outDir);
  std::strcpy(vout, outDir);
  std::strcat(out, "screen/");
  std::strcat(vout, "computerVideoFrames/");
  std::strcpy(Cdir, dir);
  std::strcpy(firstSlide, first);
  std::strcat(Cname, "computer");

  std::cout<<outDir<<" "<<dir<<" "<<first<<std::endl;
  Buffer* readBuffer;
  readBuffer = new Buffer;
  Buffer* tempWriterBuffer;
  tempWriterBuffer = new Buffer;
  Buffer* compSlidesProcBuffer;
  compSlidesProcBuffer = new Buffer;
  Buffer* compMovieWriterBuffer;
  compMovieWriterBuffer = new Buffer;
  Buffer* compSlidesWriterBuffer;
  compSlidesWriterBuffer = new Buffer;

  WriteMod compMovieWriter(compMovieWriterBuffer);
  Module compTemp(tempWriterBuffer, NULL, 15);
  WriteMod compSlidesWriter(compSlidesWriterBuffer);
  ComputerProcess compSlidesProc(compSlidesProcBuffer, compSlidesWriterBuffer);
  ComputerDistribute compDistribute(readBuffer, compSlidesProcBuffer, compMovieWriterBuffer, tempWriterBuffer);
  ReadMod compReader(readBuffer);

  boost::thread_group compPipeline;

  compPipeline.create_thread(boost::bind(&WriteMod::WriteMatsByCount, &compMovieWriter, vout));
  compPipeline.create_thread(boost::bind(&Module::nullRun, &compTemp));
  compPipeline.create_thread(boost::bind(&WriteMod::WriteSlides, &compSlidesWriter, out, Cname));
  compPipeline.create_thread(boost::bind(&ComputerProcess::run, &compSlidesProc));
  compPipeline.create_thread(boost::bind(&ComputerDistribute::run, &compDistribute));
  compPipeline.create_thread(boost::bind(&ReadMod::ReadFromPatternComp, &compReader, Cdir, firstSlide));

  compPipeline.join_all();
  
  delete readBuffer;
  delete tempWriterBuffer;
  delete compSlidesProcBuffer;
  delete compMovieWriterBuffer;
  delete compSlidesWriterBuffer;
}
