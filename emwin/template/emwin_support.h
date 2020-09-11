/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */




#ifndef _EMWIN_SUPPORT_H_
#define _EMWIN_SUPPORT_H_

//
// Color conversion TODO - choose appropriate color conversion here - see emWin documentation
//
#define COLOR_CONVERSION GUICC_M565  //GUICC_888 K

//
// Display driver TODO - choose appropriate LCD driver here - see emWin documentation
//
#define DISPLAY_DRIVER  GUIDRV_FLEXCOLOR
// TODO - specify the LCD dimensions
#define LCD_WIDTH  800	
#define LCD_HEIGHT 480

// TODO - specify the memory assigned to emWin
#define GUI_NUMBYTES (1024*1024*8) //0x8000 /*! Amount of memory assigned to the emWin library */
// TODO - specify the memory location for the frame buffers


#endif
