// ; -*-C++-*-
#ifndef _whiteBoard_h_
#define _whiteBoard_h_

using namespace cv;

class WhiteBoardProcess : public Module
{
 public:
  WhiteBoardProcess(Buffer* in, Buffer* out) : Module(in,out,150){};
  void run(int skip);

};

#endif
