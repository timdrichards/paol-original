// ; -*-C++-*-
#ifndef _gige_h_
#define _gige_h_

class CameraCap : public Producer
{

public:
  int cam;
  void run();
  
  CameraCap(Buffer *buffer, int incam);

};



#endif
