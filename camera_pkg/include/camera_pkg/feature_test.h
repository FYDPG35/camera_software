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

#define MAX_PATH_NAME		64
#define MAX_FRAME_SIZE_SUPPORT	16
#define MAX_PIXELFORMAT_SUPPORT 16

#define MAX_FRMINVAL_SUPPORT	10

#define SINGLE 1
#define DOUBLE 2

#define READ			0
#define WRITE			1
#define QUERY			2

#define SAVE_BMP		0
#define SAVE_YUV		1

#define ARRAY_SIZE(array)	(sizeof(array) / sizeof((array)[0]))

#define TEST_BUFFER_NUM		3

#define CAMERA_DEVNAME_DEFAULT		"/dev/video1"
#define DISPLAY_DEVNAME_DEFAULT		"/dev/video0"

#define SUCCESS		0
#define FAILURE 	-1
#define ENABLE		1
#define DISABLE		0
#define NO_DATA				0x00000000
#define RESET_TIME			0x00000001
#define MICRO_SEC			0x00000002
#define REGISTER_DATA			0x00000001			
#define UNREGISTER_CLEAR_ENTRY		0x00000002
#define GET_REGISTERED_DATA		0x00000003

#define MAX_SUPPORTED_FMTS		1

#define MAX_APP_PVIEW_WIDTH			1280	//1920
#define MAX_APP_PVIEW_HEIGHT			720	//1080

#define MIN_APP_PVIEW_WIDTH			1280	//320
#define MIN_APP_PVIEW_HEIGHT			720	//240

#define MAX_IMAGE_WIDTH				1280	//2592
#define MAX_IMAGE_HEIGHT			720	//1944

#define MIN_IMAGE_WIDTH				1280	//320
#define MIN_IMAGE_HEIGHT			720	//240

#define CONFIG_SAVE_16BIT_BMP			ENABLE

#define MMAP		1
#define USERPTR		2
	
int mem_type;
int max_image_width;
int max_image_height;

int total_ctrl_supported;
int total_focus_supported;
int total_exposure_supported;

struct v4l2_queryctrl	supported_ctrl[50];
struct v4l2_queryctrl	white_balamce_ctrl[5];
struct v4l2_queryctrl	focus_ctrl[5];
struct v4l2_queryctrl	exposure_ctrl[5];
char current_video_device[32];
char current_video_device_index[32];
enum capture_mode {
	//QVGA_320_240 = 0,
	//VGA_640_480 = 1,
	HD720P_1280_720 = 0,
	//HD1080P_1920_1080 = 3,
	//5MP_2592_1944 = 4,
};

enum camera_type
{
	CAMERA_USB_TYPE = 1,
	CAMERA_MXC_TYPE,
};

struct supported_formats {
	unsigned int width;
	unsigned int height;
	enum capture_mode mode;
};

typedef struct _device_data_application_struct
{
	int cam_fd, cam_fd1, disp_fd, save_fd, save_fd1;
	int wb_mode;

//	char *disp_fb_ptr;
	char *cam_device;	
	char *disp_device;
	char *rgb_888_buffer;
	char *raw_read_buffer;

	unsigned int total_frm_support;
	unsigned int total_pixfmt_support;
	unsigned int total_frminval_support;
	unsigned int show_frame_rate;
	unsigned int no_of_camera;
	unsigned int show_focus;
	unsigned int dwMask[3];
	unsigned int cam_fps, disp_fps;
	unsigned int capture_mode;
	unsigned int input_dev;
	unsigned int frame_count;

	
	pthread_t stream_process_tid;
	pthread_t snap_process_tid;
	pthread_t save_process_tid;

	struct v4l2_frmsizeenum 	fsize[MAX_FRAME_SIZE_SUPPORT];
	struct v4l2_fmtdesc		ffmt[MAX_PIXELFORMAT_SUPPORT];
	struct v4l2_frmivalenum		frminterval;
	struct v4l2_fract		desc_frm_inval[MAX_FRMINVAL_SUPPORT];
	struct v4l2_fract		stream_frm_inval;
	// struct v4l2_dbg_chip_ident	chip;
	struct v4l2_format		fmt,still_fmt,stream_fmt, disp_fmt;
	struct v4l2_streamparm 		cam_parm;
	struct v4l2_crop 		cam_crop, disp_crop;
	struct v4l2_cropcap 		disp_cropcap;
	struct v4l2_requestbuffers	cam_req, disp_req;
	struct v4l2_buffer		cam_buf, disp_buf, cam_buf1;
	struct v4l2_queryctrl 		qctrl;
	struct v4l2_control		ctrl;
	struct v4l2_capability 		disp_cap;
	enum v4l2_buf_type		cam_type, disp_type;
	struct v4l2_framebuffer		fb;
	SDL_Surface *surface;		/* the main window surface */
    SDL_Overlay* overlay;
    SDL_Rect drect;
    SDL_Event event;


	int memory_type;
	int camera_type;
	
	int display_width;
	int display_height;

	int max_preview_width;
	int max_preview_height;
	
	int stream_all_res;
	
	struct temperature{
		unsigned short int dec;
		unsigned char frac;
	}*celsius;


	struct testbuffer
	{
		unsigned char * cam_start;
		size_t cam_offset;
		unsigned int cam_length;

		unsigned char * disp_start;
		size_t disp_offset;
		unsigned int disp_length;
	
	}buffers[TEST_BUFFER_NUM];

	struct testbuffers
	{
		unsigned char * cam_start;
		size_t cam_offset;
		unsigned int cam_length;

		unsigned char * disp_start;
		size_t disp_offset;
		unsigned int disp_length;
	
	}buffers1[TEST_BUFFER_NUM];

	union
	{
		unsigned int Maintain_threads;
		struct
		{
			unsigned int snap_thread_kill		:1;
			unsigned int stream_thread_kill		:1;
		}thread;
	}kill;

	union
	{
		unsigned int G_FLAG;
		struct
		{
			unsigned int save_snap_completed	:1;
			unsigned int save_buffer_valid		:1;
			unsigned int save_file_format		:2;
			unsigned int comm_ctrl			:2;		// 0 - read, 1 - write, 2 - query 
			unsigned int valid_display_device	:1;
		}bit;
	}flag;

	struct
	{
		unsigned int still_width;
		unsigned int still_height;
		unsigned int stream_width;
		unsigned int stream_height;
		unsigned int display_width;
		unsigned int display_height;	
	}scale;	

	union
	{
		unsigned int G_FLAG;
		struct
		{
			unsigned int snap_mode		:1;
			unsigned int stream_mode	:1;
			unsigned int display_device	:1;
			unsigned int stream_progress	:1;
			unsigned int dss_stream_on	:1;
		}bit;
	}process;

	struct __attribute__ ((__packed__))
	{
		unsigned short int	type; 
		unsigned int		size; 
		unsigned short int	reserved1; 
		unsigned short int	reserved2; 
		unsigned int		offbits; 
		unsigned int		size_header; 
		int			width; 
		int			height; 
		unsigned short int	planes; 
		unsigned short int	bitcount; 
		unsigned int		compression; 
		unsigned int		size_image; 
		int			x_permeter; 
		int			y_permeter; 
		unsigned int		clr_used; 
		unsigned int		clr_important; 
	}bmp_header;

}device_data;

void print_version();

int feature_test_api_init(device_data **,int,char **);

int feature_test_api_exit(device_data *);

int get_supported_resolution_formats(device_data *);

int get_supported_pixel_formats(device_data *);

int validate_dimension(device_data *, unsigned int, unsigned int);

int snap_on(device_data *);

int snap_off(device_data *);

int stream_off(device_data *);

int stream_on(device_data *);

int find_capture_mode(device_data *);

int capture_prepare_streaming(device_data *);

int display_prepare_streaming(device_data *);

void *stream_video_userptr(void *);
void *stream_video_mmap(void *);
void *save_video_mmap(void *);

int print_pixelformat(char *, int);

int gettime(unsigned int *usec, unsigned char flag);

int get_camera_format(device_data *);

int set_camera_format(device_data *, struct v4l2_format *);

int convert_uyvy_rgb888(unsigned char *, unsigned int, unsigned int, unsigned char *);

int form_bmp_header_info(device_data *);

