// ; -*-C++-*-
#ifndef _webCamCapture_h_
#define _webCamCapture_h_

using namespace cv;

class WebCamCapture : public Module
{
 public:
 WebCamCapture(Buffer* out) : Module(NULL,out, 150){};
  void run();
  
};

#endif
