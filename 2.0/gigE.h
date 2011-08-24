// ; -*-C++-*-
#ifndef _gigE_h_
#define _gigE_h_

#include <PvApi.h>

using namespace cv;

typedef struct 
{
    unsigned long   UID;
    tPvHandle       Handle;
    tPvFrame        Frame;

} tCamera;



class GigE : public Module
{
 public:
  GigE(Buffer* out, int camIndex) : Module(NULL,out, 150){};
  void run();
  
};

#endif
