// ; -*-C++-*-
#ifndef _computerProcess_h_
#define _computerProcess_h_

using namespace cv;

class ComputerProcess : public Module
{
 public:
  ComputerProcess(Buffer* in, Buffer* out) : Module(in,out,150){};
  void run();

};

#endif
