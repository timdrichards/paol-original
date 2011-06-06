// ; -*-C++-*-
#ifndef _writeToDisk_h_
#define _writeToDisk_h_

using namespace cv;

class WriteToDisk: public Consumer
{
public:
  void setup(char* basename, char* dirIn);
  void run();
private:
  char* baseName;
  char* dir;
};

#endif
