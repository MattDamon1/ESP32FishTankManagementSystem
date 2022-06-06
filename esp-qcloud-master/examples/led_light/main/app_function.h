#ifndef APP_FUNCTION_H
#define APP_FUNCTION_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "esp_qcloud_log.h"
#include "esp_qcloud_console.h"
#include "esp_qcloud_storage.h"
#include "esp_qcloud_iothub.h"
#include "esp_qcloud_prov.h"

#include "light_driver.h"

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "OLEDDisplay.h"
#include "rotary_encoder.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "ds1302.h"
#include "ds18b20.h"
#include "at24c.h"
#include "app_function.h"
#include "driver/adc.h"
#include "driver/dac.h"

#define u8 unsigned char

#define CONFIG_ROT_ENC_CLK_GPIO 35
#define CONFIG_ROT_ENC_DTA_GPIO 32
// #define CONFIG_ROT_ENC_CLK_GPIO 32
// #define CONFIG_ROT_ENC_DTA_GPIO 35
#define CONFIG_ROT_ENC_DEBOUNCE 2000
#define CONFIG_ROT_ENC_BUTTON_GPIO 34
#define CONFIG_ROT_ENC_BUTTON_DEBOUNCE 80000 // us

#define HEATING_BASEBALL_GPIO_PIN 21
#define HEATING_BASEBALL_ON 1
#define HEATING_BASEBALL_OFF 0

#define INDICATOR_MQTT_GPIO_PIN 18
#define INDICATOR_MQTT_ON 1
#define INDICATOR_MQTT_OFF 0

#define FILTER_DRIVE_GPIO_PIN 13 //过滤驱动io口定义
#define FILTER_DRIVE_ON 1
#define FILTER_DRIVE_OFF 0

#define LEDC_LIGHT_TIMER LEDC_TIMER_3 //灯光pwm参数设置
#define LEDC_LIGHT_MODE LEDC_LOW_SPEED_MODE
#define LEDC_LIGHT_OUTPUT_IO (19) // Define the output GPIO
#define LEDC_LIGHT_CHANNEL LEDC_CHANNEL_7
#define LEDC_LIGHT_DUTY_RES LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_LIGHT_FREQUENCY (5000)           // Frequency in Hertz. Set frequency at 5 kHz

#define LEDC_STEER_GEAR_TIMER LEDC_TIMER_2
#define LEDC_STEER_GEAR_MODE LEDC_LOW_SPEED_MODE
#define LEDC_STEER_GEAR_OUTPUT_IO (12) // Define the output GPIO
#define LEDC_STEER_GEAR_CHANNEL LEDC_CHANNEL_6
#define LEDC_STEER_GEAR_DUTY_RES LEDC_TIMER_16_BIT // Set duty resolution to 13 bits
#define LEDC_STEER_GEAR_FREQUENCY (50)             // Frequency in Hertz.

#define STEER_GEAR_ANGLE_0 1641 // 舵机0度角Pwm装载值 //16bit 65535
// #define STEER_GEAR_ANGLE_45 409   // 舵机45度角Pwm装载值
// #define STEER_GEAR_ANGLE_90 614   // 舵机90度角Pwm装载值
// #define STEER_GEAR_ANGLE_135 819  // 舵机135度角Pwm装载值
#define STEER_GEAR_ANGLE_180 8193 // 舵机180度角Pwm装载值

// 创建队列句柄
// QueueHandle_t xQueue_DisplayLine_1; //一共有4行要刷新显示的内容
// QueueHandle_t xQueue_DisplayLine_2;
// QueueHandle_t xQueue_DisplayLine_3;
// QueueHandle_t xQueue_DisplayLine_4;

QueueHandle_t xQueue_ReadTemp;        //用于传递温度参数
QueueHandle_t xQueue_TemperTrigParam; //用于传递温度触发参数 用于显示

QueueHandle_t xQueue_CurrentTime;    //用于当前时间  用于喂食时间触发
QueueHandle_t xQueue_FeedTrigParam;  //用于传递喂食触发参数 用于显示
QueueHandle_t xQueue_LightTrigParam; //用于传递灯光触发参数 用于显示

QueueHandle_t xQueue_LuxParam; //用于传递当前Lux参数 用于显示和触发

QueueHandle_t xqueue_SystemStatusParam; //用于传递当前各种触发参数 用于系统状态显示

QueueHandle_t xqueue_WifiConfigParam; //用于传递当前wifi参数 用于配网状态显示 wifi名称和密码

SemaphoreHandle_t BinSemap_LightSetSync;   //二值信号量 用于灯光设置与触发之间的同步
SemaphoreHandle_t BinSemap_SetSync;        //二值信号量 用于所有设置与触发之间的同步
SemaphoreHandle_t BinSemap_CloudOrderSync; //二值信号量 用于云端下方指令与动作间的同步
SemaphoreHandle_t BinSemap_DemoExecutSync; //二值信号量 用于演示demo执行结束后重新触发一些条件动作的同步

SemaphoreHandle_t BinSemap_FullScreenUpdate; //二值信号量 用于显示更新 全屏更新
SemaphoreHandle_t BinSemap_PartUpdate_1;     //二值信号量 用于显示更新 部分1更新
SemaphoreHandle_t BinSemap_PartUpdate_2;     //二值信号量 用于显示更新 部分2更新
SemaphoreHandle_t BinSemap_PartUpdate_3;     //二值信号量 用于显示更新 部分3更新
typedef struct DisplayLine                   //发送到队列的结构体
{
    uint8_t x;       //要显示的x位置
    uint8_t y;       //
    char buffer[30]; //要显示的字符串
} typedef_stru_DisplayLine;

typedef enum enum_Rotation
{
    key_not_rotat = 0,
    key_up_right,
    key_down_left,
} type_enum_Rotation;

typedef enum enum_KeyPress
{
    key_not_press = 0,
    key_short_press,
    key_long_press,
} type_enum_KeyPress;

typedef struct stru_KeyValue
{
    type_enum_Rotation enum_Rotation;
    type_enum_KeyPress enum_KeyPress;
} type_stru_KeyValue;

typedef enum enum_UpperOrLowerTrig
{
    temper_not_trig = 0,
    temper_upper_trig,
    temper_lower_trig,
} type_enum_UpperOrLowerTrig;

typedef struct stru_TemperTrigParam
{
    type_enum_UpperOrLowerTrig enum_UpperOrLowerTrig;
    float UpperVal;
    float LowerVal;
} type_stru_TemperTrigParam;

typedef struct stru_FeedTrigParam
{
    uint8_t FeedTrigIndex;
    uint8_t FeedTrigDuration;
    uint8_t Flag_Trig; //喂食触发标志位 1分钟内只能触发一次
} type_stru_FeedTrigParam;

typedef struct stru_CloudLightByManualParam
{
    uint8_t LightByManualSwitch;
    uint8_t LightByManualBright;
} type_stru_CloudLightByManualParam;

typedef struct stru_Menu
{
    typedef_enum_DisplayWindow Cur_Index;                                              //当前索引项
    typedef_enum_DisplayWindow up;                                                     //上键
    typedef_enum_DisplayWindow down;                                                   //下键
    typedef_enum_DisplayWindow enter;                                                  //确认键
    void (*current_operation)(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue); //	当前索引执行的函数(界面)
} type_stru_Menu;

typedef enum enum_TimeDetailCursorPosition //时间详情窗口下的光标定位子选项
{
    time_detail_cursor_title = 0, //选中 标题的光标

    time_detail_cursor_sum, //统计窗口子选项数量
} typedef_enum_TimeDetailCursorPosition;

typedef enum enum_FeedTimeSetCursorPosition //喂食时间设置窗口下的光标定位子选项
{
    feed_cursor_position_switch = 0, //选中 功能使能的光标
    feed_cursor_position_table,      //选中 第几组别喂食时间的光标
    feed_cursor_position_hour,       //选中 小时的光标
    feed_cursor_position_minute,     //选中 分钟的光标
    feed_cursor_position_duration,   //选中 喂食持续时间的光标

    feed_cursor_position_sum, //统计子选项数量
} typedef_enum_FeedTimeSetCursorPosition;

typedef enum enum_LightSetCursor //喂食时间设置窗口下的光标定位子选项位置
{
    light_cursor_switch = 0, //选中 功能使能的光标
    light_cursor_mode,       //选中 模式选择的光标

    light_cursor_position_sum, //统计子选项数量 充当进入设置的光标
} typedef_enum_LightSetCursor;

typedef enum enum_LightSetByTimeCursor //按照时间段方式 亮灯设置窗口下的光标定位子选项位置
{
    light_by_time_cursor_interval_table, //选中 区间的光标
    light_by_time_cursor_begin_hour,     //选中 开始的小时的光标
    light_by_time_cursor_begin_minute,   //选中 开始的分钟的光标
    light_by_time_cursor_end_hour,       //选中 结束的小时的光标
    light_by_time_cursor_end_minute,     //选中 结束的分钟的光标
    light_by_time_cursor_bright,         //选中 亮度的光标

    light_by_time_cursor_sum, //统计子选项数量
} typedef_enum_LightSetByTimeCursor;

typedef enum enum_LightSetByLuxCursor //按照环境Lux的方式 亮灯设置窗口下的光标定位子选项位置
{
    light_by_lux_cursor_interval_table, //选中 区间的光标
    light_by_lux_cursor_lower_val,      //选中 开始的lux值的光标
    light_by_lux_cursor_upper_val,      //选中 上限lux值的光标
    light_by_lux_cursor_bright,         //选中 亮度的光标

    light_by_lux_cursor_sum, //统计子选项数量
} type_enum_LightSetByLuxCursor;

typedef enum enum_CloudOrder //云端下发的指令 对应的动作处理
{
    cloud_order_not_set = 0, //没有对应指令动作
    cloud_order_feed,        //喂食的指令动作
    cloud_order_light,       //灯光的指令动作

    cloud_order_sum, //统计指令动作数量
} type_enum_CloudOrder;

typedef enum enum_FilterSetCursor //喂食时间设置窗口下的光标定位子选项位置
{
    filter_cursor_switch = 0,   //选中 功能使能的光标
    filter_cursor_begin_hour,   //选中 开始的小时的光标
    filter_cursor_begin_minute, //选中 开始的分钟的光标
    filter_cursor_end_hour,     //选中 结束的小时的光标
    filter_cursor_end_minute,   //选中 结束的分钟的光标

    filter_cursor_position_sum, //统计子选项数量
} type_enum_FilterSetCursor;

/*!
 * \brief feed time structure
 */
typedef struct stru_FeedTime
{
    uint8_t minute; //!< Minute 0..59
    uint8_t hour;   //!< Hour 0..23
} type_stru_FeedTime;

typedef struct stru_SetFeedTimeTable //喂食时间设置窗口下的参数结构体
{
    uint8_t FeedSwitch;
    uint8_t FeedDuration[3]; //喂食持续时间 单位 S
    type_stru_FeedTime stru_SetFeedTime[3];
} typedef_stru_SetFeedTimeTable;

typedef enum enum_LightSetIntervalIndex //喂食时间设置窗口下的区间光标定位子选项位置索引
{
    light_interval_index_day = 0, //选中 区间日间区间索引
    light_Interval_index_dusk,    //选中 区间傍晚区间索引
    light_Interval_index_night,   //选中 区间夜间区间索引

    light_Interval_index_sum, //统计子选项数量
} typedef_enum_LightSetIntervalIndex;

typedef enum enum_LightSetModeIndex //灯光设置 的 模式索引
{
    mode_index_by_time = 0, //选中 按照时间段
    mode_index_by_lux,      //选中 按照Lux

    mode_index_sum, //统计子选项数量
} typedef_enum_LightSetModeIndex;

typedef struct stru_LightByLux //灯光设置下的 按照lux区间参数
{
    uint8_t LightBright; // 3组亮度值
    uint8_t LowerLux;    //
    uint8_t UpperLux;    //
} type_stru_LightByLux;

typedef struct stru_LightByTime //灯光设置下的 安装时间区间参数
{
    uint8_t LightBright; // 3组亮度值
    type_stru_FeedTime stru_BeginTime;
    type_stru_FeedTime stru_EndTime;
} type_stru_LightByTime;

typedef struct stru_LightSet //灯光设置窗口下的参数结构体
{
    uint8_t LightSwitch;
    typedef_enum_LightSetModeIndex enum_LightSetModeIndex;
    type_stru_LightByLux stru_LightByLux[light_Interval_index_sum];
    type_stru_LightByTime stru_LightByTime[light_Interval_index_sum];
} typedef_stru_LightSet;

typedef struct stru_FilterSet //过滤水泵设置窗口下的参数结构体
{
    uint8_t FilterSwitch;
    type_stru_FeedTime stru_FilterBeginTime;
    type_stru_FeedTime stru_FilterEndTime;
} type_stru_FilterSet;

typedef struct stru_LightTrigParam
{
    typedef_enum_LightSetIntervalIndex LightTrigIntervalIndex;
    typedef_enum_LightSetModeIndex LightTrigModeIndex;
    uint8_t LightTrigBright;
} type_stru_LightTrigParam;

typedef enum enum_EepromAddress // eeprom地址枚举
{
    temper_addr = 0, //水温参数存储eeprom的地址

    feed_time_addr = temper_addr + (sizeof(type_stru_UpperLowerLimitTemperParam) + 1), //喂食时间结构体参数存放地址 偏移水温参数空间大小加一个字节校验位

    light_addr = feed_time_addr + (sizeof(typedef_stru_SetFeedTimeTable) + 1), //灯光结构体参数存放地址 偏移喂食时间参数空间大小加一个字节校验位

    filter_addr = light_addr + (sizeof(typedef_stru_LightSet) + 1), //过滤参数存放地址 偏移灯光设置参数空间大小加一个字节校验位
    // feed_time_addr = temper_addr + 20, //

    eeprom_address_sum, //统计最大地址
} type_enum_EepromAddress;

typedef enum ShowUpdateSelection //更新显示选择
{
    Full_Screen_Update_Display = 0,
    Part_1_Update_Display,
    Part_2_Update_Display,
    Part_3_Update_Display,
} typedef_enum_ShowUpdateSelection;

typedef struct stru_SystemStatusWindowParam //系统状态窗口下的 显示参数
{
    uint8_t WaterTemperSwitch;               // 记录水温设置开关状态
    uint8_t FeedSwitch;                      // 记录喂食设置开关状态
    uint8_t LightSwitch;                     // 记录灯光设置开关状态
    uint8_t ToBeFeedTrigIndex;               //记录下一组即将喂食触发的组别
    type_stru_FeedTime stru_ToBeTriFeedTime; //下一组即将喂食触发的时间
} type_stru_SystemStatusWindowParam;

typedef struct stru_DistributNetWindowParam //配网状态窗口下的 显示参数
{
    uint8_t MqttNetConnect;   // 记录Mqtt连接状态
    uint8_t WifiId[20];       // 记录连接的wifi名称
    uint8_t WifiPassWord[20]; // 记录连接的wifi密码
} type_stru_DistributNetWindowParam;

type_enum_CloudOrder enum_CloudOrder;
type_stru_CloudLightByManualParam stru_CloudLightByManualParam;
type_stru_FeedTrigParam stru_CloudFeedByManualParam;
type_stru_DistributNetWindowParam stru_DistributNetWindowParam;

void GUI_Refresh(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);
void MainWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue); //主窗口显s示函数
void Get_KEY_Value(rotenc_handle_t *handle, type_stru_KeyValue *stru_KeyValue);
void SetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);            //设置窗口显示函数
void TimeSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);        //子选项时间设置窗口显示函数
void WaterTemperSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue); //子选项时间设置窗口显示函数
uint8_t ReadEeprom(EEPROM_t *eeprom_dev, uint8_t *Point_ReadEepromBuffer, uint16_t data_addr, uint8_t ReadSize);
void TimeDetailWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);  //时间详情窗口显示函数
void ActionTrigPro(OLEDDisplay_t *oled);                                           //触发条件判断处理
void TemperTrigWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);  //温度触发窗口显示函数
void FeedTimeSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue); //喂食时间设置窗口显示函数
void FeedTrigWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);    //喂食时间设置窗口显示函数
void LightSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);    //灯光设置窗口显示函数
void LightTrigWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);   //灯光触发窗口显示函数
void LedcPwmInit(void);                                                            // PWM初始化
void LedcLightPwmSetDuty(uint32_t duty);                                           //设置灯光pwm占空比 0~8190
void LedcSteerGearPwmSetDuty(uint32_t duty);                                       //设置舵机的pwm占空比 0~8190
void SelectPartOrFullScreenUpdateDisplay(typedef_enum_ShowUpdateSelection ShowUpdateSelection);
BaseType_t GetPartOrFullScreenUpdateDisplay(typedef_enum_ShowUpdateSelection ShowUpdateSelection); // 获取部分或全屏更新二值信号量
void SystemStatusWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);                //系统状态查看窗口显示函数
void ReadAdcAveValPro(adc_channel_t adc_channel, uint16_t *ReadAdc);
uint16_t filter(adc_channel_t adc_channel);
void SystemStatusWindow_2Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);     //系统状态查看窗口2显示函数
void SystemStatusWindow_3Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);     //系统状态查看窗口3显示函数
void FilterSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);          //过滤水泵设置窗口显示函数
void SystemStatusWindow_4Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);     //系统状态查看窗口4显示函数
void DemonstratWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);         //演示demo窗口显示函数
void DemonstratWindow_2Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);       //演示demo窗口_2显示函数
void DemonstratWindow_3Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);       //演示demo窗口_3显示函数
void DemonstratWindow_4Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);       //演示demo窗口_4显示函数
void DemonstratWindow_5Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);       //演示demo窗口_5显示函数
void CloudFeedByManualSwitchPro(bool OnOff);                                              //云端下发开始喂食指令处理函数
void CloudLightByManualSwitchPro(bool OnOff);                                             //云端下发 手动开灯指令处理函数
void CloudLightByManualBrightPro(int bright);                                             //云端下发 手动开灯亮度指令处理函数
void CloudFeedByManualTimePro(int feedtime);                                              //云端下发 喂食时间指令处理函数
void DistributNetworkWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);   //配网状态窗口显示函数
int CloudGetFeedTime(void);                                                               //上报给云端开始喂食时间处理函数
int CloudGetLightBright(void);                                                            //上报给云端开始亮度值处理函数
void DistributNetworkWindow_2Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue); //配网状态窗口_2显示函数
void TimeSetByManualWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);    //手动设置时间设置窗口显示函数
void TimeSetByNetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);       //联网自动同步时间窗口显示函数
void LightSetByTimeWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);     //按照时间段亮灯的灯光设置窗口显示函数
void LightSetByLuxWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue);      //按照环境光照Lux亮灯的灯光设置窗口显示函数
void IndicatorMqttPro(OLEDDisplay_t *oled);                                               // Mqtt连接状态指示灯处理函数

#endif