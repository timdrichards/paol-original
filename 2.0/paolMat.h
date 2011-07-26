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
  void read(std::string dir, std::string fileName, int countIn, int timeIn);
  void write();
  void print();
};


#endif
