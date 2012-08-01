//Hack to avoid unused var warning
argc++;
argc--;

char WBdir[1024];
std::strcpy(WBdir, argv[2]);
char WBfirst[1024];
std::strcpy(WBfirst, argv[3]);
char WBpattern[512];
std::strcpy(WBpattern, argv[4]);

////////////////////////////////
/// Camera/WB Cap/Proc Buffers /
Buffer* wbBuffer;
wbBuffer = new Buffer;
Buffer* lectVideoBuffer;
lectVideoBuffer = new Buffer;
Buffer* wbSlidesBuffer;
wbSlidesBuffer = new Buffer;
Buffer* wbSlidesWriteBuffer;
wbSlidesWriteBuffer = new Buffer;
Buffer* wbReadBuffer;
wbReadBuffer = new Buffer;

//////////////////////////////////////
// WB Cap/Proc Modules
//  WriteMod presVideoFramesWriter(lectVideoBuffer);
WriteMod wbSlidesWriter(wbSlidesWriteBuffer);
WhiteBoardFoot wbSlides(wbSlidesBuffer,wbSlidesWriteBuffer);
WhiteBoardProcess wbproc(wbBuffer, wbSlidesBuffer);
// LectVideoFrameCreate lectFrameCreator(wbBuffer, lectVideoBuffer);
LocateSpeaker locateSpeaker(wbReadBuffer, wbBuffer);
ReadMod readFromDisk(wbReadBuffer);

///////////////////////////////////////
// Launch WB mods in revers order /////
  
boost::thread_group wbThreads;

//wbThreads.create_thread(boost::bind(&WriteMod::WriteMatsByCount, &presVideoFramesWriter, outDir));
wbThreads.create_thread(boost::bind(&WriteMod::WriteSlides, &wbSlidesWriter, outDir, "whiteboard", start));
wbThreads.create_thread(boost::bind(&WhiteBoardFoot::run, &wbSlides));
wbThreads.create_thread(boost::bind(&WhiteBoardProcess::run, &wbproc, 1));
wbThreads.create_thread(boost::bind(&LocateSpeaker::run, &locateSpeaker));
wbThreads.create_thread(boost::bind(&ReadMod::ReadFromPatternFlipExt, &readFromDisk, WBdir,WBfirst,WBpattern));
  
wbThreads.join_all();
  
std::cout<<"Main: about to begin deleting WB buffers"<<std::endl;
delete wbSlidesBuffer;
delete wbBuffer;
delete lectVideoBuffer;
//delete lectVideoBuffer;
delete wbReadBuffer;
std::cout<<"Main: finished deleting WB buffers"<<std::endl;
