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

#endif
