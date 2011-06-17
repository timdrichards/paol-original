#ifndef _locateProf_h_
#define _locateProf_h_



class LocateProf : public Processor
{

 public:
  void run();
  
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
