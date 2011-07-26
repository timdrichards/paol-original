// ; -*-C++-*-
#ifndef _buffer_h_
#define _buffer_h_

using namespace cv;

//List of Paol Mats to be used by buffer//
class PMList
{
 public:
  std::vector< Ptr<paolMat> > PaolMats;
  int current;
  int oldest;
  int size;
  
  PMList();
  ~PMList();
  int push(Ptr<paolMat> inPM);
  Ptr<paolMat> pop();
  void stop();
  void print();

 private:
  bool producerRunning;
  boost::mutex listLock;
};

class Buffer
{
 public:
  std::vector< Ptr<PMList> > consumerLists;
  
  Buffer();
  ~Buffer();
  
  int push(Ptr<paolMat> inPM);
  Ptr<paolMat> pop(int id);
  void stop();
  int registerConsumer();
  void print();
 private:
  
  boost::mutex bufferLock;
};

#endif
