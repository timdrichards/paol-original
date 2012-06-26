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
  bool lectFound;
  int r,g,b;

  paolMat();
  ~paolMat();
  paolMat(Ptr<paolMat> m);
  paolMat(paolMat* m);
  void copy(Ptr<paolMat> m);
  void copyNoSrc(Ptr<paolMat> m);
  void read(std::string fullName, std::string fileName, int countIn, int timeIn);
  void write();
  void write(std::string outDir);
  void writeByCount(std::string outDir);
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
  //One to One comparison of colors in each pixel of src
  void difference(Ptr<paolMat> img);
  //Ont to One comparison of the colors in each pixel of mask
  void maskDifference(Ptr<paolMat> img);
  //difference for computer proc
  void difference(Ptr<paolMat> img, int thresh, int size, int maskBottom);
  // thresh: old rgb - new rgb > thresh // size: downsample x/y+=size
  void differenceLect(Ptr<paolMat> inImg, int thresh, int size);
  void localizeSpeaker();
  void decimateMask();
  void decimateMask(int thresh);
  //Grow mask by adding values to bordering pixels
  void growMask();
  void shrinkMask();
  void connected();
  //Must be the same size as differenceLect
  void connected(int size);
  void lectArea();
  Ptr<paolMat> crop(int x, int y, int width, int height);
  Ptr<paolMat> cropFrame(int width, int height);
  vector<int> vertMaskHistogram();
  vector<int> horMaskHistogram();
  void decimateMaskByHistogram(int hThresh, int vThresh);
  //drift is y+1, x+1
  void drift();
  void driftWAverage();
  void sweepMask();
  void sweepDown();
  //Scan left to right, top to botton toggling mask everytime the src color changes
  void intensityMask(int thresh);
  //Scan a rectangle around a pixel and change it to white,black, or red.
  void maskToWhite(int thresh);
  void average();
  void blackSrcByMask();
  //blur size is pixels adjacent i.e. 1 would be a 3x3 square centered on each pixel
  void blur(int size);
  //pDrift is y+-1 x+-1
  void pDrift();
  //Grow by blue thresh and by size
  void grow(int blueThresh, int size);
  //Shrink by blue thresh and size
  void shrink(int blueThresh, int size);
  //threshedDifference, only where both masks blue > 30
  void threshedDifference(Ptr<paolMat> old);
  void getCombine(Ptr<paolMat> img);
  void blackMaskByMask(Ptr<paolMat> img);
  void updateBackground(Ptr<paolMat> alt, Ptr<paolMat> img);
  void cleanBackground(Ptr<paolMat> img);
  void differenceDarken(Ptr<paolMat> img);
  void maskGrowRed(int size);
  void countDiffsMasked(Ptr<paolMat> img);
  


};


#endif
