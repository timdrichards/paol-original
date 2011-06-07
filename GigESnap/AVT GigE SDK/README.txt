### Prosilica GigE SDK 1.24 for Linux
###
### 05/28/2010
###

Notes:

* This distribution supports x86, PPC, ARM (LE hard-float GCC 3.3) and x64 (AMD64) Linux (2.6.X). Modify the "ARCH" file located in the Examples directory before building the samples to select the proper architecture: x86, ppc, arm or x64.

* The ARM support included in this SDK is for the embedded board TS-7800 (http://www.embeddedarm.com)

* If you need this driver for any specific platform, contact us (e.g. armbe soft-float GCC 4.1, uCLinux ...).

* The shared library in ./bin-pc is compiled with GCC 4.4

* Static libraries are provided for GCC 4.1, 4.2, 4.3, 4.4 they can be found in ./lib-pc

* Each of the sample code can be built as follows:

	> make sample ; make install
	
  If you rather compile against a static library rather than the dynamic library, build as follow:
  
	> make sample-static ; make install

The executables will be copied into the ./bin-pc folder.

* A compiled version of the SampleViewer is provided for each support architecture. It is statically compiled against wxGTK. To compile the SampleViewer source code you will need the wxGTK (>= 2.6) library.  See: http://www.wxwidgets.org/downloads . Compile wxGTK as Unicode. See:
readme-gtk.txt, install-gtk.txt, BuildCVS.txt in the wxGTK download for more information. The makefile can be modified to use the version you have installed on your system. 

* The MTU of the GigE adapter should be changed to accommodate for Jumbo frames using the 'ifconfig' command. The syntax is as follows:

	> sudo ifconfig eth0 mtu 8228

where eth0 is the adapter name and 8228 the maximum size of the frames to be used by the camera. If the MTU is set to a lower value, the camera's PacketSize should be adjusted accordingly.

* As the API use some signals internally, it is required to protect some of your system calls (such as sem_wait) from been interrupted. This can be done by calling the system call from within a loop and only exiting from it if there was no error or if errno is different from EINTR.

* Users experiencing dropped frames or packets should run their application with root privileges. This allows the OS to boost the priority of the Prosilica driver thread.
Running as root also allows the driver to bind directly to the NIC adapter. Doing so shortens the path the packets take within the OS network stack. 
Users who feel that running their application as root compromises their system security may find the following implementation satisfactory:
	-set the executable owner as root
	-set the "setuid" permission bit on the executable
	-in code, when their application starts use capset() to release all but these privileges:
		CAP_SYS_NICE
		CAP_NET_ADMIN
		CAP_NET_BROADCAST
		CAP_NET_RAW  
The application will start with all root privileges, but it will drop them immediately after startup.

* In order to use multicasting, you may have to add manually a route. The syntax is as follows:

	> sudo route -n add -net 224.0.0.0 netmask 240.0.0.0 dev eth3

where eth3 is the adapter name (replace by yours). Multicasting will only work if the application is run as root.

* The Java folders contain a JNI interface to PvAPI, plus a set of samples. You will need to use the build.xml file located in each subdirectory to import the project with Eclipse. Each of the following samples: JListAttributes, JListCameras, JSnap, JStream, JThread, JThread3 need to have PvJPI in their build path. For convenience, the JNI dynamic library has been built and placed in the bin-pc folder. Each of the Java samples need the following VM argument added to its Run/Debug settings: -Djava.library.path=/path/to/the/SDK/bin-pc/x86. The working directory needs to be /path/to/the/SDK/bin-pc/x86.
