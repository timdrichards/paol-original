// ; -*-C++-*-
#ifndef _buffer_h_
#define _buffer_h_

///////BUFFER CLASS///////////

using namespace cv;

class paolMat
{
public:
  cv::Mat src;
  int count;
  int time;
  std::string name;
  void print();
  void copy(paolMat m);
  void read(std::string fileName,int countIn,int timeIn);
  
  paolMat();
  
  //  paolMat operator = (const paolMat& m);
};

//List Item for buffer linked list//
class frameListItem
{
public:
  paolMat frame;
  frameListItem* next;
  frameListItem(paolMat newFrame);
};

//Linked list of frames (the buffer will hold one list for each consumer)//
class FrameLinkedList
{
 public:
  frameListItem* oldest;
  frameListItem* newest;
  int size;
private:
  bool producerRunning;
public:
  void push(paolMat frame);
  paolMat pop();
  void stop(){producerRunning=false;std::cout<<"FrameLinkedList:: STOP CALLED STOP CALLED"<<std::endl;};
  
  FrameLinkedList();
  
private:
  boost::mutex listLock;
  
  
};

class Buffer
{
 public:
  int registerConsumer();
  void push(paolMat frame);
  paolMat pop(int consumerID);
private:
   bool producerRunning;
public:
  void stop();
  Buffer();
 private:
  boost::mutex bufferLock;
  std::vector<FrameLinkedList*> consumerLists;
 
  
};



#endif
