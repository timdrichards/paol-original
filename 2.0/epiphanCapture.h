// ; -*-C++-*-
#ifndef _epiphanCapture_h_
#define _epiphanCapture_h_

using namespace cv;

class EpiphanCapture : public Module
{
 public:
 EpiphanCapture(Buffer* out) : Module(NULL,out, 150){};
  void run();
  void runEthCap();
  
};

#endif
