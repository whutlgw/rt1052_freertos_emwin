/**
  ******************************************************************
  * @file    lcd_test.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   lcd����Ӧ�ýӿ�
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include <stdio.h>
 
#include "fsl_elcdif.h"

#include "./lcd/bsp_lcd.h" 


/* ֡�ʣ�ֱ��ʹ����һ�����֡��
*  ʹ��ʱ��Ҫ����eLCDIF��֡�жϣ�
*  ����systick�н��и���
*/
extern __IO uint32_t s_frame_rate;

/**
 * @note �������ڲ�ͬ���Ż�ģʽ����ʱʱ�䲻ͬ��
 *       ��flexspi_nor_debug��flexspi_nor_release�汾�ĳ����У�
 *       flexspi_nor_release�汾����ʱҪ�̵ö�  
 */ 
static void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* ����nop��ָ�� */
    }
}

/**
* @brief  ��Һ��������������ͼ��
* @param  ��
* @retval ��
*/
void LCD_FillFrameBuffer(pixel_t frameBuffer[LCD_MAX_PIXEL_HEIGHT][LCD_MAX_PIXEL_WIDTH])
{
    /* �����ƶ��ٶ� */
    #define   SPEED     10
  
    /* Background color. */
    static const pixel_t bgColor = 0U;
    /* Foreground color. */
    static uint8_t fgColorIndex = 0U;
    static const pixel_t fgColorTable[] = {CL_BLUE, CL_GREEN, CL_CYAN, CL_RED,
                                            CL_MAGENTA, CL_YELLOW, CL_WHITE};
    pixel_t fgColor = fgColorTable[fgColorIndex];

    /* Position of the foreground rectangle. */
    static uint16_t upperLeftX = 0U;
    static uint16_t upperLeftY = 0U;
    static uint16_t lowerRightX = (LCD_MIN_PIXEL_WIDTH - 1U) / 2U;
    static uint16_t lowerRightY = (LCD_MIN_PIXEL_HEIGHT - 1U) / 2U;

    static int8_t incX = SPEED;
    static int8_t incY = SPEED;

    /* Change color in next forame or not. */
    static bool changeColor = false;

    uint32_t i, j;

    /* Background color. */
    for (i = 0; i < LCD_PIXEL_HEIGHT; i++)
    {
        for (j = 0; j < LCD_PIXEL_WIDTH; j++)
        {
            frameBuffer[i][j] = bgColor;
        }
    }

    /* Foreground color. */
    for (i = upperLeftY; i < lowerRightY; i++)
    {
        for (j = upperLeftX; j < lowerRightX; j++)
        {
            frameBuffer[i][j] = fgColor;
        }
    }

    /* Update the format: color and rectangle position. */
    upperLeftX += incX;
    upperLeftY += incY;
    lowerRightX += incX;
    lowerRightY += incY;

    changeColor = false;

    if (0U == upperLeftX)
    {
        incX = SPEED;
        changeColor = true;
    }
    else if (LCD_PIXEL_WIDTH - 1 == lowerRightX)
    {
        incX = -SPEED;
        changeColor = true;
    }

    if (0U == upperLeftY)
    {
        incY = SPEED;
        changeColor = true;
    }
    else if (LCD_PIXEL_HEIGHT - 1 == lowerRightY)
    {
        incY = -SPEED;
        changeColor = true;
    }

    if (changeColor)
    {
        fgColorIndex++;

        if (ARRAY_SIZE(fgColorTable) == fgColorIndex)
        {
            fgColorIndex = 0U;
        }
    }
}

/**
* @brief  ��Һ��������������ͼ��
* @param  ��
* @retval ��
*/
void LCD_ChangeAndSetNextBuffer(void)
{
  static uint8_t frameBufferIndex = 0;

  /* �л��������� */
  frameBufferIndex ^= 1U;

  /* �����һ�������� */
  LCD_FillFrameBuffer(s_psBufferLcd[frameBufferIndex]);

  /* ����ELCDIF����һ����������ַ */
  ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)s_psBufferLcd[frameBufferIndex]);

  /* �����жϱ�־ */
  s_frameDone = false;
  /* �ȴ�ֱ���ж���� */
  while (!s_frameDone)
  {
  }
}




/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
	/*��ʾ��ʾ����*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;	
	
  LCD_Clear(CL_BLACK);	/* ��������ʾȫ�� */

	/*����������ɫ������ı�����ɫ*/
  LCD_SetColors(CL_WHITE,CL_BLACK);
	
	/*ѡ������*/
  LCD_SetFont(&Font24x48);

  LCD_DisplayStringLine(LINE(0),(uint8_t* )"YH 5.0/7.0 inch LCD demo");
  LCD_DisplayStringLine(LINE(1),(uint8_t* )"Image resolution:800x480 px");
  
	if(s_frame_rate)
	{
		/*ʹ��c��׼��ѱ���ת�����ַ���*/
		sprintf(dispBuff,"Frame Rate: %d Hz",s_frame_rate);
		LCD_DisplayStringLine(LINE(2),(uint8_t* )dispBuff);
	}
	else
	{
	  LCD_DisplayStringLine(LINE(2),(uint8_t* )"Use RT1052-ELCDIF driver");
	}
    
  /*ѡ������*/
  LCD_SetFont(&Font16x32);
  LCD_SetTextColor(CL_RED);


	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(LINE(5));
	
	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	LCD_DisplayStringLine(LINE(5),(uint8_t* )dispBuff);


  /* ��ֱ�� */
  LCD_SetTextColor(CL_BLUE);

	LCD_ClearLine(LINE(6));
  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw line:");
  
	LCD_SetColors(CL_RED,CL_BLACK);
  LCD_DrawUniLine(50,250,750,250);  
  LCD_DrawUniLine(50,300,750,300);
  
	LCD_SetColors(CL_GREEN,CL_BLACK);
  LCD_DrawUniLine(300,250,400,400);  
  LCD_DrawUniLine(600,250,600,400);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(CL_BLACK,CL_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  
  
  /*������*/
  LCD_SetTextColor(CL_BLUE);
	LCD_ClearLine(LINE(6));
  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw Rect:");
	
	LCD_SetColors(CL_RED,CL_BLACK);
  LCD_DrawRect(200,250,200,100);
	
	LCD_SetColors(CL_GREEN,CL_BLACK);
  LCD_DrawRect(350,250,200,50);
	
	LCD_SetColors(CL_BLUE,CL_BLACK);
  LCD_DrawRect(200,350,50,100);
  
  delay(0xFFFFFFF);
  
  
	LCD_SetColors(CL_BLACK,CL_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  

  /*������*/
  LCD_SetTextColor(CL_BLUE);
	LCD_ClearLine(LINE(6));
	LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw Full Rect:");

 	LCD_SetColors(CL_RED,CL_BLACK);
  LCD_DrawFullRect(200,250,200,100);
	
	LCD_SetColors(CL_GREEN,CL_BLACK);
  LCD_DrawFullRect(350,250,200,50);
	
	LCD_SetColors(CL_BLUE,CL_BLACK);
  LCD_DrawFullRect(200,350,50,100);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(CL_BLACK,CL_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
  /* ��Բ */
	LCD_SetTextColor(CL_BLUE);
	LCD_ClearLine(LINE(6));
	LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw circle:");
  
	LCD_SetColors(CL_RED,CL_RED);
  LCD_DrawCircle(200,350,50);
	
	LCD_SetColors(CL_GREEN,CL_GREEN);
  LCD_DrawCircle(350,350,75);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(CL_BLACK,CL_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


  /*���Բ*/
	LCD_SetTextColor(CL_BLUE);
	LCD_ClearLine(LINE(6));
	LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw full circle:");
  
	LCD_SetColors(CL_RED,CL_BLACK);
  LCD_DrawFullCircle(300,350,50);
	
	LCD_SetColors(CL_GREEN,CL_BLACK);
  LCD_DrawFullCircle(450,350,75);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(CL_BLACK,CL_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
	LCD_ClearLine(LINE(6));	
 
  delay(0xFFFFFF);
	
	LCD_SetColors(CL_BLACK,CL_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


}
/*********************************************END OF FILE**********************/