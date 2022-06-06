/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
 * Copyright (c) 2018 by Fabrice Weinberg
 * Copyright (c) 2019 by Helmut Tschemernjak - www.radioshuttle.de
 * Copyright (c) 2020 by Brad Goodman - www.bradgoodman.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ThingPulse invests considerable time and money to develop these open source libraries.
 * Please support us by buying our products (and not the clones) from
 * https://thingpulse.com
 *
 */

#ifndef OLEDDISPLAY_h
#define OLEDDISPLAY_h

#include <sys/types.h>
#include "driver/i2c.h"
#include "OLEDDisplayFonts.h"

//#define DEBUG_OLEDDISPLAY(...) Serial.printf( __VA_ARGS__ )
//#define DEBUG_OLEDDISPLAY(...) dprintf("%s",  __VA_ARGS__ )

#ifndef DEBUG_OLEDDISPLAY
#define DEBUG_OLEDDISPLAY(...)
#endif

// Use DOUBLE BUFFERING by default
#ifndef OLEDDISPLAY_REDUCE_MEMORY
#define OLEDDISPLAY_DOUBLE_BUFFER
#endif

// Header Values
#define JUMPTABLE_BYTES 4

#define JUMPTABLE_LSB 1
#define JUMPTABLE_SIZE 2
#define JUMPTABLE_WIDTH 3
#define JUMPTABLE_START 4

#define WIDTH_POS 0
#define HEIGHT_POS 1
#define FIRST_CHAR_POS 2
#define CHAR_NUM_POS 3

// Display commands
#define CHARGEPUMP 0x8D
#define COLUMNADDR 0x21
#define COMSCANDEC 0xC8
#define COMSCANINC 0xC0
#define DISPLAYALLON 0xA5
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define EXTERNALVCC 0x1
#define INVERTDISPLAY 0xA7
#define MEMORYMODE 0x20
#define NORMALDISPLAY 0xA6
#define PAGEADDR 0x22
#define SEGREMAP 0xA0
#define SETCOMPINS 0xDA
#define SETCONTRAST 0x81
#define SETDISPLAYCLOCKDIV 0xD5
#define SETDISPLAYOFFSET 0xD3
#define SETHIGHCOLUMN 0x10
#define SETLOWCOLUMN 0x00
#define SETMULTIPLEX 0xA8
#define SETPRECHARGE 0xD9
#define SETSEGMENTREMAP 0xA1
#define SETSTARTLINE 0x40
#define SETVCOMDETECT 0xDB
#define SWITCHCAPVCC 0x2

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif

typedef enum OLEDDISPLAY_COLOR
{
    BLACK = 0,
    WHITE = 1,
    INVERSE = 2
} OLEDDISPLAY_COLOR;

typedef enum OLEDDISPLAY_TEXT_ALIGNMENT
{
    TEXT_ALIGN_LEFT = 0,
    TEXT_ALIGN_RIGHT = 1,
    TEXT_ALIGN_CENTER = 2,
    TEXT_ALIGN_CENTER_BOTH = 3
} OLEDDISPLAY_TEXT_ALIGNMENT;

typedef enum OLEDDISPLAY_GEOMETRY
{
    GEOMETRY_128_64 = 0,
    GEOMETRY_128_32 = 1,
    GEOMETRY_64_48 = 2,
    GEOMETRY_64_32 = 3,
    GEOMETRY_RAWMODE = 4
} OLEDDISPLAY_GEOMETRY;

typedef enum HW_I2C
{
    I2C_ONE,
    I2C_TWO
} HW_I2C;

typedef enum enum_DisplayWindow
{
    Main_Window = 0, //主窗口
    Set_Window,      //设置窗口

    time_set_window = 10,     //设置时间窗口 偏移量: 10
    water_temper_set_window,  //设置水温窗口
    feed_time_set_window,     //喂食时间设置窗口
    light_set_window,         //灯光设置窗口
    system_status_window,     //系统状态查看窗口
    filter_set_window,        //过滤水泵设置窗口
    demonstrat_window,        //演示demo窗口
    distribut_network_window, //配网状态窗口

    time_detail_window = 31, //时间详情窗口 偏移量: 30
    temper_detail_window,    //水温详情窗口

    temper_trig_window = 40, //温度触发窗口 偏移量40
    feed_trig_window,        //喂食触发窗口
    light_trig_window,       //灯光触发窗口
    trig_window_sum,         //触发窗口统计

    system_status_window_2 = 50, //系统状态查看窗口_2 偏移量50
    system_status_window_3,      //系统状态查看窗口_3
    system_status_window_4,      //系统状态查看窗口_3

    demonstrat_window_2 = 60, //演示demo窗口_2 偏移量60
    demonstrat_window_3,
    demonstrat_window_4,
    demonstrat_window_5,
    demonstrat_window_sum,

    distribut_network_window_2 = 70, //配网状态窗口_2 偏移量70 //
    distribut_network_window_sum,

    time_set_by_manual_window = 80, //手动设置时间窗口 偏移量: 80
    time_set_by_net_window,         //联网自动同步时间窗口

    light_set_by_time_window = 90, //按照时间段亮灯光设置窗口 偏移量: 90
    light_set_by_lux_window, //按照环境光照lux亮灯光设置窗口 
    light_set_window_sum, //统计灯光设置窗口 

    Undefined_window,
} typedef_enum_DisplayWindow;

typedef enum DisplaySetWindowSubOption //设置窗口下的子选项
{
    sub_time_option = 0,          //时间设置子选项
    sub_water_temperature_option, //水温加热设置子选项
    sub_feed_option,              //喂食时间设置子选项
    sub_light_option,             //灯光设置子选项
    sub_system_status_option,     //系统状态查看子选项
    sub_filter_set_option,        //过滤设置子选项
    sub_demonstrat_opting,        //演示demo子选项
    sub_distribut_network_opting, //配网状态子选项

    sub_option_Sum, //统计最大的子选项数
} typedef_enum_DisplaySetWindowSubOption;

typedef struct DisplayLine_sub_option_buffer
{
    char buffer[30]; //要显示的字符串
} typedef_stru_DisplayLine_sub_option_buffer;

typedef struct Display_week_string_buffer
{
    char buffer[5]; //存放要显示的星期几的字符串
} typedef_stru_Display_week_string_buffer;

typedef struct stru_DisplaySetSelectCursorLocation //用于记住选项选中光标的位置
{
    uint8_t x;     //方框光标要显示的x位置
    uint8_t y;     //
    uint8_t width; //方框光标要显示的宽度
} type_stru_DisplaySetSelectCursorLocation;

typedef enum enum_TimeSetCursorPosition //时间设置窗口下的光标定位子选项
{
    cursor_position_year = 0, //选中 年的光标
    cursor_position_month,    //选中 月的光标
    cursor_position_day,      //选中 日的光标
    cursor_position_hour,     //选中 小时的光标
    cursor_position_minute,   //选中 分钟的光标
    cursor_position_week,     //选中 星期的光标
    cursor_complete_and_exit, //选中 完成并退出

    cursor_position_sum, //统计子选项数量
} typedef_enum_TimeSetCursorPosition;

typedef enum enum_TimeSetCursorSelecParam //时间设置窗口下的更
{
    cursor_not_selec_param = 0, //没有选中光标 此时开始旋钮跳转选项
    cursor_selec_param,         //选中光标  此时开始旋钮调整参数
} typedef_enum_CursorSelecParam;

typedef enum RefreshDisplayClearScreen //刷新显示清屏
{
    dont_need_clear_screen = 0, //不需要刷新显示清屏
    clear_screen,               //刷新显示清屏
} typedef_enum_RefreshDisplayScreenOption;

typedef enum enum_ShowSquareBox //显示方框?
{
    Normal = 0,           //不显示方框
    ShowSquareBox,        //显示方框
    ShowInverseSquareBox, //亮灭翻转显示选中框 用于选中指示
} typedef_enum_ShowSquareBox;

typedef struct stru_TimeSetWindowParam
{
    // typedef_enum_CursorSelecParam enum_TimeSetCursorSelecParam;
    // typedef_enum_TimeSetCursorPosition enum_TimeSetCursorPosition;
    // void *Point_OledUpdateDisplayTime;
    void *Point_OledDs1302dev;
    typedef_stru_Display_week_string_buffer Display_week_string_buffer[8]; //星期1 到 星期日 7
    type_stru_DisplaySetSelectCursorLocation stru_TimeSetSelectCursorLocation[cursor_position_sum];
} type_stru_TimeSetWindowParam;

typedef struct CounterList
{
    volatile unsigned char valid;  //计时器有效标志位, 相当于enable //互斥锁 volatile关键字防止编译器优化
    volatile unsigned int counter; //计时器存放的值
} CounterList_StructTypedef;

typedef enum CounterId
{
    options_selected_blink_timer = 0, //选项被选中则闪烁处理计时器
    // time_details_regular_refresh_timer, //时间详情定时刷新处理计时器
    display_refresh_timer,          //显示定时刷新处理计时器
    main_window_display_time_timer, //主窗口显示时间定时刷新处理计时器
    temper_trig_window_close_timer, //温度触发窗口定时关闭退出处理计时器
    feed_trig_window_close_timer,   //喂食触发窗口定时关闭退出处理计时器
    // system_status_window_refresh_timer, //系统状态窗口定时刷新显示计时器
    window_refresh_timer, //窗口通用定时刷新显示计时器
    indicator_mqtt_blink_timer, //mqtt状态指示灯闪烁计时器

    // GOTO_SLEEP_TIMER,//进入睡眠的的定时器
    // BINITIALIZED_TIMER,//初始化阶段的定时器

    TIMER_SUM, //计时器数量总数
               //注意定时器设置时间不能超过10分钟
} CounterId_ETypeDef;

typedef enum enum_WaterTemperSetCursorPosition //时间设置窗口下的光标定位子选项
{
    cursor_switch = 0,         //选中 使能开关的光标
    cursor_temper_upper_limit, //选中 温度上限的光标
    cursor_temper_lower_limit, //选中 温度下限的光标

    water_temper_cursor_sum, //统计水温设置子选项数量 并充当"完成并退出"选项
} type_enum_WaterTemperSetCursorPosition;

typedef struct stru_UpperLowerLimitTemperParam //水温上下限
{
    uint8_t Switch;
    float upper_limit; //
    float lower_limit; //
} type_stru_UpperLowerLimitTemperParam;

typedef struct stru_TemperWindowparam
{
    typedef_enum_CursorSelecParam enum_TemperCursorSelecParam;
    // type_enum_WaterTemperSetCursorPosition enum_WaterTemperSetCursorPosition;
    type_stru_DisplaySetSelectCursorLocation stru_TemperSetSelectCursorLocation[water_temper_cursor_sum + 1];
    // type_stru_UpperLowerLimitTemperParam stru_UpperLowerLimitTemperParam;
    // void *Point_stru_UpperLowerLimitTemperParam;
} type_stru_TemperWindowparam;

#define COUNT_10_MS 1                   //计时器10ms需要的计数值
#define COUNT_100_MS (COUNT_10_MS * 10) //计时器100ms需要的计数值
#define COUNT_1_S (COUNT_100_MS * 10)   //计时器1s需要的计数值

typedef enum enum_MainWindowCursorPosition //时间设置窗口下的光标定位子选项
{
    cursor_title = 0,    //选中 标题的光标
    cursor_time,         //选中 时间的光标
    cursor_water_temper, //选中 水温的光标

    main_window_cursor_sum, //统计主窗口子选项数量
} type_enum_MainWindowCursorPosition;

typedef struct stru_MainWindowparam //主窗口显示的参数定义
{
    type_enum_MainWindowCursorPosition enum_MainWindowCursorPosition;
} type_stru_MainWindowparam;

typedef char (*FontTableLookupFunction)(const uint8_t ch);
char DefaultFontTableLookup(const uint8_t ch);
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

typedef struct OLEDDisplay_s
{
    uint8_t *buffer;

#ifdef OLEDDISPLAY_DOUBLE_BUFFER
    uint8_t *buffer_back;
#endif

    OLEDDISPLAY_GEOMETRY geometry;

    uint16_t width;
    uint16_t height;
    uint16_t displayBufferSize;

    OLEDDISPLAY_TEXT_ALIGNMENT textAlignment;
    OLEDDISPLAY_COLOR color;

    const uint8_t *fontData;

    // State values for logBuffer
    uint16_t logBufferSize;
    uint16_t logBufferFilled;
    uint16_t logBufferLine;
    uint16_t logBufferMaxLines;
    char *logBuffer;
    FontTableLookupFunction fontTableLookupFunction;

    // ESP I2C
    uint16_t i2c_sda_gpio;
    uint16_t i2c_scl_gpio;
    uint16_t i2c_port;
    uint16_t i2c_addr;
    i2c_cmd_handle_t i2c_cmd;

    // user
    // bool Flag_RenewEntireScreen; //整个界面刷新一次标志位 全屏更新
    // bool Flag_Part_1_Update;     //局部更新
    // bool Flag_Part_2_Update;     //
    // bool Flag_Part_3_Update;     //

    // bool Flag_TriParamUpdate; //触发参数发生更新标志位
    // bool Flag_LightSetSync; //灯光设置同步
    void *Point_EepromDev;
    uint8_t *Point_ReadEepromBuffer;
    // uint8_t *Point_OutPutStringBuffer; //用于oled格式化Printf输出
    typedef_enum_DisplayWindow enum_DisplayWindow;
    uint8_t DisplayStep; //显示分步骤处理 用于初始化
    typedef_stru_DisplayLine_sub_option_buffer DisplayLine_sub_option_buffer[sub_option_Sum + 1];
    // char DisplayLine_sub_option_buffer[sub_option_Sum + 1][30]; //子选项格式化的显示字符串存放
    type_stru_TimeSetWindowParam stru_TimeSetWindowParam;
    CounterList_StructTypedef sCounterList[TIMER_SUM]; //计时器结构体数组定义
    type_stru_TemperWindowparam stru_TemperWindowparam;
    type_stru_MainWindowparam stru_MainWindowparam;
} OLEDDisplay_t;

// Use this to resume after a deep sleep without resetting the display (what init() would do).
// Returns true if connection to the display was established and the buffer allocated, false otherwise.
int OLEDDisplay_allocateBuffer(OLEDDisplay_t *oled);

// Allocates the buffer and initializes the driver & display. Resets the display!
// Returns false if buffer allocation failed, true otherwise.
OLEDDisplay_t *OLEDDisplay_init(int port, int addr, int sda, int scl, uint32_t frequecy);

// Free the memory used by the display
void OLEDDisplay_end(OLEDDisplay_t *oled);

// Cycle through the initialization
void OLEDDisplay_resetDisplay(OLEDDisplay_t *oled);

/* Drawing functions */
// Sets the color of all pixel operations
void OLEDDisplay_setColor(OLEDDisplay_t *oled, OLEDDISPLAY_COLOR color);

// Returns the current color.
OLEDDISPLAY_COLOR OLEDDisplay_getColor();

// Draw a pixel at given position
void OLEDDisplay_setPixel(OLEDDisplay_t *oled, int16_t x, int16_t y);

// Draw a pixel at given position and color
void OLEDDisplay_setPixelColor(OLEDDisplay_t *oled, int16_t x, int16_t y, OLEDDISPLAY_COLOR color);

// Clear a pixel at given position FIXME: INVERSE is untested with this function
void OLEDDisplay_clearPixel(OLEDDisplay_t *oled, int16_t x, int16_t y);

// Draw a line from position 0 to position 1
void OLEDDisplay_drawLine(OLEDDisplay_t *oled, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

// Draw the border of a rectangle at the given location
void OLEDDisplay_drawRect(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t width, int16_t height);

// Fill the rectangle
void OLEDDisplay_fillRect(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t width, int16_t height);

// Draw the border of a circle
void OLEDDisplay_drawCircle(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t radius);

// Draw all Quadrants specified in the quads bit mask
void OLEDDisplay_drawCircleQuads(OLEDDisplay_t *oled, int16_t x0, int16_t y0, int16_t radius, uint8_t quads);

// Fill circle
void OLEDDisplay_fillCircle(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t radius);

// Draw a line horizontally
void OLEDDisplay_drawHorizontalLine(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t length);

// Draw a line vertically
void OLEDDisplay_drawVerticalLine(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t length);

// Draws a rounded progress bar with the outer dimensions given by width and height. Progress is
// a unsigned byte value between 0 and 100
void OLEDDisplay_drawProgressBar(OLEDDisplay_t *oled, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);

// Draw a bitmap in the internal image format
void OLEDDisplay_drawFastImage(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *image);

// Draw a XBM
void OLEDDisplay_drawXbm(OLEDDisplay_t *oled, int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *xbm);

// Draw icon 16x16 xbm format
void OLEDDisplay_drawIco16x16(OLEDDisplay_t *oled, int16_t x, int16_t y, const char *ico, int inverse);

/* Text functions */

// Draws a string at the given location
void OLEDDisplay_drawString(OLEDDisplay_t *oled, int16_t x, int16_t y, char *text);

// Draws a formatted string (like printf) at the given location
void OLEDDisplay_drawStringf(OLEDDisplay_t *oled, int16_t x, int16_t y, char *format, ...);

// Draws a String with a maximum width at the given location.
// If the given String is wider than the specified width
// The text will be wrapped to the next line at a space or dash
void OLEDDisplay_drawStringMaxWidth(OLEDDisplay_t *oled, int16_t x, int16_t y, uint16_t maxLineWidth, char *text);

// Returns the width of the const char* with the current
// font settings
uint16_t OLEDDisplay_getStringWidthLen(OLEDDisplay_t *oled, const char *text, uint16_t length);
uint16_t OLEDDisplay_getStringWidth(OLEDDisplay_t *oled, const char *text);

// Specifies relative to which anchor point
// the text is rendered. Available constants:
// TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
void OLEDDisplay_setTextAlignment(OLEDDisplay_t *oled, OLEDDISPLAY_TEXT_ALIGNMENT textAlignment);

// Sets the current font. Available default fonts
// ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
void OLEDDisplay_setFont(OLEDDisplay_t *oled, const uint8_t *fontData);

// Set the function that will convert utf-8 to font table index
void OLEDDisplay_setFontTableLookupFunction(OLEDDisplay_t *oled, FontTableLookupFunction function);

/* Display functions */

// Turn the display on
void OLEDDisplay_displayOn(OLEDDisplay_t *oled);

// Turn the display offs
void OLEDDisplay_displayOff(OLEDDisplay_t *oled);

// Inverted display mode
void OLEDDisplay_invertDisplay(OLEDDisplay_t *oled);

// Normal display mode
void OLEDDisplay_normalDisplay(OLEDDisplay_t *oled);

// Set display contrast
// really low brightness & contrast: contrast = 10, precharge = 5, comdetect = 0
// normal brightness & contrast:  contrast = 100
void OLEDDisplay_setContrast_internal(OLEDDisplay_t *oled, uint8_t contrast, uint8_t precharge, uint8_t comdetect);
static inline void OLEDDisplay_setContrast(OLEDDisplay_t *oled, uint8_t contrast)
{
    OLEDDisplay_setContrast_internal(oled, contrast, 241, 64);
}

// Convenience method to access
void OLEDDisplay_setBrightness(OLEDDisplay_t *oled, uint8_t);

// Reset display rotation or mirroring
void OLEDDisplay_resetOrientation(OLEDDisplay_t *oled);

// Turn the display upside down
void OLEDDisplay_flipScreenVertically(OLEDDisplay_t *oled);

// Mirror the display (to be used in a mirror or as a projector)
void OLEDDisplay_mirrorScreen(OLEDDisplay_t *oled);

// Clear the local pixel buffer
void OLEDDisplay_clear(OLEDDisplay_t *oled);

// Log buffer implementation

// This will define the lines and characters you can
// print to the screen. When you exeed the buffer size (lines * chars)
// the output may be truncated due to the size constraint.
int OLEDDisplay_setLogBuffer(OLEDDisplay_t *oled, uint16_t lines, uint16_t chars);

// Draw the log buffer at position (x, y)
void OLEDDisplay_drawLogBuffer(OLEDDisplay_t *oled, uint16_t x, uint16_t y);

// Get screen geometry
uint16_t OLEDDisplay_getWidth(OLEDDisplay_t *oled);
uint16_t OLEDDisplay_getHeight(OLEDDisplay_t *oled);

// Implement needed function to be compatible with Print class
size_t OLEDDisplay_write(OLEDDisplay_t *oled, uint8_t ch);
size_t OLEDDisplay_writeString(OLEDDisplay_t *oled, const char *s);

// Set the correct height, width and buffer for the geometry
void OLEDDisplay_setGeometry(OLEDDisplay_t *oled, OLEDDISPLAY_GEOMETRY g, uint16_t width, uint16_t height);

// Send all the init commands
void OLEDDisplay_sendInitCommands(OLEDDisplay_t *oled);

// converts utf8 characters to extended ascii
char *OLEDDisplay_utf8ascii(OLEDDisplay_t *oled, char *s);

void OLEDDisplay_drawInternal(OLEDDisplay_t *oled, int16_t xMove, int16_t yMove, int16_t width, int16_t height, const uint8_t *data, uint16_t offset, uint16_t bytesInData);

void OLEDDisplay_drawStringInternal(OLEDDisplay_t *oled, int16_t xMove, int16_t yMove, char *text, uint16_t textLength, uint16_t textWidth);
void OLEDDisplay_display(OLEDDisplay_t *oled);
/*
    x:横坐标，0-127
    y:0-63
    *s:要显示的字符（中英文均可，前提是数组中包含的汉字）
*/
void OledChineseEnglishMixedPrint(OLEDDisplay_t *oled, typedef_enum_RefreshDisplayScreenOption DisplayClearScreen, typedef_enum_ShowSquareBox enum_ShowSquareBox, unsigned char x, unsigned char y, char *fmt, ...);
// Refresh传入1则刷新整屏
void RefreshEntireScreenPro(OLEDDisplay_t *oled, uint8_t Refresh);
unsigned int get_counter(OLEDDisplay_t *oled, CounterId_ETypeDef id);
void register_counter(OLEDDisplay_t *oled, CounterId_ETypeDef id, unsigned int counter);
void counter_Initial(OLEDDisplay_t *oled);
void counter_handle(OLEDDisplay_t *oled);
void ReverseCodeDisplayPro(OLEDDisplay_t *oled, unsigned char x, unsigned char y, char *s);
void SelectdisplayLineBoxPro(OLEDDisplay_t *oled, unsigned char y);
// void SetAccordPresetDisplayBoxesPro(OLEDDisplay_t *oled, type_stru_DisplaySetSelectCursorLocation *cursor_location, typedef_enum_TimeSetCursorPosition SetCursorPosition);

#endif
