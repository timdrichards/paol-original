// ; -*-C++-*-
#ifndef _module_h_
#define _module_h_

using namespace cv;

class Module
{
 public:
  Buffer* input;
  Buffer* output;
  int myID;
  bool consumer;
  bool producer;
  bool keepRunning;
  boost::mutex modLock;

  Module(Buffer* in, Buffer* out, int arraySize);
  ~Module();

  paolMat* pop();
  int push(paolMat* m);
  void stop();
  bool running();
  void nullRun();
};

#endif
