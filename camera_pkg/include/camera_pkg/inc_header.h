/**********************************************************************************************************
 *   Copyright (C) 2013 by e-con Systems All Rights Reserved.                                             *
 *   www.e-consystems.com                                                                                 *
 *                                                                                                        *
 *   The source code contained or described herein and all documents                                      *
 *   related to the source code (Material) are owned by e-con Systems                                     *
 *                                                                                                        *
 *                                                                                                        *
 *   PROJECT	           :   COMMAND LINE V4L2 APPLICATION                                              *
 *   MODULE NAME           :   CMD_V4L2_FULL_FEATURE                                                      *
 *   MODULE VERSION        :   1.0                                                                        *
 *                                                                                                        *
 *                                                                                                        *
 *                                                                                                        *
 **********************************************************************************************************/

/*
 *==========================================================================================================
 *                                        REVISION HISTORY                                  
 *----------------------------------------------------------------------------------------------------------
 * CODE_REV  REASON FOR MODIFICATION                MODIFIED FUNCTION NAME  	            AUTHOR
 *----------------------------------------------------------------------------------------------------------
 * 1.0       INITIAL RELEASE							       	    BALAJI
 *
 *
 *==========================================================================================================
 */

#define MODULE_NAME "CMD_V4L2_FULL_FEATURE command line test application"

/*
 * ----------------------------------------------------------------------------------------------------------
 *
 *  Application needed standard header files
 *
 * ----------------------------------------------------------------------------------------------------------
 */
#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <errno.h>
#include <sys/types.h>	
#include <sys/stat.h>	
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <asm/types.h>
#include <sys/mman.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <getopt.h>
#include <dirent.h>
#include <signal.h>


/*
 * ----------------------------------------------------------------------------------------------------------
 *
 *  Application Support header files
 *
 * ----------------------------------------------------------------------------------------------------------
 */
#include <linux/videodev2.h>
#include <linux/fb.h>
#include <linux/media.h>
#include <linux/v4l2-subdev.h>

#define V4L2_CID_DRIVER_BASE		            (V4L2_CID_USER_BASE | 0x1001)
#define V4L2_CID_FOCUS_TRIGGER					(V4L2_CID_DRIVER_BASE + 1)
#define V4L2_CID_FACEDETECT		                (V4L2_CID_DRIVER_BASE + 2)
#define V4L2_CID_HDR                            (V4L2_CID_DRIVER_BASE + 4)
#define V4L2_CID_NOISE_FILTER                   (V4L2_CID_DRIVER_BASE + 5)
#define V4L2_CID_FOCUS_WINDOW                   (V4L2_CID_DRIVER_BASE + 6) //focus window
#define V4L2_CID_FOCUS_ROI_WINDOW               (V4L2_CID_DRIVER_BASE + 7) //roi focus window size
#define V4L2_CID_FOCUS_ROI                      (V4L2_CID_DRIVER_BASE + 8) //roi focus
#define V4L2_CID_EXPOSURE_ROI_WINDOW            (V4L2_CID_DRIVER_BASE + 9) //roi ae window size
#define V4L2_CID_EXPOSURE_ROI                   (V4L2_CID_DRIVER_BASE + 10) //roi ae window size
#define V4L2_CID_FOCUS_STATUS                   (V4L2_CID_DRIVER_BASE + 11) //roi ae window size
#define V4L2_CID_SENSOR_TEMP                   	(V4L2_CID_DRIVER_BASE + 12) //roi ae window size
#define V4l2_CID_BINNING						(V4L2_CID_DRIVER_BASE + 14)

//#define V4L2_CID_DRIVER_BASE		(V4L2_CID_USER_BASE | 0x1001)
//#define V4L2_CID_TEST_PATTERN		(V4L2_CID_DRIVER_BASE + 0)
//#define V4L2_SENS_TRIG_FOCUS		(V4L2_CID_DRIVER_BASE + 1)

#ifndef FBIO_WAITFORVSYNC
  #define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
#endif

/*
 * ----------------------------------------------------------------------------------------------------------
 *
 *  Application header files
 *
 * ----------------------------------------------------------------------------------------------------------
 */
#include <camera_pkg/feature_test.h>
#include <camera_pkg/main.h>
//#include <svn_revision.h>
