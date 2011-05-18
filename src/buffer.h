///////BUFFER CLASS///////////

using namespace cv;
using namespace boost;

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
  void push(cv::Mat frame);
  cv::Mat pop();

 FrameLinkedList() : size(0) {};

 private:
  boost::mutex listLock;
    
};

class Buffer
{
 public:
  int registerConsumer();
  void push(cv::Mat frame);
  cv::Mat pop();
  void run();
 private:
  boost::mutex bufferLock;
  std::vector<FrameLinkedList> consumerLists;

};
