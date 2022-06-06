/*
 *
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 *
 */

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

#ifdef CONFIG_BT_ENABLE
#include "esp_bt.h"
#endif

// GPIO DEFINITION
// #define COLD_GPIO_NUM 12
// #define LOW_SPEED_GPIO_NUM 14
// #define MIDDLE_SPEED_GPIO_NUM 27
// #define HIGH_SPEED_GPIO_NUM 26

// #define LOW 0
// #define HIGH 1

#define CONFIG_I2C_MASTER_PORT_NUM 1
#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define OLED_I2C_MASTER_SCL 22
#define OLED_I2C_MASTER_SDA 23
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 900000                             // I2C频率

#define CONFIG_CLK_GPIO 27 // ds1302引脚定义
#define CONFIG_IO_GPIO 26
#define CONFIG_CE_GPIO 25
#define CONFIG_TIMEZONE 8

#define DS18B20_PIN 14

#define CONFIG_24C256_SCL_GPIO OLED_I2C_MASTER_SCL //与oled共用一个iic引脚
#define CONFIG_24C256_SDA_GPIO OLED_I2C_MASTER_SDA
#define CONFIG_24C256_I2C_ADDRESS 0x50
#define EEPROM_SIZE 256

static char *TAG = "app_main";

//把格式化输出字符串存到*buffer里
int vspf(char *buffer, char *fmt, ...)
{
    va_list argptr;
    int cnt;
    va_start(argptr, fmt);
    cnt = vsprintf(buffer, fmt, argptr);
    va_end(argptr);
    return (cnt);
}

//向队列发送格式化的字符串
void QueuePartUpdateDisplayStringPro(QueueHandle_t xQueue_Handle, unsigned char x, unsigned char y, char *fmt, ...)
{
    va_list argptr;
    int cnt;
    typedef_stru_DisplayLine DisplayLineTemp = {
        .x = x, //设置显示行x坐标
        .y = y, //设置显示列y坐标
    };
    // vspf(&DisplayLineTemp.buffer[0], fmt);
    va_start(argptr, fmt);
    cnt = vsprintf(&DisplayLineTemp.buffer[0], fmt, argptr);
    va_end(argptr);
    xQueueOverwrite(xQueue_Handle, &DisplayLineTemp);
}

static void button_callback(void *arg)
{
    rotenc_handle_t *handle = (rotenc_handle_t *)arg;
    ESP_LOGI(TAG, " rotary encoder Press ButtonPress= %d", handle->state.ButtonPress);

    handle->state.position = 0;

    rotenc_event_t event = {
        .position = handle->state.position,
        .direction = handle->state.direction,
        .ButtonPress = handle->state.ButtonPress,
    };

    if (handle->q_event.queue)
    {
        xQueueOverwrite(handle->q_event.queue, &event);
    }
    // ESP_ERROR_CHECK(rotenc_reset(handle));
}

void timer1Interrupt(void *args)
{
    OLEDDisplay_t *oled = (OLEDDisplay_t *)args;
    counter_handle(oled);
}

//将显示的格式化字符串存放到buffer
void SubOptionStringExtractPro(OLEDDisplay_t *oled)
{
    vspf(&oled->DisplayLine_sub_option_buffer[sub_time_option].buffer[0], "时间设置");              //将显示的格式化字符串放到buffer
    vspf(&oled->DisplayLine_sub_option_buffer[sub_water_temperature_option].buffer[0], "水温设置"); //将显示的格式化字符串放到buffer
    vspf(&oled->DisplayLine_sub_option_buffer[sub_feed_option].buffer[0], "喂食设置");              //将显示的格式化字符串放到buffer
    vspf(&oled->DisplayLine_sub_option_buffer[sub_light_option].buffer[0], "灯光设置");             //将显示的格式化字符串放到buffer
    vspf(&oled->DisplayLine_sub_option_buffer[sub_system_status_option].buffer[0], "系统状态");     //将显示的格式化字符串放到buffer
    vspf(&oled->DisplayLine_sub_option_buffer[sub_filter_set_option].buffer[0], "过滤水泵");        //将显示的格式化字符串放到buffer
    vspf(&oled->DisplayLine_sub_option_buffer[sub_demonstrat_opting].buffer[0], "演示demo");        //将显示的格式化字符串放到buffer
    vspf(&oled->DisplayLine_sub_option_buffer[sub_distribut_network_opting].buffer[0], "配网状态"); //将显示的格式化字符串放到buffer

    vspf(&oled->DisplayLine_sub_option_buffer[sub_option_Sum].buffer[0], "退出设置"); //将显示的格式化字符串放到buffer

    // vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[0].buffer[0], "日"); //将显示的格式化字符串放到buffer
    vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[7].buffer[0], "日"); //将显示的格式化字符串放到buffer
    vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[1].buffer[0], "一"); //将显示的格式化字符串放到buffer
    vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[2].buffer[0], "二"); //将显示的格式化字符串放到buffer
    vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[3].buffer[0], "三"); //将显示的格式化字符串放到buffer
    vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[4].buffer[0], "四"); //将显示的格式化字符串放到buffer
    vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[5].buffer[0], "五"); //将显示的格式化字符串放到buffer
    vspf(&oled->stru_TimeSetWindowParam.Display_week_string_buffer[6].buffer[0], "六"); //将显示的格式化字符串放到buffer
}

void OledUserParamInitial(OLEDDisplay_t *oled)
{
    oled->enum_DisplayWindow = Main_Window;
    // oled->Flag_LightSetSync = 0;
    oled->Point_ReadEepromBuffer = (uint8_t *)pvPortMalloc(50 * sizeof(uint8_t));
    if (!oled->Point_ReadEepromBuffer) //如果没有申请成功
    {
        ESP_LOGI(TAG, "Point_ReadEepromBuffer malloc 申请内存失败");
    }
    gpio_set_level(HEATING_BASEBALL_GPIO_PIN, HEATING_BASEBALL_OFF);
    gpio_set_level(FILTER_DRIVE_GPIO_PIN, FILTER_DRIVE_OFF);
    BinSemap_LightSetSync = xSemaphoreCreateBinary();
    BinSemap_SetSync = xSemaphoreCreateBinary();
    BinSemap_CloudOrderSync = xSemaphoreCreateBinary();
    BinSemap_DemoExecutSync = xSemaphoreCreateBinary();
    BinSemap_FullScreenUpdate = xSemaphoreCreateBinary();            //二值信号量 用于显示更新 全屏更新
    BinSemap_PartUpdate_1 = xSemaphoreCreateBinary();                //二值信号量 用于显示更新 部分1更新
    BinSemap_PartUpdate_2 = xSemaphoreCreateBinary();                //二值信号量 用于显示更新 部分2更新
    BinSemap_PartUpdate_3 = xSemaphoreCreateBinary();                //二值信号量 用于显示更新 部分3更新
    SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //上电初始化整屏刷新一次
    LedcSteerGearPwmSetDuty(STEER_GEAR_ANGLE_180);                   //设置舵机的pwm占空比
}

static void i2c_oled_task(void *arg)
{
    rotenc_event_t event = {0};
    esp_timer_handle_t timer1;
    EEPROM_t eeprom_dev;
    DS1302_Dev DS1302_Dev;
    type_stru_DS1302_DateTime DS1302_DateTime;
    float ReadTemp;
    type_stru_KeyValue stru_KeyValue = {0};
    uint16_t ReadLuxAdcVal = 0;
    static const adc_channel_t t_Lux_channel = ADC_CHANNEL_5; // GPIO33 if ADC1
    static const adc_bits_width_t t_Lux_width = ADC_WIDTH_BIT_9;
    static const adc_atten_t t_Lux_atten = ADC_ATTEN_DB_11;

    adc1_config_width(t_Lux_width); // ADC初始化
    adc1_config_channel_atten(t_Lux_channel, t_Lux_atten);

    LedcPwmInit();

    if (!DS1302_begin(&DS1302_Dev, CONFIG_CLK_GPIO, CONFIG_IO_GPIO, CONFIG_CE_GPIO))
    {
        ESP_LOGE(TAG, "Error: DS1302 begin");
    }
    // 模块载入
    ds18b20_init(DS18B20_PIN);

    gpio_reset_pin(HEATING_BASEBALL_GPIO_PIN);
    gpio_set_direction(HEATING_BASEBALL_GPIO_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(FILTER_DRIVE_GPIO_PIN);
    gpio_set_direction(FILTER_DRIVE_GPIO_PIN, GPIO_MODE_INPUT_OUTPUT);
    gpio_reset_pin(INDICATOR_MQTT_GPIO_PIN);
    gpio_set_direction(INDICATOR_MQTT_GPIO_PIN, GPIO_MODE_OUTPUT);

    // xQueue_DisplayLine_1 = xQueueCreate(1,                                    /* 消息个数 */
    //                                     sizeof(typedef_stru_DisplayLine));    /* 每个消息大小，单位字节 */
    // xQueue_DisplayLine_2 = xQueueCreate(1,                                    /* 消息个数 */
    //                                     sizeof(typedef_stru_DisplayLine));    /* 每个消息大小，单位字节 */
    // xQueue_DisplayLine_3 = xQueueCreate(1,                                    /* 消息个数 */
    //                                     sizeof(typedef_stru_DisplayLine));    /* 每个消息大小，单位字节 */
    // xQueue_DisplayLine_4 = xQueueCreate(1,                                    /* 消息个数 */
    //                                     sizeof(typedef_stru_DisplayLine));    /* 每个消息大小，单位字节 */
    xQueue_ReadTemp = xQueueCreate(1,                                         /* 消息个数 */
                                   sizeof(float));                            /* 每个消息大小，单位字节 */
    xQueue_TemperTrigParam = xQueueCreate(1,                                  /* 消息个数 */
                                          sizeof(type_stru_TemperTrigParam)); /* 每个消息大小，单位字节 */
    xQueue_CurrentTime = xQueueCreate(1,                                      /* 消息个数 */
                                      sizeof(type_stru_DS1302_DateTime));     /* 每个消息大小，单位字节 */

    xQueue_FeedTrigParam = xQueueCreate(1,                                              /* 消息个数 */
                                        sizeof(type_stru_FeedTrigParam));               /* 每个消息大小，单位字节 */
    xQueue_LightTrigParam = xQueueCreate(1,                                             /* 消息个数 */
                                         sizeof(type_stru_LightTrigParam));             /* 每个消息大小，单位字节 */
    xQueue_LuxParam = xQueueCreate(1,                                                   /* 消息个数 */
                                   sizeof(uint16_t));                                   /* 每个消息大小，单位字节 */
    xqueue_SystemStatusParam = xQueueCreate(1,                                          /* 消息个数 */
                                            sizeof(type_stru_SystemStatusWindowParam)); /* 每个消息大小，单位字节 */
    xqueue_WifiConfigParam = xQueueCreate(1,                                            /* 消息个数 */
                                          sizeof(wifi_config_t));                       /* 每个消息大小，单位字节 */

    i2c_master_driver_initialize(&eeprom_dev, EEPROM_SIZE, I2C_MASTER_NUM, CONFIG_24C256_I2C_ADDRESS, CONFIG_24C256_SDA_GPIO, CONFIG_24C256_SCL_GPIO);
    // Verify that the GPIO ISR service is installed, before initializing the driver.
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    // Initialize the handle instance of the rotary device,
    // by default it uses 1 mS for the debounce time.
    rotenc_handle_t handle = {0};
    ESP_ERROR_CHECK(rotenc_init(&handle, // 编码器初始化
                                CONFIG_ROT_ENC_CLK_GPIO,
                                CONFIG_ROT_ENC_DTA_GPIO,
                                CONFIG_ROT_ENC_DEBOUNCE));

    ESP_ERROR_CHECK(rotenc_init_button(&handle,
                                       CONFIG_ROT_ENC_BUTTON_GPIO,
                                       CONFIG_ROT_ENC_BUTTON_DEBOUNCE,
                                       button_callback));
    ESP_LOGI(TAG, "Report mode by freertos queue");
    ESP_ERROR_CHECK(rotenc_set_event_queue(&handle, 10));

    OLEDDisplay_t *oled = OLEDDisplay_init(I2C_MASTER_NUM, 0x78, OLED_I2C_MASTER_SDA, OLED_I2C_MASTER_SCL, I2C_MASTER_FREQ_HZ); // oled屏幕初始化
    OLEDDisplay_setBrightness(oled, (uint8_t)(255 * 0.3));                                                                      // 亮度
    OLEDDisplay_flipScreenVertically(oled);                                                                                     //翻转屏幕
    OLEDDisplay_setTextAlignment(oled, TEXT_ALIGN_LEFT);                                                                        //对齐方式

    SubOptionStringExtractPro(oled);
    OledUserParamInitial(oled);
    counter_Initial(oled);
    // ReadEepromWhenReset(oled, &eeprom_dev);
    oled->Point_EepromDev = &eeprom_dev;
    // oled->stru_TimeSetWindowParam.Point_OledUpdateDisplayTime = &DS1302_DateTime;
    oled->stru_TimeSetWindowParam.Point_OledDs1302dev = &DS1302_Dev;

    esp_timer_init(); //软件定时器开启
    esp_timer_create_args_t args = {
        .callback = timer1Interrupt,
        .arg = (void *)oled, //将oled结构体传入定时器扫描显示
        .dispatch_method = ESP_TIMER_TASK,
        .name = "Tiemr1",
        .skip_unhandled_events = false,
    };
    esp_timer_create(&args, &timer1);
    esp_timer_start_periodic(timer1, 10e3); //定时器周期时间 us

    while (1)
    {
        if (get_counter(oled, display_refresh_timer) == 0) //定时处理
        {
            register_counter(oled, display_refresh_timer, 1 * COUNT_10_MS);
            Get_KEY_Value(&handle, &stru_KeyValue); //读编码器键值
            GUI_Refresh(oled, &stru_KeyValue);      //执行菜单显示处理
            IndicatorMqttPro(oled);                 // Mqtt连接状态指示灯处理函数

            ReadAdcAveValPro(t_Lux_channel, &ReadLuxAdcVal); //读ADC值
            // ReadLuxAdcVal = 255 - ReadLuxAdcVal; //取反 这样环境光越强则值越大 更直观
            // printf("ADC Raw: %d\t\n", ReadLuxAdcVal);

            if (get_counter(oled, main_window_display_time_timer) == 0) //定时刷新显示
            {
                register_counter(oled, main_window_display_time_timer, 1 * COUNT_1_S);
                ActionTrigPro(oled); //动作触发处理

                xQueueOverwrite(xQueue_LuxParam, &ReadLuxAdcVal); //当前Lux参数传递到队列

                if (!DS1302_getDateTime(&DS1302_Dev, &DS1302_DateTime)) //读取ds1302失败
                {
                    ESP_LOGE(pcTaskGetTaskName(0), "Error: DS1302 read failed");
                    static type_stru_DS1302_DateTime DateTimeSetDefualt = {
                        //读ds1302出错 给赋值一个初始时间
                        .second = 22,  //!< Second 0..59
                        .minute = 12,  //!< Minute 0..59
                        .hour = 12,    //!< Hour 0..23
                        .dayWeek = 2,  //!< Day of the week (0 as Sunday/ 1 as Monday)
                        .dayMonth = 2, //!< Day of the month 1..31
                        .month = 2,    //!< Month 1..12
                        .year = 2022,  //!< Year 2000..2099
                    };
                    DS1302_setDateTime(&DS1302_Dev, &DateTimeSetDefualt);
                }
                else
                {
                    xQueueOverwrite(xQueue_CurrentTime, &DS1302_DateTime); //当前时间传递到队列 用于显示或判断
                }

                ReadTemp = ds18b20_get_temp(); //每秒读一次温度
                xQueueOverwrite(xQueue_ReadTemp, &ReadTemp);
            }
        }
    }

    vTaskDelete(NULL);
}

/* Callback to handle commands received from the QCloud cloud */
static esp_err_t light_get_param(const char *id, esp_qcloud_param_val_t *val)
{
    if (!strcmp(id, "FeedTime"))
    {
        // val->b = light_driver_get_switch();
        val->i = CloudGetFeedTime();
    }
    else if (!strcmp(id, "brightness"))
    {
        val->i = CloudGetLightBright();
    }
    // else if (!strcmp(id, "hue"))
    // {
    //     val->i = light_driver_get_hue();
    // }
    // else if (!strcmp(id, "saturation"))
    // {
    //     val->i = light_driver_get_saturation();
    // }

    ESP_LOGI(TAG, "Report id: %s, val: %d", id, val->i);

    return ESP_OK;
}

/* Callback to handle commands received from the QCloud cloud */
static esp_err_t light_set_param(const char *id, const esp_qcloud_param_val_t *val)
{
    esp_err_t err = ESP_OK;
    ESP_LOGI(TAG, "Received id: %s, val: %d", id, val->i);
    if (!strcmp(id, "FeedByManualSwitch"))
    {
        // err = light_driver_set_switch(val->b);
        CloudFeedByManualSwitchPro(val->b);
    }
    else if (!strcmp(id, "FeedTime"))
    {
        CloudFeedByManualTimePro(val->i);
    }
    else if (!strcmp(id, "LightByManualSwitch"))
    {
        CloudLightByManualSwitchPro(val->b);
    }
    else if (!strcmp(id, "brightness"))
    {
        CloudLightByManualBrightPro(val->i);
    }
    // else if (!strcmp(id, "hue"))
    // {
    // err = light_driver_set_hue(val->i);
    // }
    // else if (!strcmp(id, "saturation"))
    // {
    //     err = light_driver_set_saturation(val->i);
    // }
    else
    {
        ESP_LOGW(TAG, "light_set_param ,This parameter is not supported");
    }

    return err;
}

/* Event handler for catching QCloud events */
static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case QCLOUD_EVENT_IOTHUB_INIT_DONE:
        esp_qcloud_iothub_report_device_info();
        ESP_LOGI(TAG, "QCloud Initialised");
        break;

    case QCLOUD_EVENT_IOTHUB_BOND_DEVICE:
        ESP_LOGI(TAG, "Device binding successful");
        break;

    case QCLOUD_EVENT_MQTT_CONNECTED:
        stru_DistributNetWindowParam.MqttNetConnect = 1;
        ESP_LOGI(TAG, "event_handler QCLOUD_EVENT_MQTT_CONNECTED MqttNetConnect = 1;");
        break;

    case QCLOUD_EVENT_MQTT_DISCONNECTED:
        stru_DistributNetWindowParam.MqttNetConnect = 0;
        ESP_LOGI(TAG, "event_handler QCLOUD_EVENT_MQTT_DISCONNECTED MqttNetConnect = 0;");
        break;

    case QCLOUD_EVENT_IOTHUB_UNBOND_DEVICE:
        ESP_LOGW(TAG, "Device unbound with iothub");
        esp_qcloud_wifi_reset();
        esp_restart();
        break;

    case QCLOUD_EVENT_IOTHUB_BIND_EXCEPTION:
        ESP_LOGW(TAG, "Device bind fail");
        esp_qcloud_wifi_reset();
        esp_restart();
        break;

    case QCLOUD_EVENT_IOTHUB_RECEIVE_STATUS:
        ESP_LOGI(TAG, "receive status message: %s", (char *)event_data);
        break;

    default:
        ESP_LOGW(TAG, "Unhandled QCloud Event: %d", event_id);
    }
}

static esp_err_t get_wifi_config(wifi_config_t *wifi_cfg, uint32_t wait_ms)
{
    ESP_QCLOUD_PARAM_CHECK(wifi_cfg);

    if (esp_qcloud_storage_get("wifi_config", wifi_cfg, sizeof(wifi_config_t)) == ESP_OK)
    {

#ifdef CONFIG_BT_ENABLE
        esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
#endif

        return ESP_OK;
    }

    /**< Reset wifi and restart wifi */
    esp_wifi_restore();
    esp_wifi_start();

    /**< The yellow light flashes to indicate that the device enters the state of configuring the network */
    // light_driver_breath_start(128, 128, 0); /**< yellow blink */

    /**< Note: Smartconfig and softapconfig working at the same time will affect the configure network performance */

#ifdef CONFIG_LIGHT_PROVISIONING_SOFTAPCONFIG
    char softap_ssid[32 + 1] = CONFIG_LIGHT_PROVISIONING_SOFTAPCONFIG_SSID;
    // uint8_t mac[6] = {0};
    // ESP_ERROR_CHECK(esp_wifi_get_mac(WIFI_IF_STA, mac));
    // sprintf(softap_ssid, "tcloud_%s_%02x%02x", light_driver_get_type(), mac[4], mac[5]);

    esp_qcloud_prov_softapconfig_start(SOFTAPCONFIG_TYPE_ESPRESSIF_TENCENT,
                                       softap_ssid,
                                       CONFIG_LIGHT_PROVISIONING_SOFTAPCONFIG_PASSWORD);
    esp_qcloud_prov_print_wechat_qr(softap_ssid, "softap");
#endif

#ifdef CONFIG_LIGHT_PROVISIONING_SMARTCONFIG
    esp_qcloud_prov_smartconfig_start(SC_TYPE_ESPTOUCH_AIRKISS);
#endif

#ifdef CONFIG_LIGHT_PROVISIONING_BLECONFIG
    char local_name[32 + 1] = CONFIG_LIGHT_PROVISIONING_BLECONFIG_NAME;
    esp_qcloud_prov_bleconfig_start(BLECONFIG_TYPE_ESPRESSIF_TENCENT, local_name);
#endif

    ESP_ERROR_CHECK(esp_qcloud_prov_wait(wifi_cfg, wait_ms));

#ifdef CONFIG_LIGHT_PROVISIONING_SMARTCONFIG
    esp_qcloud_prov_smartconfig_stop();
#endif

#ifdef CONFIG_LIGHT_PROVISIONING_SOFTAPCONFIG
    esp_qcloud_prov_softapconfig_stop();
#endif

    /**< Store the configure of the device */
    esp_qcloud_storage_set("wifi_config", wifi_cfg, sizeof(wifi_config_t));

    /**< Configure the network successfully to stop the light flashing */
    light_driver_breath_stop(); /**< stop blink */

    return ESP_OK;
}

//回调函数
static void state_task(void *pvParameters)
{
    static char InfoBuffer[512] = {0};
    while (1)
    {
        vTaskList((char *)&InfoBuffer);
        printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
        printf("\r\n%s\r\n", InfoBuffer);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    //首先创建一个任务单独来做占用情况这件事情
    // xTaskCreate(state_task, "state_task", 4096, NULL, 6, NULL);

    stru_CloudFeedByManualParam.FeedTrigDuration = 10;     //默认手动喂食时间为10s
    stru_CloudLightByManualParam.LightByManualBright = 10; //手动开灯亮度默认为10%

    // ESP_LOGI(TAG, "Running");
    xTaskCreate(i2c_oled_task, "i2c_oled_task_0", 1024 * 4, (void *)0, 10, NULL);

    /**
     * @brief Add debug function, you can use serial command and remote debugging.
     */
    esp_qcloud_log_config_t log_config = {
        .log_level_uart = ESP_LOG_INFO,
    };
    ESP_ERROR_CHECK(esp_qcloud_log_init(&log_config));
    /**
     * @brief Set log level
     * @note  This function can not raise log level above the level set using
     * CONFIG_LOG_DEFAULT_LEVEL setting in menuconfig.
     */
    esp_log_level_set("*", ESP_LOG_VERBOSE); //传入"*" 所有log等级均提升至 ESP_LOG_VERBOSE "Bigger chunks of debugging information,"

#ifdef CONFIG_LIGHT_DEBUG
    ESP_ERROR_CHECK(esp_qcloud_console_init());
    esp_qcloud_print_system_info(10000);
#endif /**< CONFIG_LIGHT_DEBUG */

    /**
     * @brief Initialize Application specific hardware drivers and set initial state.
     */
    light_driver_config_t driver_cfg = COFNIG_LIGHT_TYPE_DEFAULT();
    ESP_ERROR_CHECK(light_driver_init(&driver_cfg));

    /**< Continuous power off and restart more than five times to reset the device */
    if (esp_qcloud_reboot_unbroken_count() >= CONFIG_LIGHT_REBOOT_UNBROKEN_COUNT_RESET)
    {
        ESP_LOGW(TAG, "Erase information saved in flash");
        esp_qcloud_storage_erase(CONFIG_QCLOUD_NVS_NAMESPACE);
    }
    else if (esp_qcloud_reboot_is_exception(false))
    {
        ESP_LOGE(TAG, "The device has been restarted abnormally");
        // #ifdef CONFIG_LIGHT_DEBUG
        //         light_driver_breath_start(255, 0, 0); /**< red blink */
        // #else
        //         ESP_ERROR_CHECK(light_driver_set_switch(true));
        // #endif /**< CONFIG_LIGHT_DEBUG */
    }
    else
    {
        // ESP_ERROR_CHECK(light_driver_set_switch(true));
    }

    /*
     * @breif Create a device through the server and obtain configuration parameters
     *        server: https://console.cloud.tencent.com/iotexplorer
     */
    /**< Create and configure device authentication information */
    ESP_ERROR_CHECK(esp_qcloud_create_device());
    /**< Configure the version of the device, and use this information to determine whether to OTA */
    ESP_ERROR_CHECK(esp_qcloud_device_add_fw_version("0.0.1"));
    /**< Register the properties of the device */
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("power_switch", QCLOUD_VAL_TYPE_BOOLEAN));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("hue", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("saturation", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("value", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("power_switch", QCLOUD_VAL_TYPE_BOOLEAN));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("windspeed", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("mode", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("timer", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("swing", QCLOUD_VAL_TYPE_BOOLEAN));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("cold", QCLOUD_VAL_TYPE_BOOLEAN));
    ESP_ERROR_CHECK(esp_qcloud_device_add_property("LightByManualSwitch", QCLOUD_VAL_TYPE_BOOLEAN));
    ESP_ERROR_CHECK(esp_qcloud_device_add_property("FeedByManualSwitch", QCLOUD_VAL_TYPE_BOOLEAN));
    ESP_ERROR_CHECK(esp_qcloud_device_add_property("FeedTime", QCLOUD_VAL_TYPE_INTEGER));
    ESP_ERROR_CHECK(esp_qcloud_device_add_property("brightness", QCLOUD_VAL_TYPE_INTEGER));

    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("mode", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("timer", QCLOUD_VAL_TYPE_INTEGER));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("swing", QCLOUD_VAL_TYPE_BOOLEAN));
    // ESP_ERROR_CHECK(esp_qcloud_device_add_property("cold", QCLOUD_VAL_TYPE_BOOLEAN));
    /**< The processing function of the communication between the device and the server */
    ESP_ERROR_CHECK(esp_qcloud_device_add_property_cb(light_get_param, light_set_param));

    /**
     * @brief Initialize Wi-Fi.
     */
    ESP_ERROR_CHECK(esp_qcloud_wifi_init());
    ESP_ERROR_CHECK(esp_event_handler_register(QCLOUD_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    /**
     * @brief Get the router configuration
     */
    wifi_config_t wifi_cfg = {0};
    ESP_ERROR_CHECK(get_wifi_config(&wifi_cfg, portMAX_DELAY));
    xQueueOverwrite(xqueue_WifiConfigParam, &wifi_cfg); //将参数传递到配网状态窗口显示

    /**
     * @brief Connect to router
     */
    ESP_ERROR_CHECK(esp_qcloud_wifi_start(&wifi_cfg));
    ESP_ERROR_CHECK(esp_qcloud_timesync_start());
    ESP_ERROR_CHECK(esp_qcloud_timesync_wait(portMAX_DELAY));
    // esp_qcloud_timesync_wait(TickType_t ticks_to_wait)
    /**
     * @brief Connect to Tencent Cloud Iothub
     */
    ESP_ERROR_CHECK(esp_qcloud_iothub_init());
    ESP_ERROR_CHECK(esp_qcloud_iothub_start());
    ESP_ERROR_CHECK(esp_qcloud_iothub_ota_enable());
}
