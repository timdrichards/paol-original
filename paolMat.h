#ifndef _paolMat_h_
#define _poalMat_h_

class CV_EXPORTS paolMat : public cv::Mat
{
 public:
  int time;
  int count;
  std::string name;

};

#endif
