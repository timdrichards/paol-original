// ; -*-C++-*-
#ifndef _consumer_h_
#define _consumer_h_

using namespace cv;

class Consumer
{
public:

  Buffer *conBuffer;
  int myID;
  Consumer(Buffer *buffer);
  paolMat* pop();
  void stop();
  bool keepRunning();

private:
  bool killMe;
  boost::mutex stoplock;

};

#endif
