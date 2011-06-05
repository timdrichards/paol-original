// ; -*-C++-*-
#ifndef producer_h_
#define producer_h_

class Producer
{
  
public:
  Buffer *myBuffer;
  void push(cv::Mat img);
  void start(Buffer *buffer);
  void stop();
  Producer(){killMe = false;};
  bool keepRunning();

private:
  bool killMe;
  boost::mutex stoplock;


};

class readFromDisk: public Producer
{
public:
  void run();
};

#endif
