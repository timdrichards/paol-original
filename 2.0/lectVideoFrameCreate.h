// ; -*-C++-*-
#ifndef _lectVideoFrameCreate_h_
#define _lectVideoFrameCreate_h_

using namespace cv;

class LectVideoFrameCreate : public Module
{
 public:
 LectVideoFrameCreate(Buffer* in, Buffer* out) : Module(in,out, 150){};
  void run();
  
};

#endif
