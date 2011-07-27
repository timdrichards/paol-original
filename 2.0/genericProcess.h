// ; -*-C++-*-
#ifndef _genericProcess_h_
#define _genericProcess_h_

using namespace cv;

class GenericProcess : public Module
{
 public:
 GenericProcess(Buffer* in, Buffer* out) : Module(in,out, 150){};
  void run();

};

#endif
