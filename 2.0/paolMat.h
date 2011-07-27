// ; -*-C++-*-
#ifndef _paolMat_h_
#define _poalMat_h_

using namespace cv;

class paolMat
{
 public:
  cv::Mat src;
  cv::vector<cv::Mat> planes;
  int count;
  int time;
  std::string name;
  cv::Point camera;
  cv::Point prof;

  paolMat();
  ~paolMat();
  paolMat(Ptr<paolMat> m);
  paolMat(paolMat* m);
  void copy(Ptr<paolMat> m);
  void read(std::string fullName, std::string fileName, int countIn, int timeIn);
  void write();
  void print();
  void edges();
  void invert();
  void split(){cv::split(src, planes);};
  void merge();
  void createBackgroundImg(int kernalSize);
  Ptr<paolMat> returnCreateBackgroundImg(int kernalSize);
  void improveInputImg(Ptr<paolMat> background);
  Ptr<paolMat> returnImproveInputImg(Ptr<paolMat> background);
  void removeProf();
  Ptr<paolMat> returnRemoveProf();
  void createContrast();
  Ptr<paolMat> returnCreateContrast();
  void sharpen();
  Ptr<paolMat> returnSharpen();
  void shrink();
  Ptr<paolMat> returnShrink();
};


#endif
