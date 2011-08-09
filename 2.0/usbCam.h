// ; -*-C++-*-
#ifndef _usbCam_h_
#define _usbCam_h_

using namespace cv;

class UsbCam : public Module
{
 public:
 UsbCam(Buffer* out) : Module(NULL,out, 150){};
  void run();
  
};

#endif
