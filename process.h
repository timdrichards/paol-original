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
  //Buffer* input;
  //Buffer* output;
  int myID;
  paolMat pop();
  Processor(Buffer* in, Buffer* out);// : Producer(in) , Consumer(out);
  void run();
  void stop();
  void passOn()
  {
    paolMat img;
    img = pop();
    while (img.src.data)
      {
	conBuffer->push(img);
	img = proBuffer->pop(myID);
      };
    paolMat nullImg;
    conBuffer->push(nullImg);
    conBuffer->stop();
  };

};


#endif
