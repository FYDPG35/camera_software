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



device_data *dev;

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	: MAIN_01
 *
 *  Name	:	paint_main_menu
 *  Parameter1	:     	void
 *
 *  Returns	:	Function result depends on return value of child functions and 
 *  			condition available in the functions based on this return value will
 *
 *  			SUCCESS	- all the condition executed properly
 *  			FAIL	- Failed to perform specified operation
 *  			
 *  Description	: 	
 *  Comments	:  	
 ************************************************************************************************************/
int paint_main_menu()
{
	printf("\n");
	printf("+==================================================================================+\n");
	printf("|                     V4L2 command line application            			   |\n");
	printf("+==================================================================================+\n");

	return SUCCESS;
}

void get_memory_type()
{
	for(;;){
	printf("===========================================================\n");
	printf("		Choose Memory Type											\n");
	printf("===========================================================\n");
	printf("1.MMAP\n");
	printf("2.USER POINTER\n");
	printf("===========================================================\n");
	printf("Choose the option = ");
	scanf(" %d",&mem_type);
	if(mem_type != MMAP && mem_type != USERPTR)
		continue;
	else
		break;
	}
}




void signal_handler(int sig_no)
{
	if(stream_off(dev) < 0) {
		printf("\nFailed to turn off the streaming\n");
	}
	if(feature_test_api_exit(dev) < 0) {
		printf("Failed to safely exit from feature_test application \n");
	}
	exit(1);
}
	

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	: MAIN_02
 *
 *
 *  Name	:	main
 *  Parameter1	:	argc - input number of arguments from the command line
 *  Parameter2	:	argv[] - char* carry the string from the console arguments
 *
 *  Returns	:	return 0 - all the contion all executed correctly
 *  			return <FAIL ID NUMBER> if some of the case are failed in the execution.	
 *  Description	: 	
 *  Comments	:  	This application follows a menu type approch.
 *  			Termination of application can be done from main menu.
 ************************************************************************************************************/
// int main(int argc,char * argv[])
// {
// 	unsigned int size;
// 	int menu_option	= DISABLE;
// 	int binning = DISABLE;
// 	int ret_val = 0, i, j, width, height;
// 	char scan_string[32];
// 	int read_process_temp_gflag = 0;
// 	unsigned int time = 0;
// 	int framinval_index = 0;
// 	int cnt = 0;
// 	int resprintindex = 0;
// 	mem_type = MMAP;
// 	//FW_Version_Info* fwversion = malloc(sizeof(FW_Version_Info));
// 	char *read_buff = malloc(5);
// 	char *write_buff;
// 	FILE *wf;

// 	//Register a signal handler so that if terminated unexpectedly like ctrl+c, ctrl+x, handle those things
// 	// so that xserver can be restarted at the end of the application
// 	signal(SIGINT, signal_handler);
// 	signal(SIGTSTP, signal_handler);
// 	signal(SIGSEGV, signal_handler);

// 	if(feature_test_api_init(&dev,argc,argv) < 0) {
// 		printf("feature_test_api_init failed \n");
// 		return FAILURE;
// 	}
// 	dev->stream_all_res = 0;

// 	do{
// 		printf("Enter number of cameras is connected : ");
// 		scanf("%s",scan_string);
// 		dev->no_of_camera = atoi(scan_string);
// 	}while((dev->no_of_camera != SINGLE) && (dev->no_of_camera != DOUBLE));

// 	if(dev->no_of_camera == SINGLE){
// 		wf	= fopen("spi_write.txt","rb+");
// 		if(wf > 0){
// 			fseek(wf,0,SEEK_END);
// 			size=ftell(wf);
// 			rewind(wf);
// 			write_buff = malloc(size+2);
// 			memset(write_buff,0x00,sizeof(write_buff));
// 			fread(write_buff,size,1,wf);
// 			if(wf){
// 				fclose(wf);
// 				wf = (void *)-1;
// 			}
// 		}else{
// 			printf("spi_write.txt file not found\n");
// 			return FAILURE;
// 		}
// 	}



// 	//Try to stop the x11 server if already running
// 	//stop_xserver();

// 	if(dev->no_of_camera == SINGLE){
// 		if(initial_launch(dev)<0)
// 		{
// 			printf("Initial Launch failed \n");
// 			return FAILURE;
// 		}
// 	}


// 	for(;;)	{

// 		if(dev->no_of_camera == SINGLE){
// 			paint_main_menu();
// 			printf(" [%d] Still Capture Mode 	\n",MAIN_MENU_STILL);
// 			printf(" [%d] Streaming Mode  	   	\n",MAIN_MENU_STREAMING);
// 			printf(" [%d] Features    	        \n",MAIN_MENU_FEATURES);
// 			printf(" [%d] Exit from application     \n",EXIT_FRM_APPLICATION);
// 			printf(" Enter the option: ");
// 			scanf("%s",scan_string);
// 			//sscanf(scan_string,"%d",&menu_option);
// 			menu_option = atoi(scan_string);

// 			//scanf("%d",&menu_option);
// 			printf("Enterd string %s\n",scan_string);
// 			printf("Entered menu_option = %d \n",menu_option);

// 			if(menu_option == EXIT_FRM_APPLICATION) {
// 				free(write_buff);
// 				free(read_buff);
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			paint_main_menu();
// 			printf(" [1] Recording Mode 	\n");
// 			printf(" [2] Exit from application \n");
// 			scanf("%s",scan_string);
// 			menu_option = atoi(scan_string);
// 			if((menu_option != 1) && (menu_option != 2))
// 				continue;
// 			if(menu_option == 2) {
// 				break;
// 			}
// 		}	
// 		switch(menu_option) 
// 		{
// 			case MAIN_MENU_STILL: 
// 				{

// 					read_process_temp_gflag = dev->process.bit.stream_mode;	

// 					for(;;) {
// 						printf("\n=============================================\n");
// 						if(dev->no_of_camera == SINGLE)
// 						printf("Still Capture Menu              	     \n");
// 						else
// 						printf("Recording Menu              	     \n");
// 						printf("=============================================\n");
// 						printf(" [%d] Select Resolution format               \n",SUB_MENU_STILL_DIMENSION_CHANGE);
// 						if(dev->no_of_camera == SINGLE)
// 						printf(" [%d] Take Snap                              \n",SUB_MENU_STILL_TAKE_SNAP);
// 						else
// 						printf(" [%d] Record Video                              \n",SUB_MENU_STILL_TAKE_SNAP);
// 						printf(" [%d] Image Formats                          \n",SUB_MENU_STILL_IMG_FMT);
// 						printf(" [%d] Exit from snap menu                    \n",SUB_MENU_STILL_EXIT);

// 						printf("\nNote:\n");
// 						if(dev->no_of_camera == SINGLE){
// 							if(get_camera_format(dev) < 0)
// 							{
// 								printf("%s(): Unable to get the format of the capture device\n", __func__);
// 								goto exit;
// 							}
// 						}

// 						printf("Current Camera Settings \n");
// 						printf(" Width       : %d \n", dev->still_fmt.fmt.pix.width);	 
// 						printf(" Height      : %d \n", dev->still_fmt.fmt.pix.height);

// 						if(dev->no_of_camera == SINGLE)
// 							print_pixelformat("Pixel format : ",dev->fmt.fmt.pix.pixelformat); 
// 						printf("=============================================\n");
// 						printf("Enter the option: ");
// 						scanf("%s",scan_string);
// 						menu_option = atoi(scan_string);

// 						if(menu_option == SUB_MENU_STILL_EXIT) {
// 							dev->process.bit.snap_mode = DISABLE;
// 							dev->process.bit.stream_mode = read_process_temp_gflag;
// 							break;
// 						}
// 						switch(menu_option)
// 						{
// 							case SUB_MENU_STILL_DIMENSION_CHANGE: 							
// 								printf("Supported formats:\n");

// 								for(i=0;i<dev->total_frm_support; i++)
// 								{
// 									if(dev->fsize[i].discrete.width == 1152 || (dev->fsize[i].discrete.width == 2304 && dev->fsize[i].discrete.height == 1536))
// 										printf("%d: Width x Height = %d x %d\n",i, dev->fsize[i].discrete.width, dev->fsize[i].discrete.height);
// 								}


// 								printf("Enter the Width    : ");
// 								scanf("%s",scan_string);
// 								width = atoi(scan_string);


// 								printf("Enter the Height   : ");
// 								scanf("%s",scan_string);				
// 								height = atoi(scan_string);

// 								if(validate_dimension(dev, width, height) < 0) {
// 									printf("Warning! Invalid input: Try supported Values provided\n");
// 									continue;
// 								}

// 								dev->still_fmt.fmt.pix.width = width;
// 								dev->still_fmt.fmt.pix.height = height;
// 								dev->still_fmt.fmt.pix.pixelformat = dev->fmt.fmt.pix.pixelformat;
// 								break;
// 							case SUB_MENU_STILL_TAKE_SNAP: 
// 								{
// 									if(dev->no_of_camera != SINGLE){
// 										do{
// 											printf("Maximum number of frames can saved = 50\n");
// 											printf("Enter number frame want to save : ");
// 											scanf("%s",scan_string);				
// 											dev->frame_count = atoi(scan_string);
// 										}while(dev->frame_count > 50);
// 									}
// 									printf("Recording...\n");

// 									dev->process.bit.stream_mode = DISABLE;
// 									if(dev->no_of_camera == SINGLE){
// 										if(stream_off(dev) < 0) {
// 											printf(" Unable to stop streaming\n");
// 											break;
// 										}
// 									}
// 									dev->process.G_FLAG = DISABLE;

// 									gettime(&time,RESET_TIME);
// 									dev->process.bit.snap_mode = ENABLE;
// 									dev->cam_fps = 15;
// 									if(take_snap(dev) < 0) {
// 										dev->process.bit.snap_mode = DISABLE;
// 										printf(" Unable to capture the image\n");
// 										break;
// 									}
// 									gettime(&time,NO_DATA);
// 									if(dev->no_of_camera == SINGLE){
// 										printf("\nStill Capture completed time stamp.... %u ms\n",time);
// 									}
// 									else
// 									{
// 										printf("\nSave video completed time stamp.... %u ms\n",time);
// 									}
// 									dev->process.bit.snap_mode = DISABLE;

// 									if(dev->no_of_camera == SINGLE){
// 										dev->process.bit.stream_mode = ENABLE;
// 										dev->process.bit.dss_stream_on = DISABLE;
// 										if(stream_on(dev) < 0) {
// 											printf(" Unable to start streaming\n");
// 											break;
// 										}
// 									}

// 								}break;
// 							case SUB_MENU_STILL_IMG_FMT: 
// 								{
// 									unsigned int option;
// 									printf("=============================================\n");
// 									printf(" Image Pixel format 		             \n");
// 									printf("=============================================\n");
// 									printf(" [%d] YUYV mode(File save as .yuv) 	     \n", SUB_MENU_STILL_FMT_YUV);
// 									if(dev->no_of_camera == SINGLE)
// 										printf(" [%d] RGB888 mode(File save as .bmp) 	     \n", SUB_MENU_STILL_FMT_RGB);
// 									printf("=============================================\n");

// 									printf(" Enter the image format to save : \n");
// 									scanf("%s",scan_string);				
// 									option = atoi(scan_string);
// 									if(dev->no_of_camera != SINGLE)
// 										option = 1;

// 									switch(option) {
// 										case 2:	{
// 													dev->flag.bit.save_file_format = SAVE_BMP;
// 													dev->still_fmt.fmt.pix.pixelformat =	\
// 																							V4L2_PIX_FMT_UYVY;
// 												}break;
// 										case 1: {
// 													dev->flag.bit.save_file_format = SAVE_YUV;
// 													dev->still_fmt.fmt.pix.pixelformat =	\
// 																							V4L2_PIX_FMT_UYVY;
// 												}break;
// 										default:
// 												{
// 													printf(" Please enter the proper option \n");
// 												}continue;
// 									}
// 								}break;
// 							default: {
// 										 printf("\nPlease enter the proper option \n");
// 									 }continue;			
// 						}
// 					}
// 				}break;
// 			case MAIN_MENU_STREAMING: 
// 				{
// 					for(;;) {
// 						printf("\n=============================================\n");
// 						printf("Streaming Menu 		             	     \n");
// 						printf("=============================================\n");
// 						printf(" [%d] Select Resolution format       	     \n",SUB_MENU_STREAM_DIMENSION_CHANGE);
// 						printf(" [%d] Start Streaming                        \n",SUB_MENU_STREAM_START);
// 						printf(" [%d] Stop Streaming                         \n",SUB_MENU_STREAM_STOP);
// 						printf(" [%d] Exit from stream menu                  \n",SUB_MENU_STREAM_EXIT);

// 						printf("\nNote:\n");

// 						printf("Current Camera Stream Settings \n");
// 						printf(" Width       : %d \n", dev->stream_fmt.fmt.pix.width); 
// 						printf(" Height      : %d \n", dev->stream_fmt.fmt.pix.height); 
// 						print_pixelformat("Pixel format : ", dev->stream_fmt.fmt.pix.pixelformat); 
// 						printf("=============================================\n");
// 						printf("Enter the option: ");
// 						scanf("%s",scan_string);
// 						menu_option = atoi(scan_string);

// 						if(menu_option == SUB_MENU_STREAM_EXIT)
// 							break;

// 						switch(menu_option)
// 						{
// 							case SUB_MENU_STREAM_DIMENSION_CHANGE: 
// 								{
// 									printf("Supported formats:\n");
// 									resprintindex = 1;
// 									if(dev->stream_all_res == 1)
// 									{
// 										for(i=0;i<dev->total_frm_support; i++)
// 										{
// 											printf("%d: Width x Height = %d x %d\n",resprintindex++, dev->fsize[i].discrete.width, dev->fsize[i].discrete.height);
// 										}

// 									}
// 									else
// 									{
// 										for(i=0;i<dev->total_frm_support; i++)
// 										{
// 											if((dev->fsize[i].discrete.width <= 1920) && (dev->fsize[i].discrete.height <= 1080))
// 												printf("%d: Width x Height = %d x %d\n",resprintindex++, dev->fsize[i].discrete.width, dev->fsize[i].discrete.height);
// 										}
// 									}
// 									printf("Enter the Width    : ");
// 									scanf("%s",scan_string);
// 									width = atoi(scan_string);

// 									printf("Enter the Height   : ");
// 									scanf("%s",scan_string);				
// 									height = atoi(scan_string);

// 									if(validate_dimension(dev, width, height) < 0) {
// 										printf("Warning! Invalid input: Try supported Values provided\n");
// 										continue;
// 									}
// 									dev->stream_fmt.fmt.pix.width = width;
// 									dev->stream_fmt.fmt.pix.height = height;
// 									dev->stream_fmt.fmt.pix.pixelformat = dev->fmt.fmt.pix.pixelformat;

// 									dev->stream_fmt.fmt.pix.pixelformat = dev->ffmt[0].pixelformat;

// 									framinval_index = get_frame_intervals(dev);
// 									if(framinval_index == 0)
// 									{
// 										printf("Frame rate will set to default\n");
// 										break;
// 									}
// 									printf("=================================================\n");
// 									printf("Frame rate supported:\n");
// 									for(cnt = 0; cnt < framinval_index; cnt ++){
// 										if(dev->desc_frm_inval[cnt].denominator%dev->desc_frm_inval[cnt].numerator == 0)
// 											printf("[%d]: %d FPS\n",cnt+1,dev->desc_frm_inval[cnt].denominator/dev->desc_frm_inval[cnt].numerator);
// 										else
// 											printf("[%d]: %d.5 FPS\n",cnt+1,dev->desc_frm_inval[cnt].denominator/dev->desc_frm_inval[cnt].numerator);
// 									}
// 									if(framinval_index == 1)
// 									{
// 										printf("%d FPS is selected by default\n", dev->desc_frm_inval[0].denominator);
// 										dev->stream_frm_inval.numerator = 	dev->desc_frm_inval[0].numerator;
// 										dev->stream_frm_inval.denominator = 	dev->desc_frm_inval[0].denominator;
// 									}
// 									else
// 									{	
// 										printf("Select the frame rate:");
// 										scanf(" %s",scan_string);
// 										framinval_index = atoi(scan_string);

// 										framinval_index--;
// 										dev->stream_frm_inval.numerator = 	dev->desc_frm_inval[framinval_index].numerator;	
// 										dev->stream_frm_inval.denominator = 	dev->desc_frm_inval[framinval_index].denominator;			
// 									}
// 								}break;
// 							case SUB_MENU_STREAM_START: 
// 								{
// 									dev->process.bit.stream_mode = ENABLE;
// 									if(stream_on(dev) < 0) {
// 										dev->process.bit.stream_mode = DISABLE;
// 										printf(" Unable to start streaming\n");
// 										break;
// 									}
// 								}break;
// 							case SUB_MENU_STREAM_STOP: 
// 								{
// 									dev->process.bit.stream_mode = DISABLE;
// 									if(stream_off(dev) < 0) {
// 										printf(" Unable to stop streaming\n");
// 										break;
// 									}
// 								}break;
// 							default:
// 								{
// 									printf(" Please enter the proper option \n");
// 								}continue;
// 						}
// 					}
// 				}break;
// 			case MAIN_MENU_FEATURES:
// 				{
// 					if( ctrl_v4l2_parms(dev) < 0) {
// 						printf("\n Unable to process the control. Please try again \n");
// 						break;					
// 					}
// 				}break;
// 			default:
// 				{
// 					printf("\n Error in input!, Please choose correct option ....\n");
// 				}continue;
// 		}
// 	}
// exit:

// 	if(stream_off(dev) < 0) {
// 		printf("\nFailed to turn off the streaming\n");
// 	}
// 	if(feature_test_api_exit(dev) < 0) {
// 		printf("Failed to safely exit from feature_test application \n");
// 	}


// 	return 0;
// }

/************************************************************************************************************
 *  
 *  MODULE TYPE	:	FUNCTION				MODULE ID	: 
 *
 *
 *  Name	:	ctrl_v4l2_parms
 *  Parameter1	:	device_data *dev
 *
 *  Returns	:	return 0 - all the contion all executed correctly
 *  			return <FAIL ID NUMBER> if some of the case are failed in the execution.	
 *  Description	: 	
 *  Comments	:  	
 *************************************************************************************************************/

int ctrl_v4l2_parms(device_data *dev)
{
	int menu_option	= DISABLE;
	int ret_val;
	char scan_string[16];
	int control_index = 0;
	int exit_option = total_ctrl_supported+1;
	int show_fps_option = 0;
	int sub_menu_option = 0;
	for(;;)
	{
		
		for(control_index = 0; control_index < total_ctrl_supported;control_index++)
		{
			printf("[%d] -- %s\n",control_index+1,supported_ctrl[control_index].name);
		}
		show_fps_option = total_ctrl_supported+1;
		printf("[%d] -- Frame rate debug message        \n",show_fps_option);
		exit_option = show_fps_option+1;
		printf("[%d] -- Exit from Features Menu \n",exit_option);

		printf("Enter the option: ");
		scanf("%s",scan_string);
		//sscanf(scan_string,"%d",&menu_option);
		menu_option = atoi(scan_string);
		control_index = menu_option-1;
	
		if((control_index<0)||(control_index>exit_option))
		{
			printf("Wrong option: Enter correct option \n");
				
		}		
		else if(menu_option == exit_option) {
			break;
		}
		else if (menu_option == show_fps_option)
		{
			for(;;)
			{
				printf("\n========================================================\n");
				printf(" Show Frame rate debug message                          \n");
				printf("========================================================\n");
				printf(" [0] Disable                                            \n");
				printf(" [1] Enable                                             \n");
				printf(" [2] Exit from Show Frame rate menu                     \n");
				printf(" Note:\n");
				printf(" Current Value is %d \n", dev->show_frame_rate);
				printf("========================================================\n");
				printf(" Enter the Value : ");
				scanf("%s",scan_string);	
				//sscanf(scan_string,"%d",&menu_option);
				menu_option = atoi(scan_string);

				if(menu_option == 2) {
					break;
				} else {
					switch(menu_option) {
						case 0:
						case 1:
							dev->show_frame_rate	= menu_option;
						break;
						default: {
							printf("\nError in input!, Please choose correct option \n");
						}continue;
					}
				}
			}	
		}
		else if(menu_option <= total_ctrl_supported)
		{
			switch(supported_ctrl[control_index].id)
			{
			case V4L2_CID_WHITE_BALANCE_TEMPERATURE:
				for(;;)
				{
					printf("\n===============================================================\n");
					printf(" WhiteBalance Menu ");
					printf("\n===============================================================\n");
					printf(" [1] %s \n",white_balamce_ctrl[0].name);
					printf(" [2] %s \n",white_balamce_ctrl[1].name);
					printf(" [3] Exit from whitebalance  menu      \n");
 					printf(" Note:\n");
					printf("================================================================\n");
					printf("\n  Please choose the option :");
					scanf("%s",scan_string);
					//sscanf(scan_string,"%d",&menu_option);
					menu_option = atoi(scan_string);

					control_index = menu_option-1;
					if((menu_option < 1) || (menu_option>3))
						printf("Wrong option: Enter proper option \n");
					else if(menu_option == 3)
						break;
					else
					{
						printf("\n===============================================================\n");
						printf("%s Menu -- Range %d to %d, Step Size = %d, Default Value = %d ",white_balamce_ctrl[control_index].name,white_balamce_ctrl[control_index].minimum,white_balamce_ctrl[control_index].maximum,white_balamce_ctrl[control_index].step,white_balamce_ctrl[control_index].default_value);
						printf("\n===============================================================\n");
									
						dev->flag.bit.comm_ctrl	= READ;
						dev->ctrl.id = white_balamce_ctrl[control_index].id;
						if(ctrl(dev) < 0) {
							printf("\n Unable to READ the control. Please try again \n");
							continue;
						}
						printf("Current %s Value is %d\n",white_balamce_ctrl[control_index].name,dev->ctrl.value);
						printf("========================================================\n");
						printf("Please enter the %s Value : ",white_balamce_ctrl[control_index].name);
						scanf("%s",scan_string);	
						//sscanf(scan_string,"%d",&dev->ctrl.value);
						dev->ctrl.value = atoi(scan_string);

						if((dev->ctrl.value < white_balamce_ctrl[control_index].minimum) || (dev->ctrl.value > white_balamce_ctrl[control_index].maximum)) 
						{
							printf(" Enter input within the range (%d to %d)\n", white_balamce_ctrl[control_index].minimum,white_balamce_ctrl[control_index].maximum);
							continue;
						}
						dev->flag.bit.comm_ctrl	= WRITE;
						dev->ctrl.id =white_balamce_ctrl[control_index].id;
						if(ctrl(dev) < 0) {
							printf("\n Unable to WRITE the control. Please try again \n");
							continue;
						}
					}			
				}
			
			break;
			
			case V4L2_CID_FOCUS_ABSOLUTE:
				printf("total_focus_supported = %d\n",total_focus_supported);
				for(;;)
				{
					printf("\n===============================================================\n");
					printf(" Focus Menu ");
					printf("\n===============================================================\n");
					sub_menu_option= 1;
					printf(" [%d] Manual Focus \n",sub_menu_option++);
					if(total_focus_supported > 1)
					{
						printf(" [%d] Auto Focus \n",sub_menu_option++);						
					}
					if((focus_ctrl[1].id == V4L2_CID_AUTO_FOCUS_START) && total_focus_supported == 4)
					{	
						printf(" [%d] Trigger Focus \n",sub_menu_option++);								
					}
					printf(" [%d] Exit from Focus Menu \n",sub_menu_option);
					exit_option = sub_menu_option;
					printf("\n  Please choose the option :");
					scanf("%s",scan_string);
					//sscanf(scan_string,"%d",&sub_menu_option);
					sub_menu_option = atoi(scan_string);
					if((sub_menu_option < 1) || (sub_menu_option>4))
						printf("Wrong option: Enter proper option \n");

				 	else if(sub_menu_option == exit_option)
						break;
					control_index = sub_menu_option-1;
					switch(sub_menu_option)
					{
						case 1:
						{
							printf("\n===============================================================\n");
							printf("%s Menu  ----- Range %d to %d, Step Size = %d Default Value = %d ",focus_ctrl[control_index].name,focus_ctrl[control_index].minimum,focus_ctrl[control_index].maximum,focus_ctrl[control_index].step,focus_ctrl[control_index].default_value);
							printf("\n===============================================================\n");
									
							dev->flag.bit.comm_ctrl	= READ;
							dev->ctrl.id = focus_ctrl[control_index].id;
							if(ctrl(dev) < 0) {
								printf("\n Unable to READ the control. Please try again \n");
								continue;
							}
							printf("Current %s Value is %d\n",focus_ctrl[control_index].name,dev->ctrl.value);
							printf("========================================================\n");
							printf("Please enter the %s Value : ",focus_ctrl[control_index].name);
							scanf("%s",scan_string);	
							dev->ctrl.value = atoi(scan_string);

							if((dev->ctrl.value < focus_ctrl[control_index].minimum) || (dev->ctrl.value > focus_ctrl[control_index].maximum)) 
							{
								printf(" Enter input within the range (%d to %d)\n", focus_ctrl[control_index].minimum,focus_ctrl[control_index].maximum);
								continue;
							}
							dev->flag.bit.comm_ctrl	= WRITE;
							dev->ctrl.id =focus_ctrl[control_index].id;
						}
						break;
						case 2:
						{
							if(focus_ctrl[1].id ==V4L2_CID_FOCUS_AUTO)
							{
								printf("\n===============================================================\n");
								printf("%s Menu  ----- Range %d to %d, Step Size = %d ",focus_ctrl[control_index].name,focus_ctrl[control_index].minimum,focus_ctrl[control_index].maximum,focus_ctrl[control_index].step);
								printf("\n===============================================================\n");
									
								dev->flag.bit.comm_ctrl	= READ;
								dev->ctrl.id = focus_ctrl[control_index].id;
								if(ctrl(dev) < 0) {
									printf("\n Unable to READ the control. Please try again \n");
									continue;
								}
								printf("Current %s Value is %d\n",focus_ctrl[control_index].name,dev->ctrl.value);
								printf("========================================================\n");
								printf("Please enter the %s Value : ",focus_ctrl[control_index].name);
								scanf("%s",scan_string);	
								dev->ctrl.value = atoi(scan_string);

								if((dev->ctrl.value < focus_ctrl[control_index].minimum) || (dev->ctrl.value > focus_ctrl[control_index].maximum)) 
								{
									printf(" Enter input within the range (%d to %d)\n", focus_ctrl[control_index].minimum,focus_ctrl[control_index].maximum);
									continue;
								}
								dev->flag.bit.comm_ctrl	= WRITE;
								dev->ctrl.id =focus_ctrl[control_index].id;

							}
							else 
							{
								printf("\n===============================================================\n");
								printf("Auto Focus Menu 1--Enable 0--Disable");
								printf("\n===============================================================\n");
								printf("Please enter the Value");
								scanf("%s",scan_string);
								sub_menu_option = atoi(scan_string);

								switch(sub_menu_option) {
									case 0:
										dev->flag.bit.comm_ctrl = WRITE;
										dev->ctrl.id = V4L2_CID_AUTO_FOCUS_STOP;
										break;
									case 1:
										dev->flag.bit.comm_ctrl = WRITE;
										dev->ctrl.id = V4L2_CID_AUTO_FOCUS_START;
										break;
									default: 
										printf("\nError in input!, Please choose correct option \n");
										continue;
								}
							}
						}
						break;
						case 3:
						{
							dev->flag.bit.comm_ctrl = WRITE;
							dev->ctrl.id = focus_ctrl[3].id;
						}
						break;										
					}
					printf("\n Writing focus control dev->ctrl.id = 0x%x, dev->ctrl.value = %d\n",dev->ctrl.id, dev->ctrl.value);

					if(ctrl(dev) < 0) {
						printf("\n Unable to WRITE the control. Please try again \n");
						continue;
					}
				
				}
			break;
			case V4L2_CID_EXPOSURE_ABSOLUTE:
			{
				for(;;)
				{
					printf("\n===============================================================\n");
					printf(" Exposure Menu ");
					printf("\n===============================================================\n");
					printf(" [1] %s \n",exposure_ctrl[0].name);
					printf(" [2] %s \n",exposure_ctrl[1].name);
					printf(" [3] Exit from Exposure menu      \n");
 					printf(" Note:\n");
					printf("================================================================\n");
					printf("\n  Please choose the option :");
					scanf("%s",scan_string);
					menu_option = atoi(scan_string);
					control_index = menu_option-1;
					if((menu_option < 1) || (menu_option>3))
						printf("Wrong option: Enter proper option \n");
					else if(menu_option == 3)
						break;
					else
					{
						printf("\n===============================================================\n");
						printf("%s Menu -- Range %d to %d, Step Size = %d Default Value = %d",exposure_ctrl[control_index].name,exposure_ctrl[control_index].minimum,exposure_ctrl[control_index].maximum,exposure_ctrl[control_index].step,exposure_ctrl[control_index].default_value);
						printf("\n===============================================================\n");
									
						dev->flag.bit.comm_ctrl	= READ;
						dev->ctrl.id = exposure_ctrl[control_index].id;
						if(ctrl(dev) < 0) {
							printf("\n Unable to READ the control. Please try again \n");
							continue;
						}
						printf("Current %s Value is %d\n",exposure_ctrl[control_index].name,dev->ctrl.value);
						printf("========================================================\n");
						printf("Please enter the %s Value : ",exposure_ctrl[control_index].name);
						scanf("%s",scan_string);	
						dev->ctrl.value = atoi(scan_string);

						if((dev->ctrl.value < exposure_ctrl[control_index].minimum) || (dev->ctrl.value > exposure_ctrl[control_index].maximum)) 
						{
							printf(" Enter input within the range (%d to %d)\n", exposure_ctrl[control_index].minimum,exposure_ctrl[control_index].maximum);
							continue;
						}
						dev->flag.bit.comm_ctrl	= WRITE;
						dev->ctrl.id =exposure_ctrl[control_index].id;
						if(ctrl(dev) < 0) {
							printf("\n Unable to WRITE the control. Please try again \n");
							continue;
						}
					}			
				}				
			}
			break;
		case V4L2_CID_SENSOR_TEMP:
		for(;;)
			{

				printf("\n========================================================\n");
				printf(" Read Temperaure Sensor data                            \n");
				printf("========================================================\n");
				printf(" [1] Read temperature in celsius                        \n");
				printf(" [2] Exit from Read Temperature menu                    \n");
				printf("========================================================\n");
				printf(" Enter the Value : ");
				scanf("%s",scan_string);
				menu_option = atoi(scan_string);

				if(menu_option == 2) {
					break;
				} else {
					switch(menu_option) {
						case 1:
							dev->flag.bit.comm_ctrl	= READ;
							dev->ctrl.id = V4L2_CID_SENSOR_TEMP;
							if(ctrl(dev) < 0) {
								printf("\n Temperature sensor is turned off. Please try again after turning it on\n");
								continue;
							}
							else
							{
								dev->celsius=(struct temperature*)&(dev->ctrl.value);
								printf("\n Current temperature value in celsius: %d.%02d \n",dev->celsius->dec,dev->celsius->frac);
							}
							break;
						default: {
							printf("\nError in input!, Please choose correct option \n");
						}continue;
					}
				}
			}
break;
 		case V4L2_CID_DRIVER_BASE+11:
	for(;;)
			{
				printf("\n========================================================\n");
				printf(" Show Focus status                          \n");
				printf("========================================================\n");
				printf(" [0] Disable                                            \n");
				printf(" [1] Enable                                             \n");
				printf(" [2] Exit from Show focus status menu                     \n");
				printf(" Note:\n");
				printf(" Current Value is %d \n", dev->show_focus);
				printf("========================================================\n");
				printf(" Enter the Value : ");
				scanf("%s",scan_string);	
				menu_option = atoi(scan_string);

				if(menu_option == 2) {
					break;
				} else {
					switch(menu_option) {
						case 0:
						case 1:
							dev->show_focus	= menu_option;
						break;
						default: {
							printf("\nError in input!, Please choose correct option \n");
						}continue;
					}
				}
			}	
break;

			default:			
				for(;;)
				{
					sub_menu_option= 1;
					printf("\n===============================================================\n");
					printf("%s Menu -- Range %d to %d, Step Size = %d, Default Value = %d ",supported_ctrl[control_index].name,supported_ctrl[control_index].minimum,supported_ctrl[control_index].maximum,supported_ctrl[control_index].step,supported_ctrl[control_index].default_value);
					printf("\n===============================================================\n");
					printf(" [%d] Choose %s level \n",sub_menu_option++,supported_ctrl[control_index].name);
					printf(" [%d] Exit from  %s menu \n",sub_menu_option++,supported_ctrl[control_index].name);
 					printf(" Note:\n");
			
					dev->flag.bit.comm_ctrl	= READ;
					dev->ctrl.id = supported_ctrl[control_index].id;
					if(ctrl(dev) < 0) {
						printf("\n Unable to READ the control. Please try again \n");
						continue;
					}
					printf("Current %s Value is %d\n",supported_ctrl[control_index].name,dev->ctrl.value);
					printf("========================================================\n");
					printf("\n  Please choose the option :");
					scanf("%s",scan_string);
					sub_menu_option = atoi(scan_string);

					if(sub_menu_option == 2)
						break;
					switch(sub_menu_option)
					{
						case 1:	printf("Please enter the %s Value : ",supported_ctrl[control_index].name);
							scanf("%s",scan_string);	
							dev->ctrl.value = atoi(scan_string);

							if((dev->ctrl.value < supported_ctrl[control_index].minimum) || (dev->ctrl.value > supported_ctrl[control_index].maximum)) {
								printf(" Enter input within the range (%d to %d)\n", supported_ctrl[control_index].minimum,supported_ctrl[control_index].maximum);
								continue;
							}
							dev->flag.bit.comm_ctrl	= WRITE;
							dev->ctrl.id =supported_ctrl[control_index].id;
							if(ctrl(dev) < 0) {
								printf("\n Unable to WRITE the control. Please try again \n");
								continue;
							} 	
							break;
						case 2:	
							break;
						default:printf("\nError in input!, Please choose correct option \n");
					}
				}
				break;
			}			
		}
	}

	return SUCCESS;

exit: 
	return FAILURE;
}

