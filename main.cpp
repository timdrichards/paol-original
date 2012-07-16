// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/program_options.hpp>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
#include <cstdio>
#include <time.h>

//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
//Our Libs

#include "paolMat.h"
#include "buffer.h"
#include "module.h"
#include "whiteBoard.h"
#include "whiteBoardFoot.h"
#include "genericProcess.h"
#include "locateSpeaker.h"
#include "ac.h"
#include "lectVideoFrameCreate.h"
#include "computerDistribute.h"
#include "computerProcess.h"
#include "WebCamCapture.h"
//#include "usbCam.h"
//#include "sample-capture.h"
#include "computerMain.h"
#include "wbMain.h"

//Map po:: to boost::program_options::
namespace po = boost::program_options;

using namespace cv;

void computerPipeline(char* outDir, char* dir, char* first, int start);
void wbPipeline(char* outDif, char* dir, char* first, int start);

using namespace std;


int main(int argc, char** argv)
{
  //create thread group for pipelines
  boost::thread_group pipelines;
  
  //parse args
  try {
    po::options_description desc("Allowed Options");
    desc.add_options()
      ("help,h", 
       "Produce help message")
      ("out,O", po::value<string>(), "Output directory")
      ("WBdir", po::value<string>(), "Whiteboard source directory")
      ("WBfirst", po::value<string>(), "Whiteboard first frame")
      ("Cdir", po::value<string>(), "Computer source directory")
      ("Cfirst", po::value<string>(), "Computer first frame")
      ("info,I", po::value<string>(), "INFO file")
      ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help"))
      {
	cout<< desc <<endl;
	return 1;
      }
    char dir[1024];
    char Cdir[1024];
    char Cfirst[1024];
    char WBfirst[1024];
    char WBdir[1024];
    int start;

    //Check that we have out and either a whiteboard or a screen
    if( ( (vm.count("out") && vm.count("info")) &&
	  ( (vm.count("WBdir") && vm.count("WBfirst") ) ||
	    (vm.count("Cdir") && vm.count("Cfirst") ) )
	  )
	)
      {
	string startTime;
	std::ifstream INFO;
	
	string infoString = vm["info"].as<string>();
	char infoLoc[1024];
	std::strcat(infoLoc, infoString.c_str());
	
	INFO.open(infoLoc, ios::in);
	if(!INFO.is_open())
	  {
	    std::cout<<"Could not open info file\n";
	    return 1;
	  }
	else
	  {
	    //Parse start time from info file
	    

	    bool found = false;
	    while(INFO.good())
	      {
		std::getline(INFO,startTime);
		char line[1024];
		strcpy(line,startTime.c_str());
		std::cout<<"INFO: "<<line<<std::endl;
		int year,month,day,hour,minute,second;
		if(std::sscanf(line,"start: %d,%d,%d,%d,%d,%d",&year,&month,&day,&hour,&minute,&second) > 0)
		  {
		    found = true;
		    std::time_t rawtime;
		    struct tm * timeinfo;
		    std::time(&rawtime);
		    timeinfo = std::localtime(&rawtime);
		    timeinfo->tm_year = year -1900;
		    timeinfo->tm_mon = month -1;
		    timeinfo->tm_mday = day;
		    timeinfo->tm_hour = hour;
		    timeinfo->tm_min = minute;
		    timeinfo->tm_sec = second;
		    //check daylight savings
		    if( (year == 2012 && 
			 (month == 3 && day > 11) ||
			 (month > 3 && 11 > month) ||
			 (month == 11 && day < 4) ) ||
			(year == 2013 &&
			 (month == 3 && day > 10) ||
			 (month > 3 && 11 > month) ||
			 (month == 11 && day < 3) ))
		      {
			std::cout<<"Recording during daylight savings time eh?\n";
			timeinfo->tm_isdst = 1;
		      }else if(year == 2012 || year == 2013)
		      {
			std::cout<<"Not recorded during daylight savings time\n";
			timeinfo->tm_isdst = 0;
		      }else
		      {
			std::cout<<"Cannot check daylight savings time\n";
			timeinfo->tm_isdst = -1;
		      }
		    start = (int)std::mktime(timeinfo);
		    break;
		  }
	      }
	    std::cout << "Start time "<<startTime<<std::endl;
	    if(found)
	      std::cout << " in POSIX thats "<<start<<std::endl;
	    else
	      {
		std::cout << "couldn't find the time in the info file\n";
		return 1;
	      }
	    
	    
	    
	    string dirString = vm["out"].as<string>();
	    strcpy(dir,dirString.c_str());
	  }
      }else
      {
	cout<< "Must specify AT MINIMUM --out AND --start AND a whiteboard OR computer source\n"
	    <<" try --help"<<endl;
	return 1;
      }
    
    //if either WBdir or WB first, check for both
    if( vm.count("WBdir") ^ vm.count("WBfirst"))
      {
	cout <<"To process a whiteboard BOTH --WBdir and --WBfirst must be specified"<<endl;
	return 1;
      }
    
    //if either Cdir or Cfirst, check for both and --info
    if(vm.count("Cdir") ^ vm.count("Cfirst") )
      {
	cout <<"To process a screen --Cdir AND --Cfirst must be specified"<<endl;
	return 1;
      }
    
    //Launch a Whiteboard pipeline, if specified
    if(vm.count("WBdir") && vm.count("WBfirst"))
      {
	string WBfirstString = vm["WBfirst"].as<string>();
	strcpy(WBfirst,WBfirstString.c_str());
	
	string WBdirString = vm["WBdir"].as<string>();
	strcpy(WBdir,WBdirString.c_str());
		
	cout << "Whiteboard source: " << vm["WBdir"].as<string>() 
	     << " starting with: " << vm["WBfirst"].as<string>() <<endl;
	pipelines.create_thread(boost::bind(wbPipeline,
					    dir,WBdir,WBfirst,start));
	cout << "Whiteboard pipeline launched!"<<endl;
      }
    
    //Launch a Computer pipeline, if specified
    if(vm.count("Cdir") && vm.count("Cfirst"))
      {
	string CfirstString = vm["Cfirst"].as<string>();
	strcpy(Cfirst,CfirstString.c_str());
	
	string CdirString = vm["Cdir"].as<string>();
	strcpy(Cdir,CdirString.c_str());
	
	cout << "Computer source: "<< vm["Cdir"].as<string>()
	     << " starting with: "<<vm["Cfirst"].as<string>() <<endl;
	pipelines.create_thread(boost::bind(computerPipeline,
					    dir,Cdir,Cfirst, start));
	cout << "Computer pipeline launched!"<<endl;
      }
      
    pipelines.join_all();
  }
  catch(exception& e)
    {
      cerr<<"Error: "<<e.what()<<"\n";
      return 1;
    }
  catch(...)
    {
      cerr<<"Error: Exception of unknown type!\n";
    }
    
  return 0;
}
  
