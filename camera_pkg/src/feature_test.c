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
/*
 * Custom header file inclution
 */

#include "camera_pkg/inc_header.h"
FILE *fp = 0;

char video_device_names[16][64];




void * get_virtual_addr(void *base_addr,unsigned int len)
{
	int fd = -1;
		if((fd = open("/dev/mem",O_RDWR)) < 0) {
		fprintf(stderr,"open error\n");
	}
	void *registers = mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,fd,(off_t)base_addr);
	if(registers == MAP_FAILED) {
		fprintf(stderr,"Cannot mmap %s!\n",strerror(errno));
		return 0;
	}
	return registers;
}




/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	: FEATURE_TEST_01	
 *  Name	:	register_device_data
 *  Parameter1	:	device_data **dev     - A main device structure variable address be preserved. 
 *  Parameter2	:	unsigned char configure_status_flag	- Based on condition flag the function will return
 *  								data to the application or preserve the data
 *  Returns	:	int - Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAILURE	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	
 ************************************************************************************************************/
int register_device_data(device_data **dev,unsigned char configure_status_flag)
{
	static unsigned int register_device_data;
	
	switch(configure_status_flag)
	{
		case REGISTER_DATA:
		{
			*dev	= calloc(sizeof(device_data),sizeof(unsigned char));
			if(*dev == NULL)
			{
				perror("calloc: ");
				return FAILURE;
			}
			register_device_data	= (unsigned int)*dev;
		}break;

		case UNREGISTER_CLEAR_ENTRY:
		{
			register_device_data	= DISABLE;
			free(*dev);
			*dev			= NULL;

		}break;
		
		case GET_REGISTERED_DATA:
		{
			*dev			= (device_data*)register_device_data;
		}break;
	}
	
	return SUCCESS;
}

int find_video_node_index(char *video_node_name)
{
	int index = 0;
	char node_index[10];
	strcpy(node_index,(video_node_name+5));
	index = atoi(node_index);
	return index;
}


int list_and_select_device(char *selected_video_device,char *selected_video_device_index)
{
	int total_video_nodes = 0;
	DIR *video_node_dir;
	struct dirent *video_node;
	struct stat videostat;
	int video_node_index = 0;
	int video_device_index = 0;
	char video_node_name[256];
	char scan_string[32];
	int menu_option = FAILURE;
	video_node_dir = opendir("/sys/class/video4linux/");
	if(video_node_dir == NULL)
	{
		printf("%s():opendir failed\n", __func__);	
	}
	else
	{
		
		printf("\n==================== Video devices detected ====================\n");
		while((video_node = readdir(video_node_dir))!= NULL)
		{
			if(video_node->d_type == DT_LNK)
			{
				strcpy(video_node_name,video_node->d_name);
				video_node_index = find_video_node_index(video_node_name);
				if(video_node_index < 15)
				{
					total_video_nodes++;
					printf("[%d]:%s\n",total_video_nodes,video_node->d_name);
					strcpy(video_device_names[video_device_index],video_node->d_name);
					video_device_index++;
				}
			}
		}
		if(total_video_nodes == 0)
		{
			printf("No video streaming devices found \n");
			printf("================================================================\n");
		}
		else
		{
			printf("================================================================\n");
			printf("Select the video streaming device = ");
			scanf("%s",scan_string);
			sscanf(scan_string,"%d",&menu_option);
			if(menu_option>total_video_nodes)
			{
				printf("Invalid device selected\n");
				menu_option = FAILURE;				
			}
			else
			{
				menu_option--;

				strcat(selected_video_device,video_device_names[menu_option]);
				strcpy(selected_video_device_index,video_device_names[menu_option]);
			}
		}
		closedir(video_node_dir);
	}
	return menu_option;	
}

int set_memory_type(device_data *dev)
{
	FILE *desc_file;
	int dev_name_len = 0;
	
	char mxc_camera_name[]="Mxc Camera";
	
	char file_path[64]="/sys/class/video4linux/";
	char file_name[] = "/name";

	char camera_name_read[32];
	char readchar;

	strcat(file_path,current_video_device_index);
	
	strcat(file_path,file_name);
	
	desc_file = fopen(file_path,"r");
	if(desc_file == NULL)
	{
		printf("%s:file open failed \n",__func__);
		return FAILURE;
	}
	while(1)
	{
		readchar =  fgetc(desc_file);
		if((readchar == EOF) || (readchar == '\n'))
			break;
		camera_name_read[dev_name_len++] = readchar;
		
	}
	camera_name_read[dev_name_len]='\0';
		
	fclose(desc_file);	
	if(strcmp(mxc_camera_name,camera_name_read)==0)
	{
		dev->memory_type = V4L2_MEMORY_USERPTR;
		dev->camera_type = CAMERA_MXC_TYPE; 
	}
	else{
		dev->memory_type =  V4L2_MEMORY_MMAP;
		dev->camera_type = CAMERA_USB_TYPE;
	}
	return SUCCESS;
}


int open_camera_device(device_data *dev)
{
	struct v4l2_input input;
	int index;
	
	struct v4l2_capability capability;

	
	/* Open the Capture device */
	if(dev->no_of_camera == SINGLE){
		if ((dev->cam_fd = open(dev->cam_device, O_RDWR, 0)) < 0) {
			printf("%s(): Can't open camera device %s\n", __func__,  dev->cam_device);
			goto cleanup;
		}
	}
	else
	{
		if ((dev->cam_fd = open("/dev/video0", O_RDWR, 0)) < 0) {
			printf("%s(): Can't open camera device /dev/video0\n", __func__);
			goto cleanup;
		}
		if ((dev->cam_fd1 = open("/dev/video1", O_RDWR, 0)) < 0) {
			printf("%s(): Can't open camera device /dev/video1\n", __func__);
			goto cleanup;
		}
	}

	if(dev->no_of_camera == SINGLE){
		/* Get any active input */
		if (ioctl(dev->cam_fd, VIDIOC_G_INPUT, &index) < 0) {
			perror("VIDIOC_G_INPUT");
			goto cleanup;
		}

		/* Enumerate input to get the name of the input detected */
		memset(&input, 0, sizeof(input));
		input.index = index;
		if (ioctl(dev->cam_fd, VIDIOC_ENUMINPUT, &input) < 0) {
			perror("VIDIOC_ENUMINPUT");
			goto cleanup;
		}

		index = 1;
		if (ioctl(dev->cam_fd, VIDIOC_S_INPUT, &index) < 0) {
			perror("VIDIOC_S_INPUT");
			index = 0;
			if (ioctl(dev->cam_fd, VIDIOC_S_INPUT, &index) < 0) {
				perror("VIDIOC_S_INPUT");
				goto cleanup;
			}
		}

		memset(&input, 0, sizeof(input));
		input.index = index;
		if (ioctl(dev->cam_fd, VIDIOC_ENUMINPUT, &input) < 0) {
			perror("VIDIOC_ENUMINPUT");
			goto cleanup;
		}
	}

	/* Check if the device is capable of streaming */
	if(ioctl(dev->cam_fd, VIDIOC_QUERYCAP, &capability) < 0) {
		perror(" VIDIOC_QUERYCAP");
		goto cleanup;
	}
	if (capability.capabilities & V4L2_CAP_STREAMING) {
	} else {
		printf("Camera: Not capable of streaming\n");
		goto cleanup;
	}

	if(dev->no_of_camera != SINGLE){
		/* Check if the device is capable of streaming */
		if(ioctl(dev->cam_fd1, VIDIOC_QUERYCAP, &capability) < 0) {
			perror(" VIDIOC_QUERYCAP");
			goto cleanup;
		}
		if (capability.capabilities & V4L2_CAP_STREAMING) {
		} else {
			printf("Camera 1: Not capable of streaming\n");
			goto cleanup;
		}
	}
	return SUCCESS;
cleanup:
	return FAILURE;
}

void close_camera_device(device_data *dev)
{
	if(dev->cam_fd >= 0) {
		close(dev->cam_fd);
		dev->cam_fd = -1;
	}
	if(dev->no_of_camera != SINGLE){
		if(dev->cam_fd1 >= 0) {
			close(dev->cam_fd1);
			dev->cam_fd1 = -1;
		}
	}

}


void close_display_device(device_data *dev)
{
	SDL_FreeYUVOverlay(dev->overlay);
    if (dev->surface) 
        SDL_FreeSurface(dev->surface);

    dev->overlay = (SDL_Overlay *)NULL;
    dev->surface = (SDL_Surface *)NULL;

    if (SDL_WasInit(SDL_INIT_VIDEO|SDL_INIT_TIMER)) { 
        SDL_Quit();
    }

}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_03
 *
 *  Name	:	feature_test_api_init
 *  Parameter1	:	device_data **dev - Address of Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAILURE	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	This function initialize the feature test api module
 ************************************************************************************************************/
int feature_test_api_init(device_data **g_dev,int argc,char **argv)
{
	device_data *dev;
	int ret_val;
	struct v4l2_input input;
	int index;
	int video_device_index = -1;
	char video_device_selected[32]="/dev/";
	char video_device_index_selected[32];

	struct v4l2_capability capability;
	/* Initialiaze the device data struct */

	if( register_device_data(g_dev,REGISTER_DATA) < 0) {
		return FAILURE;
	}

	register_device_data(&dev,GET_REGISTERED_DATA);

	if(argc < 2){
		video_device_index = list_and_select_device(video_device_selected,video_device_index_selected);
		if(video_device_index == FAILURE)
		{
			return FAILURE;
		}
		/* Set the selected capture and display device */
		strcpy(current_video_device,video_device_selected);
		strcpy(current_video_device_index,video_device_index_selected);
		dev->cam_device 	= current_video_device;		//CAMERA_DEVNAME_DEFAULT;
	}
	else{
		strcpy(current_video_device,argv[1]);
		strcpy(current_video_device_index,argv[1]+5);
		dev->cam_device = argv[1];
	}
	printf("  %s     %s    \n",current_video_device,current_video_device_index);

	if(set_memory_type(dev)<0)
	{
		printf("%s(): Unable to set the memory type\n", __func__);
		goto cleanup;
	}
	dev->no_of_camera = SINGLE;
	////open camera device
	ret_val	= open_camera_device(dev);
	if(ret_val < 0)	{
		printf("%s(): Unable to open the selected device, please check the device connection\n", __func__);
		goto cleanup;
	}

	/* Get the supported pixel formats */
	ret_val = get_supported_pixel_formats(dev);
	if(ret_val < 0)	{
		printf(" Unable to enumerate the supported pixel formats\n");
		goto cleanup;
	}
	
	//setting the current pixel format to first pixel format supported by the camera. 
	dev->stream_fmt.fmt.pix.pixelformat = dev->ffmt[0].pixelformat;	
	dev->still_fmt.fmt.pix.pixelformat = dev->ffmt[0].pixelformat;		
	max_image_width = 0;
	max_image_height = 0;

/* Get the supported resolutions formats */
	ret_val	= get_supported_resolution_formats(dev);
	if(ret_val < 0)	{
		printf("%s(): Unable to enumerate the supported resolution formats\n", __func__);
		goto cleanup;
	}
	//setting the current resolution format to first resolution format supported by the camera
	dev->stream_fmt.fmt.pix.width = dev->fsize[0].discrete.width;
	dev->stream_fmt.fmt.pix.height = dev->fsize[0].discrete.height;

	dev->still_fmt.fmt.pix.width =	dev->fsize[0].discrete.width;	
	dev->still_fmt.fmt.pix.height = dev->fsize[0].discrete.height;	

	ret_val = get_supported_controls(dev);
	if(ret_val < 0)	{
		printf("%s(): Unable to enumerate the supported camera controls\n", __func__);
		goto cleanup;
	}


	/* Allocate buffers to store images */
	
	dev->rgb_888_buffer	= calloc(max_image_width*max_image_height*3,sizeof(char));	
	if(dev->rgb_888_buffer	== NULL) {
		printf("%s(): unable to allocate memory\n", __func__);
		goto cleanup;
	}

	dev->raw_read_buffer	= calloc(max_image_width * max_image_height*2,sizeof(char));	
	if(dev->raw_read_buffer	== NULL) {
		printf("%s(): unable to allocate memory\n", __func__);
		goto cleanup;
	}


	//Closing the camera and display, will be opened again while streaming

	if(dev->camera_type == CAMERA_USB_TYPE)
		close_camera_device(dev);
	
	close_display_device(dev);

	return SUCCESS;

cleanup:
	if(dev->cam_fd >= 0) {
		close(dev->cam_fd);
	}

	return FAILURE;
}

int get_supported_controls(device_data *dev)
{
	int ControlIndex = 0;
	int total_wb_supprted = 0;
	
	total_focus_supported = 0;

	total_ctrl_supported = 0;
	total_exposure_supported = 0;	
	
	for(ControlIndex = V4L2_CID_BRIGHTNESS; ControlIndex <= V4L2_CID_LASTP1; ControlIndex++)
	{
		dev->qctrl.id = ControlIndex;
		if(ioctl(dev->cam_fd, VIDIOC_QUERYCTRL, &dev->qctrl) < 0)
		{
			continue;
		}
		else
		{
			if(dev->qctrl.id != V4L2_CID_AUTO_WHITE_BALANCE)
			{
				supported_ctrl[total_ctrl_supported].id = dev->qctrl.id;
				supported_ctrl[total_ctrl_supported].type = dev->qctrl.type;
				memcpy(supported_ctrl[total_ctrl_supported].name,dev->qctrl.name,32);
				supported_ctrl[total_ctrl_supported].minimum = dev->qctrl.minimum;
				supported_ctrl[total_ctrl_supported].maximum = dev->qctrl.maximum;
				supported_ctrl[total_ctrl_supported].step = dev->qctrl.step;
				supported_ctrl[total_ctrl_supported].default_value = dev->qctrl.default_value;
				supported_ctrl[total_ctrl_supported].flags = dev->qctrl.flags;
				total_ctrl_supported++;
			}
			if((dev->qctrl.id == V4L2_CID_AUTO_WHITE_BALANCE) || (dev->qctrl.id == V4L2_CID_WHITE_BALANCE_TEMPERATURE))
			{
				white_balamce_ctrl[total_wb_supprted].id = dev->qctrl.id;
				white_balamce_ctrl[total_wb_supprted].type = dev->qctrl.type;
				memcpy(white_balamce_ctrl[total_wb_supprted].name,dev->qctrl.name,32);
				white_balamce_ctrl[total_wb_supprted].minimum = dev->qctrl.minimum;
				white_balamce_ctrl[total_wb_supprted].maximum = dev->qctrl.maximum;
				white_balamce_ctrl[total_wb_supprted].step = dev->qctrl.step;
				white_balamce_ctrl[total_wb_supprted].default_value = dev->qctrl.default_value;
				white_balamce_ctrl[total_wb_supprted].flags = dev->qctrl.flags;
				total_wb_supprted++;
			}
		}
	}
	for(ControlIndex = V4L2_CID_EXPOSURE_AUTO; ControlIndex <= V4L2_CID_PRIVACY; ControlIndex++)
	{
		dev->qctrl.id = ControlIndex;
		if(ioctl(dev->cam_fd, VIDIOC_QUERYCTRL, &dev->qctrl) < 0)
		{
			continue;
		}
		else
		{
			if((dev->qctrl.id != V4L2_CID_AUTO_FOCUS_START) && (dev->qctrl.id != V4L2_CID_AUTO_FOCUS_STOP) && (dev->qctrl.id != V4L2_CID_FOCUS_AUTO) && (dev->qctrl.id != V4L2_CID_EXPOSURE_AUTO))
			{
				supported_ctrl[total_ctrl_supported].id = dev->qctrl.id;
				supported_ctrl[total_ctrl_supported].type = dev->qctrl.type;
				memcpy(supported_ctrl[total_ctrl_supported].name,dev->qctrl.name,32);
				supported_ctrl[total_ctrl_supported].minimum = dev->qctrl.minimum;
				supported_ctrl[total_ctrl_supported].maximum = dev->qctrl.maximum;
				supported_ctrl[total_ctrl_supported].step = dev->qctrl.step;
				supported_ctrl[total_ctrl_supported].default_value = dev->qctrl.default_value;
				supported_ctrl[total_ctrl_supported].flags = dev->qctrl.flags;
				total_ctrl_supported++;
			}
			if((dev->qctrl.id == V4L2_CID_AUTO_FOCUS_START) || (dev->qctrl.id == V4L2_CID_AUTO_FOCUS_STOP)|| (dev->qctrl.id == V4L2_CID_FOCUS_ABSOLUTE) || (dev->qctrl.id == V4L2_CID_FOCUS_AUTO))
			{
				focus_ctrl[total_focus_supported].id = dev->qctrl.id;
				focus_ctrl[total_focus_supported].type = dev->qctrl.type;
				memcpy(focus_ctrl[total_focus_supported].name,dev->qctrl.name,32);
				focus_ctrl[total_focus_supported].minimum = dev->qctrl.minimum;
				focus_ctrl[total_focus_supported].maximum = dev->qctrl.maximum;
				focus_ctrl[total_focus_supported].step = dev->qctrl.step;
				focus_ctrl[total_focus_supported].default_value = dev->qctrl.default_value;
				focus_ctrl[total_focus_supported].flags = dev->qctrl.flags;
				total_focus_supported++;
			}
			if((dev->qctrl.id == V4L2_CID_EXPOSURE_AUTO) || (dev->qctrl.id == V4L2_CID_EXPOSURE_ABSOLUTE))
			{
				exposure_ctrl[total_exposure_supported].id = dev->qctrl.id;
				exposure_ctrl[total_exposure_supported].type = dev->qctrl.type;
				memcpy(exposure_ctrl[total_exposure_supported].name,dev->qctrl.name,32);
				exposure_ctrl[total_exposure_supported].minimum = dev->qctrl.minimum;
				exposure_ctrl[total_exposure_supported].maximum = dev->qctrl.maximum;
				exposure_ctrl[total_exposure_supported].step = dev->qctrl.step;
				exposure_ctrl[total_exposure_supported].default_value = dev->qctrl.default_value;
				exposure_ctrl[total_exposure_supported].flags = dev->qctrl.flags;
				total_exposure_supported++;

			}	
		}
	}

	return SUCCESS;
}


int initial_launch(device_data *dev)
{
	int i = 0;

			dev->stream_fmt.fmt.pix.width = dev->fsize[i].discrete.width;
			dev->stream_fmt.fmt.pix.height = dev->fsize[i].discrete.height;

	dev->process.bit.stream_mode = ENABLE;
	if(stream_on(dev) < 0) {
		dev->process.bit.stream_mode = DISABLE;
		printf("%s:Unable to start streaming\n",__func__);
		return FAILURE;
	}
	return SUCCESS;	
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_20
 *
 *  Name	:	get_supported_resolution_formats
 *  Parameter1	:	device_data *dev - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAILURE	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Get the Available dimension in the driver
 ************************************************************************************************************/
int get_supported_resolution_formats(device_data *dev)
{
	int i = 0;
	for (i=0 ;i< MAX_FRAME_SIZE_SUPPORT; i++)
	{
		dev->fsize[i].index		= i;
		dev->fsize[i].pixel_format	= dev->stream_fmt.fmt.pix.pixelformat;	//V4L2_PIX_FMT_UYVY;
		if (ioctl(dev->cam_fd, VIDIOC_ENUM_FRAMESIZES, &dev->fsize[i]) < 0)
		{
			break;
		} 

		if(max_image_width < dev->fsize[i].discrete.width)		
			max_image_width = dev->fsize[i].discrete.width;
		
		if(max_image_height < dev->fsize[i].discrete.height)
			max_image_height = dev->fsize[i].discrete.height;

		dev->total_frm_support = i+1;
	}
	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_20
 *
 *  Name	:	get_supported_pixel_formats
 *  Parameter1	:	device_data *dev - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAILURE	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Get the Available dimension in the driver
 ************************************************************************************************************/
int get_supported_pixel_formats(device_data *dev)
{
	int i = 0;
	for (i=0 ;i< MAX_PIXELFORMAT_SUPPORT; i++)
	{
		dev->ffmt[i].index		= i;
		dev->ffmt[i].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (ioctl(dev->cam_fd, VIDIOC_ENUM_FMT, &dev->ffmt[i]) < 0)
		{
			break;
		} 
		dev->total_pixfmt_support = i+1;
	}
	return SUCCESS;
}


int get_frame_intervals(device_data *dev)
{
	int framinval_index = 0;
	int close_camera = 0;
	
	if(dev->camera_type == CAMERA_USB_TYPE)
	{	
		if(dev->cam_fd < 0)
		{	
			if(open_camera_device(dev)<0)
			{
				printf("%s(): Unable to enumerate the supported resolution formats\n", __func__);
				return FAILURE;
			}
			close_camera = 1;
		}
	}

	for(;;)
	{	
		dev->frminterval.index = framinval_index;
		dev->frminterval.width = dev->stream_fmt.fmt.pix.width;
		dev->frminterval.height = dev->stream_fmt.fmt.pix.height;
		dev->frminterval.pixel_format = dev->stream_fmt.fmt.pix.pixelformat;
		if(ioctl(dev->cam_fd, VIDIOC_ENUM_FRAMEINTERVALS, &dev->frminterval) < 0) {
			break;
		}
		if(dev->frminterval.type == V4L2_FRMIVAL_TYPE_DISCRETE)
		{
			dev->desc_frm_inval[framinval_index].numerator = dev->frminterval.discrete.numerator;
			dev->desc_frm_inval[framinval_index].denominator = dev->frminterval.discrete.denominator;
		}
		framinval_index ++;
	}
	
	if(dev->camera_type == CAMERA_USB_TYPE)
	{
		if(close_camera == 1)
		{
			close_camera_device(dev);
			close_camera = 0;
		}
	}
	return framinval_index;	
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_20
 *
 *  Name	:	validate_dimenstion
 *  Parameter1	:	device_data *dev - Address of Base pointer of device structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAILURE	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Get the Available dimension in the driver
 ************************************************************************************************************/
int validate_dimension(device_data *dev, unsigned int width, unsigned int height)
{
	unsigned int i;
	for(i=0;i<dev->total_frm_support; i++)
	{
		if((dev->fsize[i].discrete.width == width) && (dev->fsize[i].discrete.height == height))
			return SUCCESS;
	}
	return FAILURE;
}

int get_display_resolution(device_data *dev)
{
	struct v4l2_cropcap	disp_cropcap;
	disp_cropcap.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if(ioctl(dev->disp_fd, VIDIOC_CROPCAP, &disp_cropcap) < 0) {
		perror("DISPLAY: VIDIOC_CROPCAP");
		return FAILURE;
	}
	dev->display_width = disp_cropcap.bounds.width;	
	dev->display_height = disp_cropcap.bounds.height;
	return SUCCESS;
}



/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_05
 *
 *  Name	:	take_snap
 *  Parameter1	:	device_data *dev - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAILURE	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Take a picture
 ************************************************************************************************************/

int take_snap(device_data *dev)
{
	int skip_count = 6, i;
	int skip_frame = 0;
	int count = 0;
	char still_file[64];
	unsigned int time;
	int frame_count = dev->frame_count;
	
	struct v4l2_input input;
	int index;
	
	struct v4l2_capability capability;
	gettime(&time,NO_DATA);
	if(dev->camera_type == CAMERA_USB_TYPE)
	{
		if(open_camera_device(dev)<0)
		{
			printf("%s(): Unable to enumerate the supported resolution formats\n", __func__);
			goto cleanup;
		}
	}

	if(capture_prepare_streaming(dev) < 0) {
		printf(" Capture_prepare_streaming failed \n");
		goto cleanup;
	}

	if (ioctl(dev->cam_fd, VIDIOC_STREAMON, &dev->cam_type) < 0) {
                perror(" CAPTURE: VIDIOC_STREAMON");
               	goto cleanup;
        }

	if(dev->no_of_camera != SINGLE){
		if (ioctl(dev->cam_fd1, VIDIOC_STREAMON, &dev->cam_type) < 0) {
			perror(" CAPTURE: VIDIOC_STREAMON");
			goto cleanup;
		}
		if(pthread_create(&dev->save_process_tid, NULL, (void *(*)(void *))save_video_mmap, (void *)dev) < 0) {
			printf("Failed to create video thread for streaming camera\n");
			goto cleanup; 
		}
	}

	if(dev->no_of_camera == SINGLE){
		while (skip_count--) {
			memset(&dev->cam_buf, 0, sizeof (dev->cam_buf));
			dev->cam_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			dev->cam_buf.memory = V4L2_MEMORY_MMAP;

			/* dequeue buffer */
			if (ioctl (dev->cam_fd, VIDIOC_DQBUF, &dev->cam_buf) < 0) {
				perror(" CAPTURE: VIDIOC_DQBUF \n");
				goto cleanup;
			}

			/* enqueue buffer */
			if (ioctl (dev->cam_fd, VIDIOC_QBUF, &dev->cam_buf) < 0) {
				perror(" CAPTURE: VIDIOC_DQBUF \n");
				goto cleanup;
			}
		}

		memset(&dev->cam_buf, 0, sizeof (dev->cam_buf));
		dev->cam_buf.type = dev->cam_type;
		dev->cam_buf.memory = V4L2_MEMORY_MMAP;
		if (ioctl (dev->cam_fd, VIDIOC_DQBUF, &dev->cam_buf) < 0) {
			perror(" CAPTURE: VIDIOC_DQBUF \n");
			goto cleanup;
		}
		printf("\n\n\nBYTES USED%d\n\n\n",dev->cam_buf.bytesused);
		gettime(&time,NO_DATA);
		printf("\nStill Image received time stamp.... %u ms\n",time);

		save_snap(dev, (unsigned char*)dev->buffers[dev->cam_buf.index].cam_start);
	}
	else
	{
		for(count=0;;count++)
		{
			sprintf(still_file,"/home/cam0_video_%dx%d_%d.yuv",	dev->fmt.fmt.pix.width,	dev->fmt.fmt.pix.height, count);
			if (((dev->save_fd = open(still_file, O_RDONLY)) < 0)) {
				dev->save_fd = open(still_file, O_WRONLY | O_CREAT | O_SYNC, S_IRWXU | S_IRGRP | S_IROTH);
				if(dev->save_fd < 0) {
					perror("open :");
					return FAILURE;
				}
				break;
			}
			close(dev->save_fd);
		}
		while (++skip_frame) {

				memset(&dev->cam_buf, 0, sizeof (dev->cam_buf));
				dev->cam_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				dev->cam_buf.memory = V4L2_MEMORY_MMAP;

				/* dequeue buffer */
				if (ioctl (dev->cam_fd, VIDIOC_DQBUF, &dev->cam_buf) < 0) {
				perror(" CAPTURE: VIDIOC_DQBUF \n");
				goto cleanup;
				}
				if(skip_frame > 10)
				write(dev->save_fd, dev->buffers[dev->cam_buf.index].cam_start, dev->fmt.fmt.pix.width * dev->fmt.fmt.pix.height *2);

				/* enqueue buffer */
				if (ioctl (dev->cam_fd, VIDIOC_QBUF, &dev->cam_buf) < 0) {
				perror(" CAPTURE: VIDIOC_DQBUF \n");
				goto cleanup;
				}

				if(skip_frame > 10){ 
					if(frame_count-- == 0)
						break;
				}
		}
		printf("\nFile %s recorded successfully .... \n",still_file);
		close(dev->save_fd);

	}
cleanup:

	if(dev->buffers != NULL) {
		/* unmmap all buffers */
		for (i = 0; i < TEST_BUFFER_NUM; i++) {
			if(dev->buffers[i].cam_start)
				munmap(dev->buffers[i].cam_start, dev->buffers[i].cam_length);
		}
	}

	
	/* stop streaming io */
        if (ioctl(dev->cam_fd, VIDIOC_STREAMOFF, &dev->cam_type) < 0) {
                perror("CAPTURE: VIDIOC_STREAMOFF \n");
                goto cleanup;
        }
	if(dev->camera_type == CAMERA_USB_TYPE)
		close_camera_device(dev);
	
	return 0;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	
 *
 *  Name	:	stream_off
 *  Parameter1	:	device_data *dev - Address of Base pointer of device structure
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	This function shutdown the streaming
 ************************************************************************************************************/

int stream_off(device_data *dev)
{
	int ret_val, i;
	if(dev->process.bit.stream_progress == DISABLE) {
		return SUCCESS;
	}

	dev->kill.thread.stream_thread_kill = ENABLE;

	if(dev->stream_process_tid) {
		pthread_join(dev->stream_process_tid, NULL);
	}

	if(dev->save_process_tid) {
		pthread_join(dev->save_process_tid, NULL);
	}

	dev->cam_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (ioctl(dev->cam_fd, VIDIOC_STREAMOFF, &dev->cam_type) < 0) {
		perror("VIDIOC_STREAMOFF");
		return FAILURE;
	}
		
	if((dev->disp_fd > DISABLE) && (dev->process.bit.dss_stream_on == ENABLE)) {
		dev->process.bit.dss_stream_on = DISABLE;
		
	}
	dev->stream_process_tid = 0;
	dev->process.bit.stream_progress = DISABLE;

	close_display_device(dev);

	if(dev->camera_type == CAMERA_USB_TYPE)
		close_camera_device(dev);

	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	
 *
 *  Name	:	stream_on
 *  Parameter1	:	
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	This function select the streaming input and start the stream
 ************************************************************************************************************/

int stream_on(device_data *dev)
{
	int ret_val;
	unsigned int time;
	struct v4l2_input input;
	int index;
	
	struct v4l2_capability capability;
	

	if(dev->process.bit.stream_progress == ENABLE) {
		return SUCCESS;
	}

	if(dev->camera_type == CAMERA_USB_TYPE)
	{	
		if(open_camera_device(dev)<0)
		{
			printf("%s(): Unable open camera device\n", __func__);
			goto cleanup;
		}
	}

	dev->kill.thread.stream_thread_kill = DISABLE;

	if(capture_prepare_streaming(dev) < 0) {
		printf(" Capture_prepare_streaming failed \n");
		goto cleanup;
	}

	if(dev->process.bit.stream_mode == ENABLE) {
		if(display_prepare_streaming(dev) < 0) {
			printf(" Display_prepare_streaming failed \n");
			goto cleanup;
		}
	}

	if (ioctl(dev->cam_fd, VIDIOC_STREAMON, &dev->cam_type) < 0) {
                perror(" CAPTURE: VIDIOC_STREAMON");
               	goto cleanup;
        }
	dev->process.bit.stream_progress = ENABLE;

	
	if(dev->memory_type == V4L2_MEMORY_USERPTR)
	{
		if(pthread_create(&dev->stream_process_tid, NULL, (void *(*)(void *))stream_video_userptr, (void *)dev) < 0) {
			printf("Failed to create video thread for streaming camera\n");
			goto cleanup; 
		}
	}
	else
	{
		if(dev->process.bit.dss_stream_on == DISABLE) 
		{ 	
			dev->process.bit.dss_stream_on = ENABLE;
			if (ioctl(dev->disp_fd, VIDIOC_STREAMON, &dev->disp_type) < 0) {
				perror("DISPLAY: VIDIOC_STREAMON");
			}
		}
		
		if(pthread_create(&dev->stream_process_tid, NULL, (void *(*)(void *))stream_video_mmap, (void *)dev) < 0) {
			printf("Failed to create video thread for streaming camera\n");
			goto cleanup; 
		}

		
	}
	return SUCCESS;
	
cleanup:
	
	return FAILURE;
}


int find_capture_mode(device_data *dev) 
{
	int i = 0;

	if(dev->process.bit.snap_mode == ENABLE)
	{
		for(i=0;i<dev->total_frm_support; i++)
		{
			if((dev->fsize[i].discrete.width == dev->still_fmt.fmt.pix.width) && (dev->fsize[i].discrete.height == dev->still_fmt.fmt.pix.height))
			{
				dev->capture_mode =i;
				break;
			}
		}

		if(i > dev->total_frm_support)
			return FAILURE;		
		
	} 
	else 
	{
		for(i=0;i<dev->total_frm_support; i++)
		{
			if((dev->fsize[i].discrete.width == dev->stream_fmt.fmt.pix.width) && (dev->fsize[i].discrete.height == dev->stream_fmt.fmt.pix.height))
			{
				dev->capture_mode =i;
				break;
			}
		}

		if(i > dev->total_frm_support)
			return FAILURE;
	}

	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	
 *
 *  Name	:	capture_prepare_streaming
 *  Parameter1	:	INT32 cam_fd - Camera file descriptor which need to be processed
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	This function initializes capture device. It selects an active input
			and detects the standard on that input. It then allocates buffers in the
			driver's memory space and mmaps them in the application space.
 ************************************************************************************************************/

int capture_prepare_streaming(device_data *dev)
{
	int ret_val, i;
	int err_chk;
	struct v4l2_frmivalenum frmivalenum;
	unsigned int time = 0;

	dev->cam_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	dev->fmt.type = dev->cam_type;

	if (ioctl(dev->cam_fd, VIDIOC_G_FMT, &dev->fmt) < 0) {
        	perror("CAPTURE: VIDIOC_G_FMT");
		goto cleanup;
        }

	if(dev->no_of_camera != SINGLE){
		if (ioctl(dev->cam_fd1, VIDIOC_G_FMT, &dev->fmt) < 0) {
			perror("CAPTURE: VIDIOC_G_FMT");
			goto cleanup;
		}
	}
	if(dev->process.bit.snap_mode == ENABLE) {
		dev->fmt.fmt.pix.width = dev->still_fmt.fmt.pix.width;
		dev->fmt.fmt.pix.height = dev->still_fmt.fmt.pix.height;
		dev->fmt.fmt.pix.pixelformat = dev->still_fmt.fmt.pix.pixelformat;
		dev->fmt.fmt.pix.bytesperline = dev->fmt.fmt.pix.width * 2;
		dev->cam_fps = 15;
	} else {
		dev->fmt.fmt.pix.width = dev->stream_fmt.fmt.pix.width;
		dev->fmt.fmt.pix.height = dev->stream_fmt.fmt.pix.height;
		dev->fmt.fmt.pix.pixelformat = dev->stream_fmt.fmt.pix.pixelformat;
		dev->fmt.fmt.pix.bytesperline = dev->fmt.fmt.pix.width * 2;

	}

	if (ioctl(dev->cam_fd, VIDIOC_S_FMT, &dev->fmt) < 0) {
        	perror("CAPTURE: VIDIOC_S_FMT");
		goto cleanup;
        }

	if(dev->no_of_camera != SINGLE){
		if (ioctl(dev->cam_fd1, VIDIOC_S_FMT, &dev->fmt) < 0) {
			perror("CAPTURE: VIDIOC_S_FMT");
			goto cleanup;
		}
	}
	/*if (ioctl(dev->cam_fd, VIDIOC_G_FMT, &dev->fmt) < 0) {
        	perror("CAPTURE: VIDIOC_G_FMT");
		goto cleanup;
        }*/

	/*if(find_capture_mode(dev) < 0)
	{
		printf("Current resolution is not supported by the driver\n");
		goto cleanup;
	}*/

	memset(&dev->cam_parm, 0x0, sizeof(struct v4l2_streamparm));
	dev->cam_type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	dev->cam_parm.type = dev->cam_type;
	if(ioctl(dev->cam_fd, VIDIOC_G_PARM, &dev->cam_parm) < 0) {
		perror("CAPTURE: VIDIOC_G_PARM"); 
		goto cleanup;
	}
	dev->cam_type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	dev->cam_parm.type = dev->cam_type;
	if(dev->process.bit.snap_mode == ENABLE) 
	{
		dev->cam_parm.parm.capture.timeperframe.numerator = dev->desc_frm_inval[0].numerator;	
		dev->cam_parm.parm.capture.timeperframe.denominator = dev->desc_frm_inval[0].denominator;
	}
	else
	{
		dev->cam_parm.parm.capture.timeperframe.numerator = dev->stream_frm_inval.numerator;
		dev->cam_parm.parm.capture.timeperframe.denominator = dev->stream_frm_inval.denominator;
	}
	//dev->cam_parm.parm.capture.capturemode = dev->capture_mode;

	if(ioctl(dev->cam_fd, VIDIOC_S_PARM, &dev->cam_parm) < 0) {
		perror("CAPTURE: VIDIOC_S_PARM"); 
		goto cleanup;
	}
	//dev->fmt.fmt.pix.sizeimage = dev->fmt.fmt.pix.width * dev->fmt.fmt.pix.height * 2;
	dev->cam_type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	dev->cam_parm.type = dev->cam_type;
	if(ioctl(dev->cam_fd, VIDIOC_G_PARM, &dev->cam_parm) < 0) {
		perror("CAPTURE: VIDIOC_G_PARM"); 
		goto cleanup;
	}

	
	if(dev->no_of_camera != SINGLE){
		memset(&dev->cam_parm, 0x0, sizeof(struct v4l2_streamparm));
		dev->cam_type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		dev->cam_parm.type = dev->cam_type;
		if(ioctl(dev->cam_fd1, VIDIOC_G_PARM, &dev->cam_parm) < 0) {
			perror("CAPTURE: VIDIOC_G_PARM"); 
			goto cleanup;
		}
		dev->cam_type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		dev->cam_parm.type = dev->cam_type;
		if(dev->process.bit.snap_mode == ENABLE) 
		{
			dev->cam_parm.parm.capture.timeperframe.numerator = dev->desc_frm_inval[0].numerator;	
			dev->cam_parm.parm.capture.timeperframe.denominator = dev->desc_frm_inval[0].denominator;
		}
		else
		{
			dev->cam_parm.parm.capture.timeperframe.numerator = dev->stream_frm_inval.numerator;
			dev->cam_parm.parm.capture.timeperframe.denominator = dev->stream_frm_inval.denominator;
		}
		//dev->cam_parm.parm.capture.capturemode = dev->capture_mode;

		if(ioctl(dev->cam_fd1, VIDIOC_S_PARM, &dev->cam_parm) < 0) {
			perror("CAPTURE: VIDIOC_S_PARM"); 
			goto cleanup;
		}
		//dev->fmt.fmt.pix.sizeimage = dev->fmt.fmt.pix.width * dev->fmt.fmt.pix.height * 2;
		dev->cam_type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		dev->cam_parm.type = dev->cam_type;
		if(ioctl(dev->cam_fd1, VIDIOC_G_PARM, &dev->cam_parm) < 0) {
			perror("CAPTURE: VIDIOC_G_PARM"); 
			goto cleanup;
		}
	}


	/* Buffer allocation
	 * Buffer can be allocated either from capture driver or
	 * user pointer can be used
	 */
	/* Request for MAX_BUFFER input buffers. As far as Physically contiguous
	 * memory is available, driver can allocate as many buffers as
	 * possible. If memory is not available, it returns number of
	 * buffers it has allocated in count member of reqbuf.
	 * HERE count = number of buffer to be allocated.
	 * type = type of device for which buffers are to be allocated.
	 * memory = type of the buffers requested i.e. driver allocated or
	 * user pointer */
	memset(&dev->cam_req, 0, sizeof (dev->cam_req));
	dev->cam_req.count = TEST_BUFFER_NUM;
	dev->cam_req.type = dev->cam_type;
	dev->cam_req.memory = V4L2_MEMORY_MMAP;
	if (ioctl(dev->cam_fd, VIDIOC_REQBUFS, &dev->cam_req) < 0) {
		perror("CAPTURE: VIDIOC_REQBUFS");
		goto cleanup;
	}
	/* Mmap the buffers
	 * To access driver allocated buffer in application space, they have
	 * to be mmapped in the application space using mmap system call */
	for (i = DISABLE; i < dev->cam_req.count; i++)	{
		memset(&dev->cam_buf,DISABLE, sizeof(dev->cam_buf));
		dev->cam_buf.type = dev->cam_type;
		dev->cam_buf.index = i;
		dev->cam_buf.memory = V4L2_MEMORY_MMAP;
		
		if((err_chk=ioctl(dev->cam_fd, VIDIOC_QUERYBUF, &dev->cam_buf)) < 0) {
			printf("ERROR CHECKING = %d\n",err_chk);
			perror(" CAPTURE: VIDIOC_QUERYBUF");
			goto cleanup;
		}
		dev->buffers[i].cam_offset = (size_t) dev->cam_buf.m.offset;
		dev->buffers[i].cam_length = dev->cam_buf.length;
		dev->buffers[i].cam_start = mmap(NULL, dev->cam_buf.length,
					     PROT_READ | PROT_WRITE, MAP_SHARED, dev->cam_fd,
					     dev->buffers[i].cam_offset);
		if (dev->buffers[i].cam_start == MAP_FAILED) {
			printf(" Cannot mmap = %d buffer\n", i);
			goto cleanup;
		}
		/* Enqueue buffers
		 * Before starting streaming, all the buffers needs to be
		 * en-queued in the driver incoming queue. These buffers will
		 * be used by thedrive for storing captured frames. */
		if(ioctl(dev->cam_fd, VIDIOC_QBUF, &dev->cam_buf) < 0) {
			perror("CAPTURE: VIDIOC_QBUF");
			goto cleanup;
		}
		
	}

	if(dev->no_of_camera != SINGLE){
		/* Buffer allocation
	 * Buffer can be allocated either from capture driver or
	 * user pointer can be used
	 */
	/* Request for MAX_BUFFER input buffers. As far as Physically contiguous
	 * memory is available, driver can allocate as many buffers as
	 * possible. If memory is not available, it returns number of
	 * buffers it has allocated in count member of reqbuf.
	 * HERE count = number of buffer to be allocated.
	 * type = type of device for which buffers are to be allocated.
	 * memory = type of the buffers requested i.e. driver allocated or
	 * user pointer */
	memset(&dev->cam_req, 0, sizeof (dev->cam_req));
	dev->cam_req.count = TEST_BUFFER_NUM;
	dev->cam_req.type = dev->cam_type;
	dev->cam_req.memory = V4L2_MEMORY_MMAP;
	if (ioctl(dev->cam_fd1, VIDIOC_REQBUFS, &dev->cam_req) < 0) {
		perror("CAPTURE: VIDIOC_REQBUFS");
		goto cleanup;
	}
	/* Mmap the buffers
	 * To access driver allocated buffer in application space, they have
	 * to be mmapped in the application space using mmap system call */
	for (i = DISABLE; i < dev->cam_req.count; i++)	{
		memset(&dev->cam_buf1,DISABLE, sizeof(dev->cam_buf1));
		dev->cam_buf1.type = dev->cam_type;
		dev->cam_buf1.index = i;
		dev->cam_buf1.memory = V4L2_MEMORY_MMAP;
		
		if((err_chk=ioctl(dev->cam_fd1, VIDIOC_QUERYBUF, &dev->cam_buf1)) < 0) {
			printf("ERROR CHECKING = %d\n",err_chk);
			perror(" CAPTURE: VIDIOC_QUERYBUF");
			goto cleanup;
		}
		dev->buffers1[i].cam_offset = (size_t) dev->cam_buf1.m.offset;
		dev->buffers1[i].cam_length = dev->cam_buf1.length;
		dev->buffers1[i].cam_start = mmap(NULL, dev->cam_buf1.length,
					     PROT_READ | PROT_WRITE, MAP_SHARED, dev->cam_fd1,
					     dev->buffers[i].cam_offset);
		if (dev->buffers1[i].cam_start == MAP_FAILED) {
			printf(" Cannot mmap = %d buffer\n", i);
			goto cleanup;
		}
		/* Enqueue buffers
		 * Before starting streaming, all the buffers needs to be
		 * en-queued in the driver incoming queue. These buffers will
		 * be used by thedrive for storing captured frames. */
		if(ioctl(dev->cam_fd1, VIDIOC_QBUF, &dev->cam_buf1) < 0) {
			perror("CAPTURE: VIDIOC_QBUF");
			goto cleanup;
		}
		
	}

	}
	return SUCCESS;

cleanup:

	return FAILURE;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	
 *
 *  Name	:	display_prepare_streaming
 *  Parameter1	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	This function initializes display device. It sets output and standard for 
			LCD. These output and standard are same as those detected in capture device.
			It, then, allocates buffers in the driver's memory space and mmaps them in
			the application space
 ************************************************************************************************************/

int display_prepare_streaming(device_data *dev)
{
	SDL_Color colors[256];
    char driver[256];
    const SDL_VideoInfo *info;
    int i;
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) {
        printf("Unable to initialize SDL\n");
        return -1;
    }

    SDL_VideoDriverName(driver, sizeof(driver));   
    printf("Video driver: %s\n", driver);
    printf("obtained video mode %dx%d depth %d format %c%c%c%c \n",dev->stream_fmt.fmt.pix.width, dev->stream_fmt.fmt.pix.height, 32,
            (dev->stream_fmt.fmt.pix.pixelformat & 0xff),
            (dev->stream_fmt.fmt.pix.pixelformat >> 8) & 0xff,
            (dev->stream_fmt.fmt.pix.pixelformat >> 16) & 0xff,
            (dev->stream_fmt.fmt.pix.pixelformat >> 24) & 0xff);

    if ((dev->surface = SDL_SetVideoMode(dev->stream_fmt.fmt.pix.width, dev->stream_fmt.fmt.pix.height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF )) == NULL) {
        printf("Unable to obtain video mode %dx%dx%d\n",
                dev->stream_fmt.fmt.pix.width, dev->stream_fmt.fmt.pix.height, 32);
        return -1;
    }

    dev->overlay = SDL_CreateYUVOverlay(dev->stream_fmt.fmt.pix.width, dev->stream_fmt.fmt.pix.height, SDL_YUY2_OVERLAY, dev->surface);
	printf("HW Acceleration = %d\n",dev->overlay->hw_overlay);
    SDL_ShowCursor(SDL_ENABLE);

    SDL_WM_SetCaption("Sample V4l2 Application", "Sample V4l2 Application");

    return 0;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	THREAD					MODULE ID	: 	
 *
 *  Name	:	stream_video
 *  Parameter1	:	PINT0 data
 *
 *  Returns	:	INT0 data - camera structure data 
 *  Description	: 	
 *  Comments	:  	 
 ************************************************************************************************************/
void *stream_video_userptr(void * args)
{
	device_data *dev = args;
	int stream_count = DISABLE;
   	unsigned int time = 0;
	int ret_val;
	signed int old_buf_index = -1;
	unsigned int i, fps = DISABLE, old_stream_count = DISABLE;
	int one_time = 0;
	struct timeval cur_tv, tv_start;
	struct timeval prev_tv;
	
	gettimeofday(&prev_tv, NULL);
	gettimeofday(&cur_tv, NULL);
	gettimeofday(&tv_start, NULL);
	
	for(;;stream_count++) {		
		if(dev->kill.thread.stream_thread_kill == ENABLE) {
			break;
		}

		memset(&dev->cam_buf, 0, sizeof (dev->cam_buf));
		dev->cam_buf.type = dev->cam_type;
		dev->cam_buf.memory = V4L2_MEMORY_MMAP;
		if (ioctl (dev->cam_fd, VIDIOC_DQBUF, &dev->cam_buf) < 0) {
			perror(" CAPTURE: VIDIOC_DQBUF \n");
			continue;
		}
		

		memset(&dev->disp_buf,0,sizeof(dev->disp_buf));

		dev->disp_buf.type = dev->disp_type;
		dev->disp_buf.memory = V4L2_MEMORY_USERPTR;
		dev->disp_buf.index = dev->cam_buf.index;	//
		dev->disp_buf.m.userptr = (unsigned long) dev->buffers[dev->cam_buf.index].cam_offset;
		dev->disp_buf.length = dev->buffers[dev->cam_buf.index].cam_length;

		dev->disp_buf.timestamp.tv_sec = tv_start.tv_sec;
	        dev->disp_buf.timestamp.tv_usec = tv_start.tv_usec + (1000000L / 30 * stream_count);
		if (ioctl(dev->disp_fd, VIDIOC_QBUF, &dev->disp_buf) < 0) {
	        	perror("DISPLAY: stream_video_userptr VIDIOC_QBUF failed");
	        }

                if(dev->process.bit.dss_stream_on == DISABLE) { // Start playback after buffers queued
			dev->process.bit.dss_stream_on = ENABLE;
			if (ioctl(dev->disp_fd, VIDIOC_STREAMON, &dev->disp_type) < 0) {
        	       		perror("DISPLAY: VIDIOC_STREAMON");
		        }
		}


		dev->disp_buf.type = dev->disp_type;
		dev->disp_buf.memory = V4L2_MEMORY_USERPTR;
		if(ioctl(dev->disp_fd, VIDIOC_DQBUF, &dev->disp_buf) < 0) {
			perror("DISPLAY: VIDIOC_DQBUF");
		}

		dev->cam_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		dev->cam_buf.memory = V4L2_MEMORY_MMAP;
		if (ioctl(dev->cam_fd, VIDIOC_QBUF, &dev->cam_buf) < 0){
			perror("CAPTURE: VIDIOC_QBUF");
		}


		gettimeofday(&cur_tv, NULL);
		if(cur_tv.tv_sec > prev_tv.tv_sec)
		{
			prev_tv	= cur_tv;
			fps	= stream_count - old_stream_count;
			old_stream_count = stream_count;
		}
		if(dev->show_frame_rate	== 1)
		{
			fprintf(stderr," \rFrames per second = %dfps ",fps);
		}
		if(dev->show_focus == 1)
		{
					dev->flag.bit.comm_ctrl	= READ;
					dev->ctrl.id = V4L2_CID_DRIVER_BASE+11;
					if(ctrl(dev) < 0) {
						printf("\n Unable to READ the control. Please try again \n");
						continue;
					}
					fprintf(stderr,"\rCurrent %s Value is %d",supported_ctrl[21].name,dev->ctrl.value);


        }
}

cleanup:

	//Release Capture and Display buffer
	if(dev->buffers != NULL) {
		for (i = DISABLE; i < TEST_BUFFER_NUM; i++) {
			if(dev->buffers[i].cam_start){
				munmap(dev->buffers[i].cam_start, dev->buffers[i].cam_length);
			}
			if(dev->buffers[i].disp_start)
				munmap(dev->buffers[i].disp_start, dev->buffers[i].disp_length);
			dev->buffers[i].cam_start = NULL;
			dev->buffers[i].disp_start = NULL;
		}
	}
	dev->kill.thread.stream_thread_kill	= ENABLE;
	pthread_exit(NULL);
}

void *save_video_mmap(void * args)
{
	device_data *dev = args;
	int i = 0;
	int skip_frame = 0;
	int count = 0;
	char still_file[64];
	int frame_count = dev->frame_count;
	for(count=0;;count++)
	{
		sprintf(still_file,"/home/cam1_video_%dx%d_%d.yuv",	dev->fmt.fmt.pix.width,	dev->fmt.fmt.pix.height, count);
		if (((dev->save_fd1 = open(still_file, O_RDONLY)) < 0)) {
			dev->save_fd1 = open(still_file, O_WRONLY | O_CREAT | O_SYNC, S_IRWXU | S_IRGRP | S_IROTH);
			if(dev->save_fd1 < 0) {
				perror("open :");
				return (void *)FAILURE;
			}
			break;
		}
		close(dev->save_fd1);
	}
	while (++skip_frame) {
			
			memset(&dev->cam_buf1, 0, sizeof (dev->cam_buf1));
			dev->cam_buf1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			dev->cam_buf1.memory = V4L2_MEMORY_MMAP;

			/* dequeue buffer */
			if (ioctl (dev->cam_fd1, VIDIOC_DQBUF, &dev->cam_buf1) < 0) {
			perror(" CAPTURE: VIDIOC_DQBUF \n");
			goto cleanup;
			}
			if(skip_frame > 10)
			write(dev->save_fd1, dev->buffers1[dev->cam_buf1.index].cam_start, dev->fmt.fmt.pix.width * dev->fmt.fmt.pix.height *2);

			/* enqueue buffer */
			if (ioctl (dev->cam_fd1, VIDIOC_QBUF, &dev->cam_buf1) < 0) {
			perror(" CAPTURE: VIDIOC_DQBUF \n");
			goto cleanup;
			}

			if(skip_frame > 10){ 
				if(dev->frame_count-- == 0)
					break;
			}
	}
	printf("\nFile %s recorded successfully ....\n",still_file);
	close(dev->save_fd1);
cleanup:
	if(dev->no_of_camera != SINGLE){
		/* stop streaming io */
		if (ioctl(dev->cam_fd1, VIDIOC_STREAMOFF, &dev->cam_type) < 0) {
			perror("CAPTURE1: VIDIOC_STREAMOFF \n");
			goto cleanup;
		}
		if(dev->buffers1 != NULL) {
			/* unmmap all buffers */
			for (i = 0; i < TEST_BUFFER_NUM; i++) {
				if(dev->buffers1[i].cam_start)
					munmap(dev->buffers1[i].cam_start, dev->buffers1[i].cam_length);
			}
		}
	}
	pthread_exit(NULL);	

}

void *stream_video_mmap(void * args)
{
	device_data *dev = args;
	int stream_count = DISABLE, time;
	int ret_val;
	signed int old_buf_index = -1;
	unsigned int i, fps = DISABLE, old_stream_count = DISABLE;
	char *p_data;
	int width,height;
	width = dev->stream_fmt.fmt.pix.width;
	height = dev->stream_fmt.fmt.pix.height;

	struct timeval cur_tv, tv_start,t,p;
	struct timeval prev_tv;
	char *cam_ptr, *disp_ptr;	
	dev->disp_buf.type = dev->disp_type;
	dev->disp_buf.memory = V4L2_MEMORY_MMAP;
	dev->disp_buf.index = 0;
	
	memset(&dev->cam_buf, 0, sizeof (dev->cam_buf));
	dev->cam_buf.type = dev->cam_type;
	dev->cam_buf.memory = V4L2_MEMORY_MMAP;
	dev->cam_buf.index = 0;
	
	gettimeofday(&prev_tv, NULL);
	gettimeofday(&cur_tv, NULL);
	gettimeofday(&tv_start, NULL);
	

	for(;;stream_count++) {		
		if(dev->kill.thread.stream_thread_kill == ENABLE) {
			break;
		}	
		//Dque cam buffer
		if (ioctl (dev->cam_fd, VIDIOC_DQBUF, &dev->cam_buf) < 0) {
			perror(" CAPTURE: VIDIOC_DQBUF \n");
			continue;
		}

		if (dev->overlay != NULL) {
        p_data = (unsigned char *) dev->overlay->pixels[0];
        dev->drect.x = 0;
        dev->drect.y = 0;
        dev->drect.w = width;
        dev->drect.h = height;
    }

    SDL_LockYUVOverlay(dev->overlay);
    memcpy(p_data, dev->buffers[dev->cam_buf.index].cam_start, width * height * 2);
    SDL_UnlockYUVOverlay(dev->overlay);
    SDL_DisplayYUVOverlay(dev->overlay, &dev->drect);

			//Que Cam buffer
		if (ioctl(dev->cam_fd, VIDIOC_QBUF, &dev->cam_buf) < 0){
			perror("CAPTURE: VIDIOC_QBUF");
		}

		gettimeofday(&cur_tv, NULL);
		if(cur_tv.tv_sec > prev_tv.tv_sec)
		{
			prev_tv	= cur_tv;
			fps	= stream_count - old_stream_count;
			old_stream_count = stream_count;
		}
		if(dev->show_frame_rate	== 1)
		{
			fprintf(stderr," \rFrames per second = %dfps ",fps);
		}
		if(dev->show_focus == 1)
		{
					dev->flag.bit.comm_ctrl	= READ;
					dev->ctrl.id = V4L2_CID_DRIVER_BASE+11;
					if(ctrl(dev) < 0) {
						printf("\n Unable to READ the control. Please try again \n");
						continue;
					}
					printf("\rCurrent %s Value is %d",supported_ctrl[22].name,dev->ctrl.value);


       		 }


        }

cleanup:

	//Release Capture and Display buffer
	if(dev->buffers != NULL) {
		for (i = DISABLE; i < TEST_BUFFER_NUM; i++) {
			if(dev->buffers[i].cam_start)
				munmap(dev->buffers[i].cam_start, dev->buffers[i].cam_length);
			if(dev->buffers[i].disp_start)
				munmap(dev->buffers[i].disp_start, dev->buffers[i].disp_length);
			dev->buffers[i].cam_start = NULL;
			dev->buffers[i].disp_start = NULL;
		}
	}
	dev->kill.thread.stream_thread_kill	= ENABLE;
	pthread_exit(NULL);	
}

int print_pixelformat(char *prefix, int val)
{
	printf("%s: %c%c%c%c\n", prefix ? prefix : "pixelformat",
					val & 0xff,
					(val >> 8) & 0xff,
					(val >> 16) & 0xff,
					(val >> 24) & 0xff);
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	
 *
 *  Name	:	feature_test_api_exit
 *  Parameter1	:	device_data *dev - Address of Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAILURE	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	This function initialize the feature test api module
 ************************************************************************************************************/
int feature_test_api_exit(device_data *dev)
{
	int ret_val;
	
	if(dev->disp_fd > 0) {
		close(dev->disp_fd);
		dev->disp_fd = -1;
	}

	if(dev->cam_fd > 0) {
		close(dev->cam_fd);
		dev->cam_fd = -1;
	}

	if(dev->raw_read_buffer)
	{
		free(dev->raw_read_buffer);
		dev->raw_read_buffer	= NULL;
	}

	if(dev->rgb_888_buffer)
	{
		free(dev->rgb_888_buffer);
		dev->rgb_888_buffer	= NULL;
	}

	if(register_device_data(&dev, UNREGISTER_CLEAR_ENTRY) < 0) {
		return FAILURE;
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION					MODULE ID	: FEATURE_TEST_02	
 *
 *  Name	:	save_snap
 *  Parameter1	:	
 *
 *  Returns	:	int 
 *  Description	: 	
 *  Comments	:  	This function will save the RGB888 by taking raw RGB565 data from the camera 
 ************************************************************************************************************/

int save_snap(device_data *dev, unsigned char *buffer)
{
	unsigned char * virtual_addr = 0;
	int count = 0, ret_val;
	unsigned int time;
	char still_file[64];
	virtual_addr = buffer;

	if(dev->flag.bit.save_file_format == SAVE_YUV) {
		for(count=0;;count++)
		{
			sprintf(still_file,"/home/snap_%dx%d_%d.yuv",	dev->fmt.fmt.pix.width,	dev->fmt.fmt.pix.height, count);
			if (((dev->save_fd = open(still_file, O_RDONLY)) < 0)) {
				dev->save_fd = open(still_file, O_WRONLY | O_CREAT | O_SYNC, S_IRWXU | S_IRGRP | S_IROTH);
				if(dev->save_fd < 0) {
					perror("open :");
					return FAILURE;
				}
				break;
			}
			close(dev->save_fd);
		}
		write(dev->save_fd, virtual_addr, dev->fmt.fmt.pix.width * dev->fmt.fmt.pix.height *2);
		close(dev->save_fd);
	} else {
		for(count=0;;count++) {
			sprintf(still_file,"/home/snap_%dx%d_%d.bmp",	dev->fmt.fmt.pix.width,	dev->fmt.fmt.pix.height, count);
			if (((dev->save_fd = open(still_file, O_RDONLY)) < 0 ))
			{
				dev->save_fd = open(still_file,  O_WRONLY | O_CREAT | O_SYNC, S_IRWXU | S_IRGRP | S_IROTH);
				if(dev->save_fd < 0) {
					perror("open :");
					return FAILURE;
				}
				break;
			}
			close(dev->save_fd);
		}


		if(dev->fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_UYVY)
		{
			if(convert_uyvy_rgb888(virtual_addr,dev->fmt.fmt.pix.width, dev->fmt.fmt.pix.height, dev->rgb_888_buffer) < 0)
			{

			}
		}
		else
		{	
			if(convert_uyvy_rgb888(virtual_addr,dev->fmt.fmt.pix.width, dev->fmt.fmt.pix.height, dev->rgb_888_buffer) < 0)
			{

			}
		}

		form_bmp_header_info(dev);

		write(dev->save_fd, (void *)&dev->bmp_header, sizeof(dev->bmp_header));
		write(dev->save_fd,dev->rgb_888_buffer, dev->fmt.fmt.pix.width * dev->fmt.fmt.pix.height *3);
		close(dev->save_fd);
	}
	dev->flag.bit.save_snap_completed	= ENABLE;
	gettime(&time,NO_DATA);
	printf("\nFile %s saved successfully .... %u ms\n",still_file, time);
	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_20
 *
 *  Name	:	get_camera_format
 *  Parameter1	:	cam_data *cam - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Get the using dimension in the driver for capture routine
 ************************************************************************************************************/
int get_camera_format(device_data *dev)
{
	dev->fmt.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(dev->cam_fd, VIDIOC_G_FMT, &dev->fmt) < 0)
	{
		perror("CAPTURE VIDIOC_G_FMT");
		return FAILURE;
	} 
	return SUCCESS;
}

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	
 *
 *  Name	:	gettime
 *  Parameter1	:	Address of time required variable
 *  Returns	:	FNRESLT
 *  Description	: 	
 *  Comments	:  	This function will give the time differece based on last hit time.
 ************************************************************************************************************/
int gettime(unsigned int *usec, unsigned char flag)
{
	static long long int start_time;
	struct timeval tv;

	if(flag	== RESET_TIME)
	{
		start_time	= 0;
	}

	gettimeofday(&tv,NULL);

	if(start_time == 0)
	{
		start_time = tv.tv_sec * 1000000 + tv.tv_usec;
	}
	if(flag == MICRO_SEC)
	{
		*usec	= ((tv.tv_sec * 1000000 + tv.tv_usec)-start_time);
	}else
	{
		*usec	= ((tv.tv_sec * 1000000 + tv.tv_usec)-start_time)/1000;
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_09
 *
 *  Name	:	
 *  Parameter1	:	cam_data *cam - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	
 ************************************************************************************************************/
int ctrl(device_data *dev)
{
	if(dev->flag.bit.comm_ctrl	== WRITE)
	{
		if(ioctl(dev->cam_fd, VIDIOC_S_CTRL, &dev->ctrl) < 0)
		{
			perror("CAPTURE: VIDIOC_S_CTRL");
			return FAILURE;

		}
	}else if(dev->flag.bit.comm_ctrl	== READ)
	{
		if(ioctl(dev->cam_fd, VIDIOC_G_CTRL, &dev->ctrl) < 0)
		{
			printf("CAPTURE: VIDIOC_G_CTRL");
			return FAILURE;

		}

	}else if(dev->flag.bit.comm_ctrl	== QUERY)
	{
		if(ioctl(dev->cam_fd, VIDIOC_QUERYCTRL, &dev->qctrl) < 0)
		{
			printf("CAPTURE: VIDIOC_QUERYCTRL");
		}
	}
	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_07
 *
 *  Name	:	convert_uyvy_rgb888
 *  Parameter1	:	cam_data *cam - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Function will convert YUYV to BMP888
 ************************************************************************************************************/
int convert_yuyv_rgb888(unsigned char *yuyv_buffer, unsigned int width, unsigned int height, unsigned char *rgb888)
{
	int i = 0, j = 0;
	unsigned char *rgb_buffer;
	int temp, yuvcount = 0, rgbcount = 0;
	float u_val, v_val, y1_val, y2_val;
	unsigned int time;

	rgb_buffer = rgb888;
	
	for (i=0; i < height; i++)
	{
		for (j = 0; j < width; j+=2)
		{
			y1_val = (float)yuyv_buffer[yuvcount++];				
			u_val = (float)yuyv_buffer[yuvcount++];
			y2_val = (float)yuyv_buffer[yuvcount++];
			v_val = (float)yuyv_buffer[yuvcount++];


			u_val = u_val - 128;
			v_val = v_val - 128;		

			temp = (int)(y1_val + (1.770 * u_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +0] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);

			temp = (int)(y1_val - (0.344 * u_val) - (0.714 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +1] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
				
			temp = (int)(y1_val + (1.403 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +2] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
	
			temp = (int)(y2_val + (1.770 * u_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +3] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
	
			temp = (int)(y2_val - (0.344 * u_val) - (0.714 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +4] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
				
			temp = (int)(y2_val + (1.403 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +5] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
		}
	}
	gettime(&time,NO_DATA);
	printf("Conversion completed form YUYV to RGB 888 :%u ms\n",time);

	return SUCCESS;
}



/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_07
 *
 *  Name	:	convert_uyvy_rgb888
 *  Parameter1	:	cam_data *cam - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Function will convert YUYV to BMP888
 ************************************************************************************************************/
int convert_uyvy_rgb888(unsigned char *yuyv_buffer, unsigned int width, unsigned int height, unsigned char *rgb888)
{
	int i = 0, j = 0;
	unsigned char *rgb_buffer;
	int temp, yuvcount = 0, rgbcount = 0;
	float u_val, v_val, y1_val, y2_val;
	unsigned int time;

	rgb_buffer = rgb888;

	for (i=0; i < height; i++)
	{
		for (j = 0; j < width; j+=2)
		{
			u_val = (float)yuyv_buffer[yuvcount++];				
			y1_val = (float)yuyv_buffer[yuvcount++];
			v_val = (float)yuyv_buffer[yuvcount++];
			y2_val = (float)yuyv_buffer[yuvcount++];
						
			u_val = u_val - 128;
			v_val = v_val - 128;		

			temp = (int)(y1_val + (1.770 * u_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +0] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);

			temp = (int)(y1_val - (0.344 * u_val) - (0.714 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +1] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
				
			temp = (int)(y1_val + (1.403 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +2] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
	
			temp = (int)(y2_val + (1.770 * u_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +3] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
	
			temp = (int)(y2_val - (0.344 * u_val) - (0.714 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +4] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
				
			temp = (int)(y2_val + (1.403 * v_val));
			rgb_buffer[(((height-1)-i) * width * 3) + j*3 +5] = (temp > 255) ? 255 : ((temp < 0) ? 0 :(unsigned char)temp);
		}
	}
	gettime(&time,NO_DATA);
	printf("Conversion completed form UYVY to RGB 888 :%u ms\n",time);

	return SUCCESS;
}


/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	:	FEATURE_TEST_08
 *
 *  Name	:	form_bmp_header_info
 *  Parameter1	:	cam_data *cam - Base pointer of camera structure
 *  Parameter2	:	
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  Description	: 	
 *  Comments	:  	Will form standard bmp888 header
 ************************************************************************************************************/
int form_bmp_header_info(device_data *dev)
{
	dev->bmp_header.type			= 0x4D42;
	dev->bmp_header.size			= sizeof(dev->bmp_header) + 		\
							dev->fmt.fmt.pix.width *	\
							dev->fmt.fmt.pix.height *3;
	dev->bmp_header.reserved1		= 0;
	dev->bmp_header.reserved2		= 0;
	dev->bmp_header.offbits			= sizeof(dev->bmp_header);
	dev->bmp_header.size_header		= 40;
	dev->bmp_header.width			= dev->fmt.fmt.pix.width;
	dev->bmp_header.height			= dev->fmt.fmt.pix.height;
	dev->bmp_header.planes			= 1;
	dev->bmp_header.bitcount		= 24;
	dev->bmp_header.compression		= 0;
	dev->bmp_header.size_image		= dev->fmt.fmt.pix.width *dev->fmt.fmt.pix.height *3;
	dev->bmp_header.x_permeter		= 0x0B13;
	dev->bmp_header.y_permeter		= 0x0B13;
	dev->bmp_header.clr_used		= 0;
	dev->bmp_header.clr_important		= 0;
	return SUCCESS;
}

