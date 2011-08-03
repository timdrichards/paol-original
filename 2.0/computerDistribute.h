// ; -*-C++-*-
#ifndef _computerDistribute_h_
#define _computerDistribute_h_

using namespace cv;

class ComputerDistribute : public Module
{
 public:
  ComputerDistribute(Buffer* in, Buffer* out) : Module(in,out,150){};
  void run();

};

#endif
