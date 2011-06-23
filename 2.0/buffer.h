// ; -*-C++-*-
#ifndef _buffer_h_
#define _buffer_h_

using namespace cv;

//List of Paol Mats to be used by buffer//
class PMList
{
 public:
  std::vector<paolMat*> PaolMats;
  int push(paolMat* inPM);
  paolMat* pop();
  void stop();
  int current;
  int oldest;
  int size;
  
  PMList();
  ~PMList();
  
 private:
  bool producerRunning;
  boost::mutex listLock;
};

class Buffer
{
 public:
  int push(paolMat* inPM);
  paolMat* pop(int id);
  void stop();
  int registerConsumer();

  Buffer();
  ~Buffer();

 private:
  std::vector<PMList*> consumerLists;
  boost::mutex bufferLock;
};

#endif
