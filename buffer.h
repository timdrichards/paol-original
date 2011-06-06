// ; -*-C++-*-
#ifndef _buffer_h_
#define _buffer_h_

///////BUFFER CLASS///////////

using namespace cv;


//List Item for buffer linked list//
struct frameListItem
{
  cv::Mat frame;
  frameListItem* next;
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
  void push(cv::Mat frame);
  cv::Mat pop();
  void stop(){producerRunning=false;std::cout<<"FrameLinkedList:: STOP CALLED STOP CALLED"<<std::endl;};
  
  FrameLinkedList();
  
private:
  boost::mutex listLock;
  
  
};

class Buffer
{
 public:
  int registerConsumer();
  void push(cv::Mat frame);
  cv::Mat pop(int consumerID);
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
