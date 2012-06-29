// ; -*-C++-*-
#ifndef _module_h_
#define _module_h_

using namespace cv;

class Module
{
 public:
  Buffer* input;
  Buffer* output;
  int myID;
  bool consumer;
  bool producer;
  bool keepRunning;
  boost::mutex modLock;

  Module(Buffer* in, Buffer* out, int arraySize);
  ~Module();

  Ptr<paolMat> pop();
  int push(Ptr<paolMat> m);
  void stop();
  bool running();
  void nullRun();
};

class ReadMod : public Module
{
 public:
  ReadMod(Buffer* out) : Module(NULL, out, 150){};
  //~ReadMod();
  void ReadFromPattern(char* dir, char* firstImg);
  void ReadFromPatternComp(char* dir, char* firstImg);
  //For reading in upside down images
  void ReadFromPatternFlip(char* dir, char* firstImg);
  void ReadFromPatternFlipExt(char* dir, char* firstImg, char* pattern);
  void ReadFromPatternFlipExtCrop(char* dir, char* firstImg, int x, int y, int width, int height);
  void ReadFromPatternFlipTiff(char* dir, char* firstImg);
  
};

class WriteMod : public Module
{
 public:

  WriteMod(Buffer* in) : Module(in, NULL, 150){};
    
  void WriteMats();
  void WriteMats(std::string outDir);
  void WriteSlides(std::string outDir, std::string name);
  void WriteMatsByCount(std::string outDir);
  void WriteVideo();
  void WriteVideo(char* label);
  void WriteCompVideo(std::string outDir);

};
#endif
