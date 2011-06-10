/*
| ==============================================================================
| Copyright (C) 2005-2007 Prosilica.  All Rights Reserved.
|
| Redistribution of this header file, in original or modified form, without
| prior written consent of Prosilica is prohibited.
|
|==============================================================================
|
| This sample code, open the first camera found on the host computer and set it
| for capturing. It then wait for the user to press a key before enqueuing a
| frame and saving it to a TIFF file if the capture was successful
|
|==============================================================================
|
| THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
| WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
| NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
| DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
| INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
| LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
| OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF
| LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
| NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
| EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
|
|==============================================================================
*/


#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <time.h>


#include "PvApi.h"
#include "ImageLib.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include <memory.h>

// camera's data
typedef struct 
{
    unsigned long   UID;
    tPvHandle       Handle;
    tPvFrame        Frame;

} tCamera;


void Sleep(unsigned int time)
{
    struct timespec t,r;
    
    t.tv_sec    = time / 1000;
    t.tv_nsec   = (time % 1000) * 1000000;    
    
    while(nanosleep(&t,&r)==-1)
        t = r;
}


// Global variables
unsigned long FrameSize = 0;

// wait for a camera to be plugged
void WaitForCamera()
{
    printf("waiting for a camera ");
    while(!PvCameraCount())
    {
        printf(".");
        Sleep(250);
    }
    printf("\n");
}

// wait for the user to press q
bool WaitForUserToQuitOrSnap()
{
    char c;

    do
    {
        c = getc(stdin);

    } while(c != 'q' && c != 's');

    return c == 's';
}

// get the first camera found
bool CameraGet(tCamera* Camera)
{
    tPvUint32 count,connected;
    tPvCameraInfo list;

    count = PvCameraList(&list,1,&connected);
    if(count == 1)
    {
        Camera->UID = list.UniqueId;
        printf("got camera %s\n",list.SerialString);
        return true;
    
    }
    else
        return false;
}

// open the camera
bool CameraSetup(tCamera* Camera)
{
	tPvErr Error = PvCameraOpen(Camera->UID,ePvAccessMaster,&(Camera->Handle));   
	printf("Setup returns %d\n", Error);
	return (!Error);
}

// setup and start streaming
bool CameraStart(tCamera* Camera)
{

    // Auto adjust the packet size to max supported by the network, up to a max of 8228.
    // NOTE: In Vista, if the packet size on the network card is set lower than 8228,
    //       this call may break the network card's driver. See release notes.
    //
    //PvCaptureAdjustPacketSize(Camera->Handle,8228);

    // how big should the frame buffers be?
    tPvErr Error = PvAttrUint32Get(Camera->Handle,"TotalBytesPerFrame",&FrameSize);
	printf("Start returns %d,  Frame size= %d\n", Error, FrameSize);
	Error = (tPvErr)0;  // BFS added this
	if(!Error)
    {
        bool failed = false;

        // allocate the buffer for the single frame we need
        Camera->Frame.Context[0]  = Camera;
        Camera->Frame.ImageBuffer = new char[FrameSize];
        if(Camera->Frame.ImageBuffer)
            Camera->Frame.ImageBufferSize = FrameSize;
        else
            failed = true;

        if(!failed)
        {
            // set the camera is capture mode
            tPvErr Error = PvCaptureStart(Camera->Handle);
			printf("Capture start returns %d\n", Error);
            if(!Error)
            {
		        // set the camera in continuous acquisition mode
		        Error = PvAttrEnumSet(Camera->Handle,"FrameStartTriggerMode","Freerun");
				printf("Set trigger mode returns %d\n", Error);
				Error = (tPvErr)0;		// THIS APPEARS TO BE NECESSARY, NOT SURE WHY
				if(!Error)
		        {			
                	        // and set the acquisition mode into continuous
							Error = PvCommandRun(Camera->Handle,"AcquisitionStart");
							printf("Start acquisition returns %d\n", Error);
                	        if(Error)
                	        {
                    		        // if that fail, we reset the camera to non capture mode
                    		        PvCaptureEnd(Camera->Handle) ;
                    		        return false;
                	        }
                	        else
                    		        return true;
		        }
		        else
		            return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

// stop streaming
void CameraStop(tCamera* Camera)
{
    PvCommandRun(Camera->Handle,"AcquisitionStop");
    PvCaptureEnd(Camera->Handle);  
}

// snap and save a frame from the camera
bool CameraSnap(tCamera* Camera)
{
	bool snapOK = false;
    tPvErr Error = PvCaptureQueueFrame(Camera->Handle,&(Camera->Frame),NULL);
	printf("Snap returns %d\n", Error);
	if(!Error)
    {
        printf("waiting for the frame to be done ...\n");
        while(PvCaptureWaitForFrameDone(Camera->Handle,&(Camera->Frame),100) == ePvErrTimeout)
            printf("still waiting ...\n");
        if(Camera->Frame.Status == ePvErrSuccess)
        {
			snapOK = true;
        }
        else
            printf("the frame failed to be captured ...\n");
    } 
    else
        printf("failed to enqueue the frame\n");

	return snapOK;
}

// unsetup the camera
void CameraUnsetup(tCamera* Camera)
{
    PvCameraClose(Camera->Handle);
    // and free the image buffer of the frame
    delete [] (char*)Camera->Frame.ImageBuffer;
}


void CameraDisplay(tCamera* Camera)
{
	printf("Note that bit depth and number of planes should match camera setup\n");
	cv::Mat img;//(Camera->Frame.Width, Camera->Frame.Height, IPL_DEPTH_8U, 1);
	//img = cv::cvarrToMat(Camera->Frame.ImageBuffer).clone();
	//img = Camera->Frame.ImageBuffer;
	//memcpy(img.data, Camera->Frame.ImageBuffer, FrameSize);		// dest, source, #
	char* frameChar[FrameSize];
	
	
	frameChar = Camera->Frame.ImageBuffer;
	//char frame = Camera->Frame.ImageBuffer;
	//std::cout<< Camera->Frame.ImageBuffer&  << std::endl;
	/*
	// display main window
	cvNamedWindow( "Example1", 0 ); //CV_WINDOW_AUTOSIZE );
	cvShowImage( "Example1", img);
	cvResizeWindow("Example1", 300, 300 );

		// just for fun, make another image and do operations on it
		IplImage* output = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
		cvCanny( img, output, 20, 40 );

		CvFont font;
		cvInitFont( &font, CV_FONT_HERSHEY_DUPLEX, 2.0, 2.0 );
		cvPutText(output, "Edge Image", cvPoint(10,60), &font, cvScalar(255));

		cvNamedWindow( "Example2", 0 ); //CV_WINDOW_AUTOSIZE );
		cvShowImage( "Example2", output );
		cvResizeWindow("Example2", 300, 300 );

		// wait for key, then close displayed images
		printf("select either window and press any key to clear the windows\n");
		cvWaitKey( 0 );
		
		cvReleaseImage( &output );
		cvDestroyWindow( "Example2" );    

	cvReleaseImage( &img );
	cvDestroyWindow( "Example1" ); 
	*/
}


bool CameraSetupAndStart(tCamera* Camera)
{
bool setupOK = false;
    // get a camera from the list
    if(CameraGet(Camera))
    {
        // setup the camera
        if(CameraSetup(Camera))
        {
            // start streaming from the camera
            if(CameraStart(Camera))
				setupOK = true;
            else
			{
                printf("failed to start streaming\n");

				// unsetup the camera
				CameraUnsetup(Camera);
			}
        }
        else
            printf("failed to setup the camera\n");
    }
    else
        printf("failed to find a camera\n");
   
	return setupOK;
}

int main(int argc, char* argv[])
{
    // initialize the Prosilica API
    if(!PvInitialize())
    { 
        tCamera Camera;

        memset(&Camera,0,sizeof(tCamera));

        // wait for a camera to be plugged
        WaitForCamera();

		// get a camera from the list, set it up, start streaming
		if( CameraSetupAndStart(&Camera) )
        {
			while (true)
			{
				printf("camera is ready now. Press q to quit or s to take a picture\n");
				// wait for the user to quit or snap
				if(WaitForUserToQuitOrSnap())
				{
					// snap now
					if( CameraSnap(&Camera))
					{
						// save image
						if(!ImageWriteTiff("./snap.tiff",&(Camera.Frame)))
							printf("Failed to save the grabbed frame!\n");
			            else
						    printf("frame saved\n");

						// display image in OpenCV in two ways
						//	CameraDisplay(&Camera);
					}
					else
						printf("unable to snap image\n");
				}
				else
					break;
			}
		}
		else
			printf("unable to setup and start the camera\n");        

		// uninitialise the API
        PvUnInitialize();
    }
    else
        printf("failed to initialise the API\n");

	printf("Capture done, press key to continue:\n");
	getc(stdin);

	return 0;
}
