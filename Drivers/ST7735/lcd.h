#ifndef __LCD_H
#define __LCD_H		

#include "main.h"
#include "st7735.h"
#include "fonts.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COLOR_WHITE         0xFFFF
#define COLOR_BLACK         0x0000	  
#define COLOR_BLUE          0x001F  
#define COLOR_BRED          0XF81F
#define COLOR_GRED          0XFFE0
#define COLOR_GBLUE         0X07FF
#define COLOR_RED           0xF800
#define COLOR_MAGENTA       0xF81F
#define COLOR_GREEN         0x07E0
#define COLOR_CYAN          0x7FFF
#define COLOR_YELLOW        0xFFE0
#define COLOR_BROWN         0XBC40 //棕色
#define COLOR_BRRED         0XFC07 //棕红色
#define COLOR_GRAY          0X8430 //灰色
#define COLOR_DARKBLUE      0X01CF	//深蓝色
#define COLOR_LIGHTBLUE     0X7D7C	//浅蓝色  
#define COLOR_GRAYBLUE      0X5458 //灰蓝色
#define COLOR_TIANYIBLUE    0X667F
    
struct LastPrint {
    uint8_t x_end;
    uint8_t y_end;
};

struct StringHandler {
    uint8_t x0;
    uint8_t y0;
    uint8_t area_width;
    uint8_t area_height;
    uint16_t color;
    uint16_t bgcolor;
    FontDef *font;
    
    struct LastPrint last;
};

extern ST7735_Object_t st7735_pObj;
extern uint32_t st7735_id;

extern uint16_t POINT_COLOR;	//画笔颜色
extern uint16_t BACK_COLOR;  //背景色 

void LCD_init(void);
void LCD_SetBrightness(uint32_t Brightness);
uint32_t LCD_GetBrightness(void);
void LCD_Light(uint32_t Brightness_Dis,uint32_t time);
void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef* font, uint16_t color, uint16_t bgcolor);
void LCD_ShowString(struct StringHandler* handler, uint8_t *p);
void LCD_printf(struct StringHandler* handler, const char *format, ...);

extern ST7735_Ctx_t ST7735Ctx;

#ifdef __cplusplus
}
#endif

#endif  
