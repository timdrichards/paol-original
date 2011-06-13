#ifndef _process_h_
#define _process_h_

using namespace cv;
using namespace boost;

class Processor : public Producer, public Consumer
{
 private:
  bool killMe;
  boost::mutex proLock;
 
  public:
  bool keepRunning();
  Buffer* input;
  Buffer* output;
  int myID;
  cv::Mat pop();
  Processor(Buffer* in, Buffer* out);
  void run();
  void stop();
  void passOn()
  {
    cv::Mat img;
    img = pop();
    while (img.data)
      {
	output->push(img);
	img = pop();
      };
    cv::Mat nullImg;
    output->push(nullImg);
    output->stop();
  };

};


#endif
