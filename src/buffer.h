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
  frameListItem* first;
  frameListItem* last;
  int size;
  void push(cv::Mat frame);
  cv::Mat pop();
 private:
  boost::mutex::scoped_lock listLock;
  
};

class Buffer
{
 public:
  int registerConsumer();
  void push(cv::Mat frame);
  cv::Mat pop();
  void run();
 private:
  boost::mutex::scoped_lock bufferLock;
  std::vector<FrameLinkedList> consumerLists;

};
