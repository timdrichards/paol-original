// ; -*-C++-*-
#ifndef _writeToDisk_h_
#define _writeToDisk_h_

using namespace cv;

class WriteToDisk: public Consumer
{
public:
  WriteToDisk(Buffer* inBuffer, std::string basename, std::string dirIn);
  void run();
private:
  char* baseName;
  char* dir;
};

class WriteMovie: public Consumer
{
public:
  WriteMovie(Buffer* in, std::string dest, int fpsIn);
  void run();
  char* destName;
  int fps;
};

#endif
