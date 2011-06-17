// ; -*-C++-*-
#ifndef producer_h_
#define producer_h_

class Producer
{


  
public:
  Buffer *proBuffer;
  void push(paolMat img);
  void stop();
  Producer(Buffer *buffer);
  bool keepRunning();
  
private:
  bool killMe;
  boost::mutex stoplock;


};

class ReadFromDisk: public Producer
{
public:
  char* dirIn;
  char* firstImageIn;
  ReadFromDisk(Buffer *buffer, char *dir, char* firstImage) : Producer(buffer){dirIn = dir; firstImageIn = firstImage;};
  void run();
  void readDir();
  void readFromPattern(char *dir, char* firstImage);
};

#endif
