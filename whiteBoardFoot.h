// ; -*-C++-*-
#ifndef _whiteBoardFoot_h_
#define _whiteBoardFoot_h_

using namespace cv;

class WhiteBoardFoot : public Module
{
 public:
  WhiteBoardFoot(Buffer* in, Buffer* out) : Module(in,out,15){};
  void run();

};

#endif
