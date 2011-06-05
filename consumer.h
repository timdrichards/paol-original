// ; -*-C++-*-
#ifndef _consumer_h_
#define _consumer_h_

using namespace cv;

class Consumer
{
public:

  Buffer *myBuffer;
  int myID;
  void start(Buffer *buffer);
  cv::Mat pop();
  void stop();
  Consumer(){killMe = false; myID = -1;};
  bool keepRunning();

private:
  bool killMe;
  boost::mutex stoplock;

};

//Demo read from disk class//

class imWindow: public Consumer
{
public:
  void run();
  
};

#endif
