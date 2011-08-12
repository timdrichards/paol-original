#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gphoto2/gphoto2.h>
#include "opencv/cv.h"

#include "usb.h"
#include "sample-capture.h"

extern "C"{

  extern void errordumper(GPLogLevel level, const char *domain, const char *str, void *data);
  extern void capture_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size);
  extern void capture_to_file(Camera *camera, GPContext *context, char *fn);
  GPContext* sample_create_context();

}

int main(int argc, char **argv) {
	Camera	*camera;
	int	retval;
	GPContext *context = sample_create_context();
	char fileName[256] = "foo.jpg";
	
	gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
	gp_camera_new(&camera);

	/* When I set GP_LOG_DEBUG instead of GP_LOG_ERROR above, I noticed that the
	 * init function seems to traverse the entire filesystem on the camera.  This
	 * is partly why it takes so long.
	 * (Marcus: the ptp2 driver does this by default currently.)
	 */
	printf("Camera init.  Takes about 10 seconds.\n");
	retval = gp_camera_init(camera, context);
	if (retval != GP_OK) 
	  {
	    printf("  Retval: %d\n", retval);
	    exit (1);
	  };
	capture_to_file(camera, context, fileName);
	gp_camera_exit(camera, context);

	return 0;
}
