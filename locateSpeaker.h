// ; -*-C++-*-
#ifndef _LocateSpeaker_h_
#define _LocateSpeaker_h_

using namespace cv;

class LocateSpeaker : public Module
{
 public:
 LocateSpeaker(Buffer* in, Buffer* out) : Module(in,out, 150){};
  void run();
  
};

#endif
