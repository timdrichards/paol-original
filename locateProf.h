#ifndef _locateProf_h_
#define _locateProf_h_

class locationData
{
 public:
  locationData(paolMat in, double scale);
  paolMat big;
  paolMat small;
};

class LocateProf : public Processor
{

 public:
  void run();
  std::vector<locationData> frameBuffer;
  int frameBufferSize;
  void setup(int size, int scale);
  int current;
  int newest;
  bool newFrame();
  double scale;
  paolMat difference;
  void findProf();
  void createDifference();
  
 LocateProf(Buffer* in, Buffer* out) : Processor(in,out){};

};


class test : public Processor
{
 public:
  void run();
 test(Buffer* in, Buffer* out) : Processor(in,out){};
  
};

class Accumulate : public Processor
{
 public:
  paolMat outImg;
  void run();
  void setup(int bufferSizeIn);
  int bufferSize;
  int current;
  void addPaol(paolMat inImg);
  void subtractPaol();
  void updateOutput();
  void insert();
  bool keepgoing;
  std::vector<paolMat> buffer;
  //std::vector<std::vector<std::vector<int> > > big;
 Accumulate(Buffer* in, Buffer* out) : Processor(in,out){};
 
};

#endif
