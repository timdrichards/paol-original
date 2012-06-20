// ; -*-C++-*-
#ifndef _ac_h_
#define _ac_h_

using namespace cv;

class AC : public Module
{
 public:
  AC(Buffer* in, Buffer* out) : Module(in,out, 150){};
  void run();
  
};

#endif
