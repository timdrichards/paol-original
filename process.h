#ifndef _process_h_
#define _process_h_

using namespace cv;
using namespace boost;

class Processor : public Producer, public Consumer
{
 private:
  bool killMe;
  boost::mutex proLock;
  bool keepRunning();
  Buffer* input;
  Buffer* output;
  int myID;
  cv::Mat pop();

 public:
  Processor(Buffer* in, Buffer* out);
  void run();
  void stop();
};


#endif