#ifndef _whiteBoardProcess_h_
#define _whiteBoardProcess_h_

using namespace cv;
using namespace boost;


class WhiteBoardProcess : public Processor
{

 public:
  void run();
  WhiteBoardProcess (Buffer* in, Buffer* out) : Processor (in,out){cv::setNumThreads(4);};
  cv::Mat inputImg;
  cv::Mat backgroundImg;
  cv::Mat improvedInputImg;
  cv::Mat improvedInputImgNoProf;
  cv::Mat improvedInputImgNoProfContrast;
  cv::Mat improvedInputImgNoProfContrastSharp;
  cv::vector<cv::Mat> inputPlanes;
  cv::vector<cv::Mat> backgroundPlanes;
  cv::vector<cv::Mat> improvedPlanes;
  cv::vector<cv::Mat> improvedPlanesNoProf;
  cv::vector<cv::Mat> improvedPlanesNoProfContrast;
  cv::vector<cv::Mat> improvedPlanesNoProfContrastSharp;

  void createBackgroundImg(int kernalSize);
  void createImprovedInputImg();
  void removeProf();
  void createContrastImprovedInputImg();
  void sharpenContrastImprovedInputImg();
};



#endif
