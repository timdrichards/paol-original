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
  paolMat pop();
  Processor(Buffer* in, Buffer* out);
  void run();
  void stop();
  void passOn()
  {
    paolMat img;
    img = pop();
    while (img.src.data)
      {
	output->push(img);
	img = input->pop(myID);
      };
    paolMat nullImg;
    output->push(nullImg);
    output->stop();
  };

};


#endif
