// Including Boost
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/containers/vector.hpp>

//Including C++ Libs
#include <iostream>
#include <iterator>
#include <queue>
//#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <jpeglib.h>
#include <jerror.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-camera.h>


//Open CV
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "paolMat.h"
#include "buffer.h"
#include "module.h"
#include "genericProcess.h"
#include "usbCamC.h"
#include "usbCam.h"


#define _debug_

extern "C" {
  static void errordumper(GPLogLevel level, const char *domain, const char *str, void *data)
  {
    fprintf(stdout, "%s\n", str);
  }
  
  static void capture_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size) 
  {
    int retval;
    CameraFile *file;
    CameraFilePath camera_file_path;
    
    printf("Capturing.\n");
    
    /* NOP: This gets overridden in the library to /capt0000.jpg */
    strcpy(camera_file_path.folder, "/");
    strcpy(camera_file_path.name, "foo.jpg");
    
    retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
    printf("  Retval: %d\n", retval);
    
    printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);
    
    retval = gp_file_new(&file);
    printf("  Retval: %d\n", retval);
    retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
				GP_FILE_TYPE_NORMAL, file, context);
    printf("  Retval: %d\n", retval);
    
    gp_file_get_data_and_size (file, ptr, size);
    
    printf("Deleting.\n");
    retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
				   context);
    printf("  Retval: %d\n", retval);
    
    gp_file_free(file);
  }
  static void
capture_to_file(Camera *camera, GPContext *context, char *fn) {
	int fd, retval;
	CameraFile *file;
	CameraFilePath camera_file_path;

	printf("Capturing.\n");

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path.folder, "/");
	strcpy(camera_file_path.name, "foo.jpg");

	retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
	printf("  Retval: %d\n", retval);

	printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

	fd = open(fn, O_CREAT | O_WRONLY, 0644);
	retval = gp_file_new_from_fd(&file, fd);
	printf("  Retval: %d\n", retval);
	retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
		     GP_FILE_TYPE_NORMAL, file, context);
	printf("  Retval: %d\n", retval);

	printf("Deleting.\n");
	retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
			context);
	printf("  Retval: %d\n", retval);

	gp_file_free(file);
  }
};

void UsbCam::run()
{
  Camera *camera;
  int retval;
  GPContext *context = sample_create_context();
  
  gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
  gp_camera_new(&camera);
  
  /* When I set GP_LOG_DEBUG instead of GP_LOG_ERROR above, I noticed that the
   * init function seems to traverse the entire filesystem on the camera.  This
   * is partly why it takes so long.
   * (Marcus: the ptp2 driver does this by default currently.)
   */
  printf("Camera init.  Takes about 10 seconds.\n");
  retval = gp_camera_init(camera, context);
  if (retval != GP_OK) {
    printf("  Retval: %d\n", retval);
    exit (1);
  }
  capture_to_file(camera, context, "foo.jpg");
  gp_camera_exit(camera, context);
};

