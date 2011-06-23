// ; -*-C++-*-
#ifndef _buffer_h_
#define _buffer_h_



///PaolMat////

using namespace cv;

class paolMat
{
public:
  cv::Mat src;
  cv::vector<cv::Mat> planes;
  int count;
  int time;
  std::string name;
  cv::Point prof;
  cv::Point camera;
  void print();
  void copy(paolMat m);
  void read(std::string fileName,int countIn,int timeIn);
  void edges();
  //This is a slow method for testing, not production//
  void invert();
  void split(){cv::split(src, planes);};
  void merge();
  void createBackgroundImg(int kernalSize);
  void improveInputImg(paolMat background);
  void removeProf();
  void createContrast();
  void sharpen();
  void shrink();


  paolMat();
  ~paolMat();
  
  //  paolMat operator = (const paolMat& m);
};

///////BUFFER CLASS///////////

//List Item for buffer linked list//
class frameListItem
{
public:
  paolMat frame;
  frameListItem* next;
  frameListItem(paolMat newFrame);
  ~frameListItem();
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
  ~FrameLinkedList();
  
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
  ~Buffer();
 private:
  boost::mutex bufferLock;
  std::vector<FrameLinkedList*> consumerLists;
 
  
};



#endif
