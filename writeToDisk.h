// ; -*-C++-*-
#ifndef _writeToDisk_h_
#define _writeToDisk_h_

using namespace cv;

class WriteToDisk: public Consumer
{
public:
  WriteToDisk(Buffer* inBuffer, std::string basename, std::string dirIn);
  ~WriteToDisk();
  void run();
private:
  char baseName[256];
  char dir[256];
};

class WriteMovie: public Consumer
{
public:
  WriteMovie(Buffer* in, std::string dest, int fpsIn);
  void run();
  char* destName;
  int fps;
};

class WriteDebugMovie: public Consumer
{
public:
  WriteDebugMovie(Buffer* in, std::string dest, int fpsIn);
  void run();
  char* destName;
  int fps;
};

#endif
