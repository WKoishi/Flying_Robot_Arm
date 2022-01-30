#include "lcd.h"
#include "spi.h"
#include "tim.h"
#include "stdarg.h"

//SPI显示屏接口
//LCD_RST
#define LCD_RST_SET     
#define LCD_RST_RESET  
//LCD_RS//dc  
#define LCD_RS_SET      HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port,LCD_WR_RS_Pin,GPIO_PIN_SET)//PC4 
#define LCD_RS_RESET    HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port,LCD_WR_RS_Pin,GPIO_PIN_RESET)
//LCD_CS  
#define LCD_CS_SET      HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET)
#define LCD_CS_RESET    HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)
//SPI Driver
#define SPI spi4
#define SPI_Drv (&hspi4)
#define delay_ms HAL_Delay
#define get_tick HAL_GetTick
//LCD_Brightness timer
#define LCD_Brightness_timer &htim1
#define LCD_Brightness_channel TIM_CHANNEL_2

static int32_t lcd_init(void);
static int32_t lcd_gettick(void);
static int32_t lcd_writereg(uint8_t reg,uint8_t* pdata,uint32_t length);
static int32_t lcd_readreg(uint8_t reg,uint8_t* pdata);
static int32_t lcd_senddata(uint8_t* pdata,uint32_t length);
static int32_t lcd_recvdata(uint8_t* pdata,uint32_t length);

ST7735_IO_t st7735_pIO = {
	lcd_init,
	NULL,
	NULL,
	lcd_writereg,
	lcd_readreg,
	lcd_senddata,
	lcd_recvdata,
	lcd_gettick
};

ST7735_Object_t st7735_pObj;
uint32_t st7735_id;

#define LCD_TEXT_LENGTH 50
uint8_t lcd_text_buf[LCD_TEXT_LENGTH];

void LCD_printf(struct StringHandler* handler, const char *format, ...)
{
    va_list args;
 
    va_start(args, format);
    vsnprintf((char *)lcd_text_buf, LCD_TEXT_LENGTH, (char *)format, args);
    va_end(args);
    LCD_ShowString(handler, lcd_text_buf);
}

void LCD_init(void)
{
    struct StringHandler init_message;

    #ifdef TFT96
    ST7735Ctx.Orientation = ST7735_ORIENTATION_LANDSCAPE_ROT180;
    ST7735Ctx.Panel = HannStar_Panel;
    ST7735Ctx.Type = ST7735_0_9_inch_screen;
    #elif TFT18
    ST7735Ctx.Orientation = ST7735_ORIENTATION_PORTRAIT;
    ST7735Ctx.Panel = BOE_Panel;
    ST7735Ctx.Type = ST7735_1_8a_inch_screen;
    #else
    error "Unknown Screen"

    #endif

    ST7735_RegisterBusIO(&st7735_pObj,&st7735_pIO);
    ST7735_LCD_Driver.Init(&st7735_pObj,ST7735_FORMAT_RBG565,&ST7735Ctx);
    ST7735_LCD_Driver.ReadID(&st7735_pObj,&st7735_id);

    LCD_SetBrightness(0);
    ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width, ST7735Ctx.Height, COLOR_BLACK);

    init_message.x0 = 4;
    init_message.y0 = ST7735Ctx.Height - 21;
    init_message.area_width = ST7735Ctx.Width;
    init_message.area_height = 12;
    init_message.color = COLOR_WHITE;
    init_message.bgcolor = COLOR_BLACK;
    init_message.font = &Font_7x10;
    LCD_printf(&init_message, "initialize...");
    ST7735_LCD_Driver.FillRect(&st7735_pObj, 4,  4, 16, 8, COLOR_RED);
    ST7735_LCD_Driver.FillRect(&st7735_pObj, 24, 4, 16, 8, COLOR_GREEN);
    ST7735_LCD_Driver.FillRect(&st7735_pObj, 44, 4, 16, 8, COLOR_BLUE);
	
//	#ifdef TFT96
//	extern unsigned char WeActStudiologo_160_80[];
//	ST7735_LCD_Driver.DrawBitmap(&st7735_pObj,0,0,WeActStudiologo_160_80);
//	#elif TFT18
//	extern unsigned char WeActStudiologo_128_160[];
//	ST7735_LCD_Driver.DrawBitmap(&st7735_pObj,0,0,WeActStudiologo_128_160);	
//	#endif
	
    uint32_t tick = get_tick();
    while (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) != GPIO_PIN_SET)
    {
        delay_ms(10);

        if (get_tick() - tick <= 1000)
            LCD_SetBrightness((get_tick() - tick) * 500 / 1000);
        else if (get_tick() - tick <= 3000)
        {
            init_message.x0 = ST7735Ctx.Width - 34;
            init_message.y0 = 1;
            init_message.font = &Font_11x18;
            init_message.color = COLOR_TIANYIBLUE;
            LCD_printf(&init_message, "%03d", 1 + (get_tick() - tick - 1000) / 20);
            ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, ST7735Ctx.Height - 4, 
                                        (get_tick() - tick - 1000) * ST7735Ctx.Width / 2000, 4, COLOR_TIANYIBLUE);
        }
        else if (get_tick() - tick > 3000)
            break;
	}
    while (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET)
    {
        delay_ms(10);
    }
    LCD_Light(0, 300);

    ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width,ST7735Ctx.Height, COLOR_BLACK);

//    LCD_printf(4, 4, ST7735Ctx.Width, 16, 16, "WeAct Studio");
//    LCD_printf(4, 22, ST7735Ctx.Width, 16, 16, "STM32H7xx 0x%X", HAL_GetDEVID());
//    LCD_printf(4, 40, ST7735Ctx.Width, 16, 16, "LCD ID:0x%X", st7735_id);

    LCD_Light(500, 200);
}

void LCD_SetBrightness(uint32_t Brightness)
{
    __HAL_TIM_SetCompare(LCD_Brightness_timer, LCD_Brightness_channel, Brightness);
}

uint32_t LCD_GetBrightness(void)
{
    return __HAL_TIM_GetCompare(LCD_Brightness_timer, LCD_Brightness_channel);
}


// 屏幕逐渐变亮或者变暗
// Brightness_Dis: 目标值
// time: 达到目标值的时间,单位: ms
void LCD_Light(uint32_t Brightness_Dis,uint32_t time)
{
    uint32_t Brightness_Now;
    uint32_t time_now;
    float temp1,temp2;
    float k,set;

    Brightness_Now = LCD_GetBrightness();
    time_now = 0;
    if(Brightness_Now == Brightness_Dis)
        return;

    if(time == time_now)
        return;

    temp1 = Brightness_Now;
    temp1 = temp1 - Brightness_Dis;
    temp2 = time_now;
    temp2 = temp2 - time;

    k = temp1 / temp2;

    uint32_t tick=get_tick();
    while(1)
    {
        delay_ms(1);

        time_now = get_tick()-tick;

        temp2 = time_now - 0;

        set = temp2*k + Brightness_Now;

        LCD_SetBrightness((uint32_t)set);

        if(time_now >= time) break;
    }
}


uint8_t char_write_buf[160];  //128*160
void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef* font, uint16_t color, uint16_t bgcolor) 
{
    uint16_t i, pixel, j;
    
    if(((x + font->width) > ST7735Ctx.Width) ||
            ((y + font->height) > ST7735Ctx.Height))
    {
        return;
    }

    for(i = 0; i < font->height; i++) 
    {
        if (ST7735_SetCursor(&st7735_pObj, x, y+i) == ST7735_OK)
        {
            pixel = font->data[(ch - 32) * font->height + i];
            for(j = 0; j < font->width; j++) 
            {
                if((pixel << j) & 0x8000)  {
                    char_write_buf[2*j] = (color>>8) & 0XFF;
                    char_write_buf[(2*j) + 1] = color & 0XFF;
                } else {
                    char_write_buf[2*j] = (bgcolor>>8) & 0XFF;
                    char_write_buf[(2*j) + 1] = bgcolor & 0XFF;
                }
            }
            st7735_send_data(&(st7735_pObj.Ctx), (uint8_t*)char_write_buf, 2U*font->width);
        }
    }
//    ST7735_FillRGBRect(&st7735_pObj, x, y, (uint8_t *)&char_write_buf, 
//                        font->width, font->height);
}

//显示字符串
void LCD_ShowString(struct StringHandler* handler, uint8_t *ptr)
{
    uint8_t Xpos = handler->x0;
    uint8_t Ypos = handler->y0;
    uint16_t width = handler->x0 + handler->area_width;
    uint16_t height = handler->y0 + handler->area_height;
    
    while((*ptr<='~')&&(*ptr>=' '))//判断是不是非法字符!
    {       
        if(Xpos + handler->font->width > width)
        {
            Xpos = handler->x0;
            Ypos += handler->font->height;
        }
        if (Ypos + handler->font->height > height)
            break;  //退出
        LCD_WriteChar(Xpos, Ypos, *ptr, handler->font, handler->color, handler->bgcolor);
        Xpos += handler->font->width;
        ptr++;
    }
    
    // 当字符显示区域发生变化时，清除原来的像素
    if (handler->last.y_end - Ypos > 0)  // 多行
    {
        ST7735_LCD_Driver.FillRect(&st7735_pObj, Xpos, Ypos, width - Xpos, 
                                        handler->font->height, COLOR_BLACK);
        ST7735_LCD_Driver.FillRect(&st7735_pObj, handler->x0, Ypos + handler->font->height, handler->area_width, 
                                        handler->last.y_end - Ypos, COLOR_BLACK);
    }
    else // 单行
    {
        if (handler->last.x_end - Xpos > 0)
        {
            ST7735_LCD_Driver.FillRect(&st7735_pObj, Xpos, Ypos, handler->last.x_end - Xpos, 
                                        handler->font->height, COLOR_BLACK);
        }
    }
    handler->last.x_end = Xpos;
    handler->last.y_end = Ypos;
}


static int32_t lcd_init(void)
{
    int32_t result = ST7735_OK;
    HAL_TIMEx_PWMN_Start(LCD_Brightness_timer,LCD_Brightness_channel);
    return result;
}

static int32_t lcd_gettick(void)
{
    return HAL_GetTick();
}

static int32_t lcd_writereg(uint8_t reg,uint8_t* pdata,uint32_t length)
{
    int32_t result;
    LCD_CS_RESET;
    LCD_RS_RESET;
    result = HAL_SPI_Transmit(SPI_Drv,&reg,1,100);
    LCD_RS_SET;
    if(length > 0)
        result += HAL_SPI_Transmit(SPI_Drv,pdata,length,500);
    LCD_CS_SET;
    if(result>0){
        result = -1;}
    else{
        result = 0;}
    return result;
}

static int32_t lcd_readreg(uint8_t reg,uint8_t* pdata)
{
    int32_t result;
    LCD_CS_RESET;
    LCD_RS_RESET;

    result = HAL_SPI_Transmit(SPI_Drv,&reg,1,100);
    LCD_RS_SET;
    result += HAL_SPI_Receive(SPI_Drv,pdata,1,500);
    LCD_CS_SET;
    if(result>0){
        result = -1;}
    else{
        result = 0;}
    return result;
}

static int32_t lcd_senddata(uint8_t* pdata,uint32_t length)
{
    int32_t result;
    LCD_CS_RESET;
    //LCD_RS_SET;
    result =HAL_SPI_Transmit(SPI_Drv,pdata,length,100);
    LCD_CS_SET;
    if(result>0){
        result = -1;}
    else{
        result = 0;}
    return result;
}

static int32_t lcd_recvdata(uint8_t* pdata,uint32_t length)
{
    int32_t result;
    LCD_CS_RESET;
    //LCD_RS_SET;
    result = HAL_SPI_Receive(SPI_Drv,pdata,length,500);
    LCD_CS_SET;
    if(result>0){
        result = -1;}
    else{
        result = 0;}
    return result;
}

