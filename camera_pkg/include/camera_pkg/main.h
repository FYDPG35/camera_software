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
 *   CODE_REV  : 1.0					                   			          *
 **********************************************************************************************************/

/*
 *==========================================================================================================
 *                                        REVISION HISTORY                                  
 *----------------------------------------------------------------------------------------------------------
 * CODE_REV  REASON FOR MODIFICATION                MODIFIED FUNCTION NAME  	            AUTHOR
 *----------------------------------------------------------------------------------------------------------
 * 1.0       INITIAL RELEASE                                                        	   BALAJI P
 *
 *==========================================================================================================
 */

static const char* program_name = NULL;

enum main_menu
{
	MAIN_MENU_BASE_MENU,
	MAIN_MENU_STILL,
	MAIN_MENU_STREAMING,
	MAIN_MENU_FEATURES,
	MAIN_MENU_HID_FEATURES,
	EXIT_FRM_APPLICATION,
};

enum hid_menu
{
	SUB_MENU_HID_SPI_WRITE,
	SUB_MENU_HID_SPI_READ,
	SUB_MENU_HID_READ_FIRMWAREVERSION,
	SUB_MENU_HID_EXIT,
};

enum still_menu
{
	SUB_MENU_STILL_BASE_MENU,
	SUB_MENU_STILL_DIMENSION_CHANGE,
	SUB_MENU_STILL_TAKE_SNAP,
	SUB_MENU_STILL_IMG_FMT,
	SUB_MENU_STILL_EXIT,
};

enum still_file_fmt
{
	SUB_MENU_STILL_FMT_BASE_MENU,
	SUB_MENU_STILL_FMT_YUV,
	SUB_MENU_STILL_FMT_RGB,
};

enum stream_menu
{
	SUB_MENU_STREAM_BASE_MENU,
	SUB_MENU_STREAM_DIMENSION_CHANGE,
	SUB_MENU_STREAM_START,
	SUB_MENU_STREAM_STOP,
	SUB_MENU_STREAM_EXIT,
};

enum features_menu
{
	SUB_MENU_FEATURE_BASE_MENU,
	SUB_MENU_FEATURE_BRIGHTNESS,
	SUB_MENU_FEATURE_CONTRAST,
	SUB_MENU_FEATURE_SATURATION,
	SUB_MENU_FEATURE_HUE,
	SUB_MENU_FEATURE_WHITEBALANCE,
	SUB_MENU_FEATURE_V_FLIP,
	SUB_MENU_FEATURE_H_MIRROR,
	SUB_MENU_FEATURE_FOCUS,
	SUB_MENU_FEATURE_EFFECTS,
	SUB_MENU_FEATURE_TEST_PATTERN,
	SUB_MENU_FEATURE_SHOWFPS,
	SUB_MENU_FEATURE_EXIT,
};

enum focus_menu
{
	SUB_MENU_FEATURE_FOCUS_BASE,
	SUB_MENU_FEATURE_FOCUS_CONTINUOS,
	SUB_MENU_FEATURE_FOCUS_TRIGGER,
	SUB_MENU_FEATURE_FOCUS_ABSOLUTE,
	SUB_MENU_FEATURE_FOCUS_EXIT,
};


int paint_main_menu();
int ctrl_v4l2_parms(device_data *dev);
