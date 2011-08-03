// ; -*-C++-*-
#ifndef _computerDistribute_h_
#define _computerDistribute_h_

using namespace cv;

class ComputerDistribute : public Module
{
 public:
  ComputerDistribute(Buffer* in, Buffer* compProcIn, Buffer* compMovieIn, Buffer* tempDisplayIn);
  void run();
  
  Buffer* compProc;
  Buffer* compMovie;
  Buffer* tempDisplay;
};

#endif
