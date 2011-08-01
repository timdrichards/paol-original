// ; -*-C++-*-
#ifndef _paolMat_h_
#define _poalMat_h_

using namespace cv;

class paolMat
{
 public:
  cv::Mat src;
  cv::Mat mask;
  int count;
  int time;
  int difs;
  std::string name;
  cv::Point camera;
  cv::Point prof;

  paolMat();
  ~paolMat();
  paolMat(Ptr<paolMat> m);
  paolMat(paolMat* m);
  void copy(Ptr<paolMat> m);
  void copyNoSrc(Ptr<paolMat> m);
  void read(std::string fullName, std::string fileName, int countIn, int timeIn);
  void write();
  void writeMask();
  void print();
  void edges();
  Ptr<paolMat> returnEdges();
  void invert();
  void createBackgroundImg(int kernalSize);
  Ptr<paolMat> returnCreateBackgroundImg(int kernalSize);
  void improveInputImg(Ptr<paolMat> background);
  Ptr<paolMat> returnImproveInputImg(Ptr<paolMat> background);
  void removeProf(Ptr<paolMat> oldImg);
  Ptr<paolMat> returnRemoveProf(Ptr<paolMat> oldImg);
  void createContrast();
  Ptr<paolMat> returnCreateContrast();
  void sharpen();
  Ptr<paolMat> returnSharpen();
  void shrink();
  Ptr<paolMat> returnShrink();
  void difference(Ptr<paolMat> img, int thresh, int size, int maskBottom);
  void localizeSpeaker();
  void decimateMask();
  void connected();
  
};


#endif
