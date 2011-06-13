// ; -*-C++-*-
#ifndef producer_h_
#define producer_h_

class Producer
{


  
public:
  Buffer *proBuffer;
  void push(paolMat img);
  void start(Buffer *buffer);
  void stop();
  Producer(){killMe = false;};
  bool keepRunning();

private:
  bool killMe;
  boost::mutex stoplock;


};

class ReadFromDisk: public Producer
{
public:
  void run();
  void readDir();
  void readFromPattern(char *dir, char* firstImage);
};

#endif
