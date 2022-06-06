#include "app_function.h"

static void (*current_operation_func)(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue); //定义一个函数指针
static char *TAG = "app_function";

//索引表
type_stru_Menu stru_MenuTable[] =
    {
        // Cur_Index,previous,next,enter,back,(*current_operation)(u8,u8)
        //主窗口
        {Main_Window, Undefined_window, Undefined_window, Undefined_window, MainWindowPro},
        //设置窗口
        {Set_Window, Undefined_window, Undefined_window, Undefined_window, SetWindowPro},
        //
        {time_set_window, Undefined_window, Undefined_window, Undefined_window, TimeSetWindowPro},                   //
        {time_set_by_manual_window, Undefined_window, Undefined_window, Undefined_window, TimeSetByManualWindowPro}, //
        {time_set_by_net_window, Undefined_window, Undefined_window, Undefined_window, TimeSetByNetWindowPro},       //

        {water_temper_set_window, Undefined_window, Undefined_window, Undefined_window, WaterTemperSetWindowPro},  //
        {time_detail_window, Undefined_window, Undefined_window, Undefined_window, TimeDetailWindowPro},           //
        {temper_trig_window, Undefined_window, Undefined_window, Undefined_window, TemperTrigWindowPro},           //
        {temper_detail_window, Undefined_window, Undefined_window, Undefined_window, WaterTemperSetWindowPro},     //
        {feed_time_set_window, Undefined_window, Undefined_window, Undefined_window, FeedTimeSetWindowPro},        //
        {feed_trig_window, Undefined_window, Undefined_window, Undefined_window, FeedTrigWindowPro},               //
        {light_set_window, Undefined_window, Undefined_window, Undefined_window, LightSetWindowPro},               //
        {light_set_by_time_window, Undefined_window, Undefined_window, Undefined_window, LightSetByTimeWindowPro}, //
        {light_set_by_lux_window, Undefined_window, Undefined_window, Undefined_window, LightSetByLuxWindowPro},   //

        {light_trig_window, Undefined_window, Undefined_window, Undefined_window, LightTrigWindowPro},                 //
        {system_status_window, system_status_window_2, system_status_window_2, Set_Window, SystemStatusWindowPro},     //
        {system_status_window_2, system_status_window_3, system_status_window_3, Set_Window, SystemStatusWindow_2Pro}, //
        {system_status_window_3, system_status_window_4, system_status_window_4, Set_Window, SystemStatusWindow_3Pro}, //
        {system_status_window_4, system_status_window, system_status_window, Set_Window, SystemStatusWindow_4Pro},     //
        {filter_set_window, Undefined_window, Undefined_window, Undefined_window, FilterSetWindowPro},                 //
        {demonstrat_window, Undefined_window, Undefined_window, Undefined_window, DemonstratWindowPro},                //
        {demonstrat_window_2, Undefined_window, Undefined_window, Undefined_window, DemonstratWindow_2Pro},            //
        {demonstrat_window_3, Undefined_window, Undefined_window, Undefined_window, DemonstratWindow_3Pro},            //
        {demonstrat_window_4, Undefined_window, Undefined_window, Undefined_window, DemonstratWindow_4Pro},            //
        {demonstrat_window_5, Undefined_window, Undefined_window, Undefined_window, DemonstratWindow_5Pro},            //

        {distribut_network_window, distribut_network_window_2, distribut_network_window_2, Main_Window, DistributNetworkWindowPro}, //
        {distribut_network_window_2, Undefined_window, distribut_network_window, Undefined_window, DistributNetworkWindow_2Pro},    //
};

void encoder_event_callback(rotenc_event_t event)
{
    ESP_LOGI(TAG, "Event: position %d, direction %s", event.position,
             event.direction ? (event.direction == ROTENC_CW ? "CW++" : "CCW--") : "NOT_SET");
}

void Get_KEY_Value(rotenc_handle_t *handle, type_stru_KeyValue *stru_KeyValue)
{
    rotenc_event_t event = {0};
    if (xQueueReceive(handle->q_event.queue, &event, 3) == pdTRUE)
    {
        encoder_event_callback(event);
        if (event.ButtonPress == short_Press)
        {
            stru_KeyValue->enum_KeyPress = key_short_press;
            // return key_short_press;
        }
        else if (event.ButtonPress == long_Press)
        {
            stru_KeyValue->enum_KeyPress = key_long_press;
            // return key_long_press;
        }
        else if (event.direction == ROTENC_CW)
        {
            stru_KeyValue->enum_Rotation = key_up_right;
            // return key_up_right;
        }
        else if (event.direction == ROTENC_CCW)
        {
            stru_KeyValue->enum_Rotation = key_down_left;
            // return key_down_left;
        }
    }
    // return key_not_rotat;
}

/*
函数功能：刷新界面
参数：无
返回值：无
*/
void GUI_Refresh(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue)
{
    static typedef_enum_DisplayWindow last_index = Undefined_window;
    bool Flag_GotTableOperation = 0;
    static typedef_enum_DisplayWindow current_table_index = Main_Window;
    u8 i;

    for (i = 0; i < (sizeof(stru_MenuTable) / sizeof(type_stru_Menu)); i++)
    { //查结构体
        if (oled->enum_DisplayWindow == stru_MenuTable[i].Cur_Index)
        {
            current_operation_func = stru_MenuTable[i].current_operation;
            current_table_index = i;
            Flag_GotTableOperation = 1;
            break;
        }
    }

    if (Flag_GotTableOperation == 1)
    {
        Flag_GotTableOperation = 0;
        if (stru_KeyValue->enum_Rotation != key_not_rotat || stru_KeyValue->enum_KeyPress != key_not_press) //只有按键按下才刷屏
        {
            if (stru_KeyValue->enum_KeyPress == key_short_press)
            {
                if (stru_MenuTable[current_table_index].enter != Undefined_window)
                {
                    stru_KeyValue->enum_KeyPress = key_not_press;
                    oled->enum_DisplayWindow = stru_MenuTable[current_table_index].enter; //更新索引值
                }
            }
            else
            {
                switch (stru_KeyValue->enum_Rotation)
                {
                case key_up_right:
                    if (stru_MenuTable[current_table_index].up != Undefined_window)
                    {
                        oled->enum_DisplayWindow = stru_MenuTable[current_table_index].up; //更新索引值
                        stru_KeyValue->enum_Rotation = key_not_rotat;
                    }
                    break;
                case key_down_left:
                    if (stru_MenuTable[current_table_index].down != Undefined_window)
                    {
                        oled->enum_DisplayWindow = stru_MenuTable[current_table_index].down; //更新索引值
                        stru_KeyValue->enum_Rotation = key_not_rotat;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        if (last_index != oled->enum_DisplayWindow)
        {
            oled->DisplayStep = 0;
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //全屏更新显示
            for (i = 0; i < (sizeof(stru_MenuTable) / sizeof(type_stru_Menu)); i++)
            { //查结构体
                if (oled->enum_DisplayWindow == stru_MenuTable[i].Cur_Index)
                {
                    current_operation_func = stru_MenuTable[i].current_operation;
                    current_table_index = i;
                    break;
                }
            }
            last_index = oled->enum_DisplayWindow; //更新上一界面索引值
        }
    }
    else //如果没有查到对应的结构体函数则返回主界面
    {
        SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //全屏更新显示
        oled->enum_DisplayWindow = Main_Window;
        current_table_index = Main_Window;
        current_operation_func = stru_MenuTable[Main_Window].current_operation;
    }

    (*current_operation_func)(oled, stru_KeyValue); //执行当前索引对应的函数
}

void ActionTrigPro(OLEDDisplay_t *oled) //触发条件判断处理
{
    static type_stru_UpperLowerLimitTemperParam stru_UpperLowerLimitTemperParam = {0};
    static type_stru_TemperTrigParam stru_TemperTrigParam = {0};
    static typedef_stru_SetFeedTimeTable stru_SetFeedTimeTable = {0};
    type_stru_DS1302_DateTime stru_GetCurrentTime = {0}; //获取当前时间
    static type_stru_FeedTrigParam stru_FeedTrigParam = {0};
    static typedef_stru_LightSet stru_LightSet = {0};
    static type_stru_LightTrigParam stru_LightTrigParam = {0};
    static typedef_enum_LightSetIntervalIndex LastLightTrigIntervalIndex = light_Interval_index_sum; //记住上次触发的索引
    static type_stru_SystemStatusWindowParam stru_SystemStatusWindowParam = {0};
    static type_stru_FilterSet stru_FilterSet = {0};

    static uint8_t ActionTrigStep = 0;
    static bool Flag_ReadTemperParamSuccess = 0;
    static bool Flag_ReadFeedTimeParamSuccess = 0;
    static bool Flag_ReadLightParamSuccess = 0;
    static bool Flag_ReadFilterParamSuccess = 0;
    static bool Flag_CloudLightByManual = 0;
    bool Flag_FindLightTrigTimeSucc = 0;
    float ReadTemp; //获取当前温度
    static uint8_t Filter_0 = 0;
    static uint8_t Filter_1 = 0;
    static uint8_t Filter_2 = 0;
    static uint8_t Filter_3 = 0;
    static uint8_t Filter_4 = 0;
    static uint8_t Filter_5 = 0;

    bool Flag_FindToBeFeedTimeSuccess = 0;

    uint16_t CurrentTimeMergerTemp = 0; //将时间和并成千位数方便判断 比如 22:12 合并 2212
    uint16_t BeginTimeMergerTemp = 0;
    uint16_t EndTimeMergerTemp = 0;
    uint16_t CompareTimeTemp = 0;     //用于代替当前时间进行比较
    uint8_t FindToBeTrigTimeStep = 0; //找下一次触发的时间组 分步骤处理

    uint8_t CurrentLuxAdcValTemp = 0; //当前LuxADC值临时变量
    uint8_t LowerLuxAdcValTemp = 0;
    uint8_t UpperLuxAdcValTemp = 0;

    uint16_t ReadLuxAdcVal = 0;
    if (ActionTrigStep == 0 || xSemaphoreTake(BinSemap_SetSync, (TickType_t)10) == pdTRUE) //初始化 执行一次 或 设置改变时同步一次
    {
        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, temper_addr, sizeof(type_stru_UpperLowerLimitTemperParam)) == DATA_VERIFICATION)
        {
            stru_UpperLowerLimitTemperParam = *(type_stru_UpperLowerLimitTemperParam *)oled->Point_ReadEepromBuffer;
            stru_SystemStatusWindowParam.WaterTemperSwitch = stru_UpperLowerLimitTemperParam.Switch;
            Flag_ReadTemperParamSuccess = 1;
        }
        else
        {
            Flag_ReadTemperParamSuccess = 0;
            ESP_LOGI(TAG, "ActionTrigPro  读温度参数失败");
        }
        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, feed_time_addr, sizeof(typedef_stru_SetFeedTimeTable)) == DATA_VERIFICATION)
        {
            stru_SetFeedTimeTable = *(typedef_stru_SetFeedTimeTable *)oled->Point_ReadEepromBuffer;
            stru_SystemStatusWindowParam.FeedSwitch = stru_SetFeedTimeTable.FeedSwitch;
            Flag_ReadFeedTimeParamSuccess = 1;
        }
        else
        {
            Flag_ReadFeedTimeParamSuccess = 0;
            ESP_LOGI(TAG, "ActionTrigPro()  读喂食时间参数失败");
        }

        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, light_addr, sizeof(typedef_stru_LightSet)) == DATA_VERIFICATION)
        {
            stru_LightSet = *(typedef_stru_LightSet *)oled->Point_ReadEepromBuffer;
            stru_LightTrigParam.LightTrigModeIndex = stru_LightSet.enum_LightSetModeIndex; //记住触发模式
            stru_SystemStatusWindowParam.LightSwitch = stru_LightSet.LightSwitch;
            Flag_ReadLightParamSuccess = 1;
        }
        else
        {
            Flag_ReadLightParamSuccess = 0;
            ESP_LOGI(TAG, "ActionTrigPro()  读灯光参数失败");
        }
        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, filter_addr, sizeof(type_stru_FilterSet)) == DATA_VERIFICATION)
        {
            stru_FilterSet = *(type_stru_FilterSet *)oled->Point_ReadEepromBuffer;
            Flag_ReadFilterParamSuccess = 1;
        }
        else
        {
            Flag_ReadFilterParamSuccess = 0;
            ESP_LOGI(TAG, "ActionTrigPro()  读过滤参数失败");
        }
        xQueueOverwrite(xqueue_SystemStatusParam, &stru_SystemStatusWindowParam); //将参数传递到系统状态窗口显示
        ActionTrigStep++;
    }
    // ESP_LOGI(TAG, "STEER_GEAR_MAX_LOAD_VALUE = %d", STEER_GEAR_MAX_LOAD_VALUE);
    // ESP_LOGI(TAG, "STEER_GEAR_ANGLE_0 = %d", (uint32_t)STEER_GEAR_ANGLE_0);
    // ESP_LOGI(TAG, "STEER_GEAR_ANGLE_0 = %f", STEER_GEAR_ANGLE_0);
    // ESP_LOGI(TAG, "STEER_GEAR_ANGLE_180 = %d", STEER_GEAR_ANGLE_180);
    // ESP_LOGI(TAG, "stru_CloudLightByManualParam.LightByManualSwitch = %d", stru_CloudLightByManualParam.LightByManualSwitch);
    // ESP_LOGI(TAG, "stru_CloudLightByManualParam.LightByManualBright = %d", stru_CloudLightByManualParam.LightByManualBright);
    // ESP_LOGI(TAG, "stru_DistributNetWindowParam.MqttNetConnect = %d", stru_DistributNetWindowParam.MqttNetConnect);

    if (xSemaphoreTake(BinSemap_CloudOrderSync, (TickType_t)10) == pdTRUE) //如果云端指令下发同步一次
    {
        if (enum_CloudOrder == cloud_order_feed)
        {
            xQueueOverwrite(xQueue_FeedTrigParam, &stru_CloudFeedByManualParam); //发送显示参数到喂食触发显示窗口
            oled->enum_DisplayWindow = feed_trig_window;
        }
        else if (enum_CloudOrder == cloud_order_light)
        {
            if (stru_CloudLightByManualParam.LightByManualSwitch == 1)
            {
                Flag_CloudLightByManual = 1;
                LedcLightPwmSetDuty((uint32_t)((8190 * stru_CloudLightByManualParam.LightByManualBright) / 100)); //按照设置的pwm百分比输出pwm
            }
            else
            {
                Flag_CloudLightByManual = 0;
                LedcLightPwmSetDuty((uint32_t)8190 * (0));           //冗余 功能不使能则关闭pwm
                if (xSemaphoreGive(BinSemap_LightSetSync) == pdTRUE) //手动开灯关闭后  同步触发一次灯光
                {
                    printf("手动灯光关闭 发送灯光触发一次同步信号量\r\n");
                }
            }
        }

        enum_CloudOrder = cloud_order_not_set; //清零,每次下发只处理一次
    }

    if ((oled->enum_DisplayWindow >= temper_trig_window && oled->enum_DisplayWindow < trig_window_sum)) //不在任一触发窗口时才判断
    {
        return;
    }
    if (oled->enum_DisplayWindow == demonstrat_window || (oled->enum_DisplayWindow >= demonstrat_window_2 && oled->enum_DisplayWindow < demonstrat_window_sum)) //不在任一演示窗口时才判断
    {
        return;
    }

    if (xSemaphoreTake(BinSemap_DemoExecutSync, (TickType_t)10) == pdTRUE) //如果演示demo窗口执行过一次 同步触发一次条件符合的动作
    {
        stru_TemperTrigParam.enum_UpperOrLowerTrig = temper_not_trig;
        if (xSemaphoreGive(BinSemap_LightSetSync) == pdTRUE) //同步设置到灯光触发
        {
            // printf("发送灯光设置同步信号量成功\r\n");
        }
    }

    //温度判断触发处理
    if (oled->enum_DisplayWindow != water_temper_set_window)
    {
        if (Flag_ReadTemperParamSuccess == 1)
        {
            // Flag_ReadTemperParamSuccess = 0;
            if (stru_UpperLowerLimitTemperParam.Switch == 1) //只有功能使能开启时才判断温度上下限
            {
                if (xQueuePeek(xQueue_ReadTemp, &ReadTemp, (TickType_t)10))
                { // pcRxedMessage now points to the struct AMessage variable posted
                  // by vATask.
                    stru_TemperTrigParam.UpperVal = stru_UpperLowerLimitTemperParam.upper_limit;
                    stru_TemperTrigParam.LowerVal = stru_UpperLowerLimitTemperParam.lower_limit;
                    if (ReadTemp >= stru_UpperLowerLimitTemperParam.upper_limit && stru_TemperTrigParam.enum_UpperOrLowerTrig != temper_upper_trig)
                    {
                        Filter_0++;
                        Filter_1 = 0;
                        if (Filter_0 >= 10)
                        {
                            Filter_0 = 0;
                            oled->enum_DisplayWindow = temper_trig_window;
                            gpio_set_level(HEATING_BASEBALL_GPIO_PIN, HEATING_BASEBALL_OFF); //温度上限则关闭加热棒
                            stru_TemperTrigParam.enum_UpperOrLowerTrig = temper_upper_trig;
                            xQueueOverwrite(xQueue_TemperTrigParam, &stru_TemperTrigParam);
                        }
                    }
                    else if (ReadTemp <= stru_UpperLowerLimitTemperParam.lower_limit && stru_TemperTrigParam.enum_UpperOrLowerTrig != temper_lower_trig)
                    {
                        Filter_0 = 0;
                        Filter_1++;
                        if (Filter_1 >= 10)
                        {
                            Filter_1 = 0;
                            oled->enum_DisplayWindow = temper_trig_window;
                            gpio_set_level(HEATING_BASEBALL_GPIO_PIN, HEATING_BASEBALL_ON); //温度下限则开启加热棒
                            stru_TemperTrigParam.enum_UpperOrLowerTrig = temper_lower_trig;
                            xQueueOverwrite(xQueue_TemperTrigParam, &stru_TemperTrigParam);
                        }
                    }
                    else
                    {
                        Filter_0 = 0;
                        Filter_1 = 0;
                    }
                }
            }
            else
            {
                gpio_set_level(HEATING_BASEBALL_GPIO_PIN, HEATING_BASEBALL_OFF); //冗余 功能不使能关闭加热棒
            }
        }
    }

    //喂食时间触发判断处理
    if (oled->enum_DisplayWindow != feed_time_set_window)
    { //当不在设置窗口和不在触发窗口时才判断
        if (Flag_ReadFeedTimeParamSuccess == 1)
        {
            if (stru_SetFeedTimeTable.FeedSwitch == 1) //只有功能使能开启时才判断
            {
                if (xQueuePeek(xQueue_CurrentTime, &stru_GetCurrentTime, (TickType_t)10))
                { // pcRxedMessage now points to the struct AMessage variable posted
                    // by vATask.
                    CurrentTimeMergerTemp = stru_GetCurrentTime.hour * 100; //合并时间参数方便判断
                    CurrentTimeMergerTemp += stru_GetCurrentTime.minute;
                    for (uint8_t i = 0; i < 3; i++)
                    {
                        EndTimeMergerTemp = stru_SetFeedTimeTable.stru_SetFeedTime[i].hour * 100; //合并时间参数
                        EndTimeMergerTemp += stru_SetFeedTimeTable.stru_SetFeedTime[i].minute;
                        if (FindToBeTrigTimeStep == 0)
                        {
                            if (CurrentTimeMergerTemp < EndTimeMergerTemp) //找出下次触发的时间组 当前时间小于某一组设置好的喂食时间
                            {
                                FindToBeTrigTimeStep++;
                                CompareTimeTemp = EndTimeMergerTemp; //记住 用于比较时间

                                stru_SystemStatusWindowParam.stru_ToBeTriFeedTime = stru_SetFeedTimeTable.stru_SetFeedTime[i];
                                stru_SystemStatusWindowParam.ToBeFeedTrigIndex = i;                       //记住下次触发的组别
                                xQueueOverwrite(xqueue_SystemStatusParam, &stru_SystemStatusWindowParam); //将参数传递到系统状态窗口显示
                                Flag_FindToBeFeedTimeSuccess = 1;
                                // break; //找到最近触发的组后 跳出循环
                            }
                        }
                        else if (FindToBeTrigTimeStep == 1)
                        {
                            if (CompareTimeTemp > EndTimeMergerTemp) //如果继续循环找出有比上次记录要小的时间(但比当前大) 则采用此时间
                            {
                                CompareTimeTemp = EndTimeMergerTemp; //记住 用于比较时间
                                stru_SystemStatusWindowParam.stru_ToBeTriFeedTime = stru_SetFeedTimeTable.stru_SetFeedTime[i];
                                stru_SystemStatusWindowParam.ToBeFeedTrigIndex = i;                       //记住下次触发的组别
                                xQueueOverwrite(xqueue_SystemStatusParam, &stru_SystemStatusWindowParam); //将参数传递到系统状态窗口显示
                            }
                        }
                    }
                    if (Flag_FindToBeFeedTimeSuccess == 0) //没有找到下次喂食时间组  说明当前时间比所有喂食时间组大 所以下次触发的是最小的时间组
                    {
                        CompareTimeTemp = stru_SetFeedTimeTable.stru_SetFeedTime[0].hour * 100; //合并时间参数 //给比较时间一个初始值
                        CompareTimeTemp += stru_SetFeedTimeTable.stru_SetFeedTime[0].minute;
                        for (uint8_t i = 0; i < 3; i++)
                        {
                            EndTimeMergerTemp = stru_SetFeedTimeTable.stru_SetFeedTime[i].hour * 100; //合并时间参数
                            EndTimeMergerTemp += stru_SetFeedTimeTable.stru_SetFeedTime[i].minute;
                            if (CompareTimeTemp > EndTimeMergerTemp) //找出下次触发的时间组 当前时间小于某一组设置好的喂食时间
                            {
                                stru_SystemStatusWindowParam.ToBeFeedTrigIndex = i; //记住下次触发的组别
                                CompareTimeTemp = EndTimeMergerTemp;
                            }
                        }
                        stru_SystemStatusWindowParam.stru_ToBeTriFeedTime.hour = CompareTimeTemp / 100;
                        stru_SystemStatusWindowParam.stru_ToBeTriFeedTime.minute = CompareTimeTemp % 100;
                        xQueueOverwrite(xqueue_SystemStatusParam, &stru_SystemStatusWindowParam); //将参数传递到系统状态窗口显示
                    }

                    for (uint8_t i = 0; i < 3; i++)
                    {
                        EndTimeMergerTemp = stru_SetFeedTimeTable.stru_SetFeedTime[i].hour * 100; //合并时间参数
                        EndTimeMergerTemp += stru_SetFeedTimeTable.stru_SetFeedTime[i].minute;
                        if (CurrentTimeMergerTemp == EndTimeMergerTemp) //时分判断
                        {
                            if (stru_FeedTrigParam.Flag_Trig == 0)
                            {
                                Filter_2++;
                                if (Filter_2 >= 3)
                                {
                                    Filter_2 = 0;
                                    oled->enum_DisplayWindow = feed_trig_window;
                                    stru_FeedTrigParam.FeedTrigIndex = i;                                        //记住是第几组触发
                                    stru_FeedTrigParam.Flag_Trig = 1;                                            //一分钟内只能触发一次
                                    stru_FeedTrigParam.FeedTrigDuration = stru_SetFeedTimeTable.FeedDuration[i]; //记住是触发的组的喂食时长
                                    xQueueOverwrite(xQueue_FeedTrigParam, &stru_FeedTrigParam);                  //发送显示参数到显示窗口
                                }
                            }
                        }
                        else
                        {
                            if (stru_FeedTrigParam.FeedTrigIndex == i) //上次触发的table组别的时间分钟与当前不一致则清零喂食触发标志位
                            {
                                stru_FeedTrigParam.Flag_Trig = 0; //
                            }
                        }
                    }
                }
            }
            else
            {
                // gpio_set_level(FEED_DRIVE_GPIO_PIN, FEED_DRIVE_OFF); //冗余 功能不使能关闭喂食驱动器
            }
        }
    }

    //灯光触发判断处理
    if (oled->enum_DisplayWindow != light_set_window && (oled->enum_DisplayWindow < light_set_by_time_window || oled->enum_DisplayWindow > light_set_window_sum)) //不在设置窗口时才判断
    {
        if (Flag_ReadLightParamSuccess == 1 && Flag_CloudLightByManual == 0)
        {
            if (stru_LightSet.LightSwitch == 1) //只有功能使能开启时才判断
            {
                if (stru_LightTrigParam.LightTrigModeIndex == mode_index_by_time) //按照时间段
                {
                    if (xQueuePeek(xQueue_CurrentTime, &stru_GetCurrentTime, (TickType_t)10)) //获取当前时间
                    {                                                                         // pcRxedMessage now points to the struct AMessage variable posted
                                                                                              // by vATask.
                        // ESP_LOGI(TAG, "xQueuePeek(xQueue_CurrentTime  获取当前时间成功\n");
                        CurrentTimeMergerTemp = stru_GetCurrentTime.hour * 100; //合并时间参数方便判断
                        CurrentTimeMergerTemp += stru_GetCurrentTime.minute;
                        // ESP_LOGI(TAG, "CurrentTimeMergerTemp =%d \n", CurrentTimeMergerTemp);
                        // ESP_LOGI(TAG, "stru_GetCurrentTime.minute =%d \n", stru_GetCurrentTime.minute);

                        for (uint8_t i = 0; i < light_Interval_index_sum; i++)
                        {
                            if (i == light_interval_index_day) //日间区间判断
                            {
                                BeginTimeMergerTemp = stru_LightSet.stru_LightByTime[i].stru_BeginTime.hour * 100; //合并时间参数
                                BeginTimeMergerTemp += stru_LightSet.stru_LightByTime[i].stru_BeginTime.minute;
                                EndTimeMergerTemp = stru_LightSet.stru_LightByTime[i].stru_EndTime.hour * 100; //合并时间参数
                                EndTimeMergerTemp += stru_LightSet.stru_LightByTime[i].stru_EndTime.minute;
                            }
                            else //傍晚和夜晚 区间判断
                            {
                                BeginTimeMergerTemp = stru_LightSet.stru_LightByTime[i - 1].stru_EndTime.hour * 100; //合并时间参数
                                BeginTimeMergerTemp += stru_LightSet.stru_LightByTime[i - 1].stru_EndTime.minute;
                                EndTimeMergerTemp = stru_LightSet.stru_LightByTime[i].stru_EndTime.hour * 100; //合并时间参数
                                EndTimeMergerTemp += stru_LightSet.stru_LightByTime[i].stru_EndTime.minute;
                            }
                            if (CurrentTimeMergerTemp >= BeginTimeMergerTemp && CurrentTimeMergerTemp < EndTimeMergerTemp) //当前大于开始时间 并且小于结束时间
                            {
                                // ESP_LOGI(TAG, "i == light_interval_index_day 傍晚或夜晚区间判断条件符合\n");
                                Flag_FindLightTrigTimeSucc = 1; //触发某一组则标志位置1
                                Filter_3++;
                                if (Filter_3 >= 3)
                                {
                                    // ESP_LOGI(TAG, "Filter_3 >= 3 傍晚或夜晚判断条件符合\n");
                                    Filter_3 = 0;
                                    stru_LightTrigParam.LightTrigIntervalIndex = i; //记住当前触发区间组
                                    if (LastLightTrigIntervalIndex != stru_LightTrigParam.LightTrigIntervalIndex || xSemaphoreTake(BinSemap_LightSetSync, (TickType_t)10) == pdTRUE)
                                    {
                                        xSemaphoreTake(BinSemap_LightSetSync, (TickType_t)10); //清零标志位
                                        LastLightTrigIntervalIndex = stru_LightTrigParam.LightTrigIntervalIndex;
                                        oled->enum_DisplayWindow = light_trig_window;
                                        stru_LightTrigParam.LightTrigBright = stru_LightSet.stru_LightByTime[i].LightBright; //记住当前触发的组的亮度
                                        xQueueOverwrite(xQueue_LightTrigParam, &stru_LightTrigParam);                        //发送显示参数到显示窗口
                                        LedcLightPwmSetDuty((uint32_t)((8190 * stru_LightTrigParam.LightTrigBright) / 100)); //按照设置的pwm百分比输出pwm
                                    }
                                }
                            }
                        }

                        if (Flag_FindLightTrigTimeSucc == 0)
                        {
                            Filter_5++;
                            if (Filter_5 >= 5)
                            {
                                Filter_5 = 0;
                                LedcLightPwmSetDuty((uint32_t)8190 * (0)); //冗余 没有时间段匹配则关闭pwm
                            }
                        }
                    }
                }
                else //按照lux
                {
                    if (xQueuePeek(xQueue_LuxParam, &ReadLuxAdcVal, (TickType_t)10) == pdTRUE) //获取当前lux
                    {                                                                          // pcRxedMessage now points to the struct AMessage variable posted
                                                                                               // by vATask.
                        // ESP_LOGI(TAG, "xQueuePeek(xQueue_LuxParam  获取当前Lux值成功\n");
                        CurrentLuxAdcValTemp = ReadLuxAdcVal;

                        for (uint8_t i = 0; i < light_Interval_index_sum; i++)
                        {
                            if (i == light_interval_index_day) //日间区间判断
                            {
                                LowerLuxAdcValTemp = stru_LightSet.stru_LightByLux[i].LowerLux;
                                UpperLuxAdcValTemp = stru_LightSet.stru_LightByLux[i].UpperLux;
                            }
                            else //傍晚和夜晚 区间判断
                            {
                                UpperLuxAdcValTemp = stru_LightSet.stru_LightByLux[i - 1].LowerLux; //当前上限为上一组的下限

                                LowerLuxAdcValTemp = stru_LightSet.stru_LightByLux[i].LowerLux;
                                // LowerLuxAdcValTemp = stru_LightSet.stru_LightByLux[i - 1].UpperLux;
                                // UpperLuxAdcValTemp = stru_LightSet.stru_LightByLux[i].UpperLux;
                            }
                            if (CurrentLuxAdcValTemp >= LowerLuxAdcValTemp && CurrentLuxAdcValTemp < UpperLuxAdcValTemp) //当前小于下限Lux 并且大于上限Lux
                            {
                                // ESP_LOGI(TAG, "i == light_interval_index_day 日间区间判断条件符合\n");
                                Filter_3++;
                                if (Filter_3 >= 3)
                                {
                                    // ESP_LOGI(TAG, "Filter_3 >= 3 日间判断条件符合\n");
                                    Filter_3 = 0;
                                    stru_LightTrigParam.LightTrigIntervalIndex = i; //记住当前触发区间
                                    if (LastLightTrigIntervalIndex != stru_LightTrigParam.LightTrigIntervalIndex || xSemaphoreTake(BinSemap_LightSetSync, (TickType_t)10) == pdTRUE)
                                    {
                                        xSemaphoreTake(BinSemap_LightSetSync, (TickType_t)10); //清零标志位
                                        LastLightTrigIntervalIndex = stru_LightTrigParam.LightTrigIntervalIndex;
                                        oled->enum_DisplayWindow = light_trig_window;
                                        stru_LightTrigParam.LightTrigBright = stru_LightSet.stru_LightByLux[i].LightBright;  //记住当前触发的组的亮度
                                        xQueueOverwrite(xQueue_LightTrigParam, &stru_LightTrigParam);                        //发送显示参数到显示窗口
                                        LedcLightPwmSetDuty((uint32_t)((8190 * stru_LightTrigParam.LightTrigBright) / 100)); //按照设置的pwm百分比输出pwm
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                LedcLightPwmSetDuty((uint32_t)8190 * (0)); //冗余 功能不使能则关闭pwm
            }
        }
    }

    //过滤开启时间触发判断处理
    if (oled->enum_DisplayWindow != filter_set_window)
    { //当不在设置窗口和不在触发窗口时才判断
        if (Flag_ReadFilterParamSuccess == 1)
        {
            if (stru_FilterSet.FilterSwitch == 1) //只有功能使能开启时才判断
            {
                if (xQueuePeek(xQueue_CurrentTime, &stru_GetCurrentTime, (TickType_t)10))
                { // pcRxedMessage now points to the struct AMessage variable posted
                    // by vATask.
                    CurrentTimeMergerTemp = stru_GetCurrentTime.hour * 100; //合并时间参数方便判断
                    CurrentTimeMergerTemp += stru_GetCurrentTime.minute;

                    BeginTimeMergerTemp = stru_FilterSet.stru_FilterBeginTime.hour * 100; //合并时间参数
                    BeginTimeMergerTemp += stru_FilterSet.stru_FilterBeginTime.minute;
                    EndTimeMergerTemp = stru_FilterSet.stru_FilterEndTime.hour * 100; //合并时间参数
                    EndTimeMergerTemp += stru_FilterSet.stru_FilterEndTime.minute;
                    if (CurrentTimeMergerTemp > BeginTimeMergerTemp && CurrentTimeMergerTemp < EndTimeMergerTemp) //大于开始并且小于结束时间
                    {
                        Filter_4++;
                        if (Filter_4 >= 3)
                        {
                            Filter_4 = 0;
                            gpio_set_level(FILTER_DRIVE_GPIO_PIN, FILTER_DRIVE_ON); //时间匹配开启过滤驱动器
                        }
                    }
                    else
                    {
                        Filter_4 = 0;
                        gpio_set_level(FILTER_DRIVE_GPIO_PIN, FILTER_DRIVE_OFF); //时间不匹配则关闭喂食驱动器
                    }
                }
            }
            else
            {
                gpio_set_level(FILTER_DRIVE_GPIO_PIN, FILTER_DRIVE_OFF); //冗余 功能不使能关闭过滤驱动
            }
        }
    }
}

// 选择发送部分或全屏更新二值信号量
void SelectPartOrFullScreenUpdateDisplay(typedef_enum_ShowUpdateSelection ShowUpdateSelection)
{
    switch (ShowUpdateSelection)
    {
    case Full_Screen_Update_Display:
        if (xSemaphoreGive(BinSemap_FullScreenUpdate) == pdTRUE) //同步更新显示
        {
            printf("发送全屏更新二值信号量成功\r\n");
        }
        // oled->Flag_RenewEntireScreen = 1;
        break;
    case Part_1_Update_Display:
        if (xSemaphoreGive(BinSemap_PartUpdate_1) == pdTRUE) //同步更新显示
        {
            // printf("发送部分更新1二值信号量成功\r\n");
        }
        // oled->Flag_Part_1_Update = 1;
        break;
    case Part_2_Update_Display:
        if (xSemaphoreGive(BinSemap_PartUpdate_2) == pdTRUE) //同步更新显示
        {
            // printf("发送部分更新2二值信号量成功\r\n");
        }
        // oled->Flag_Part_2_Update = 1;
        break;
    case Part_3_Update_Display:
        if (xSemaphoreGive(BinSemap_PartUpdate_3) == pdTRUE) //同步更新显示
        {
            // printf("发送部分更新3二值信号量成功\r\n");
        }
        // oled->Flag_Part_3_Update = 1;
        break;
    default:
        break;
    }
}

// 获取部分或全屏更新二值信号量
BaseType_t GetPartOrFullScreenUpdateDisplay(typedef_enum_ShowUpdateSelection ShowUpdateSelection)
{
    BaseType_t err = pdFALSE;
    switch (ShowUpdateSelection)
    {
    case Full_Screen_Update_Display:
        if (xSemaphoreTake(BinSemap_FullScreenUpdate, (TickType_t)0) == pdTRUE) //同步更新显示
        {
            printf("接收全屏更新二值信号量成功\r\n");
            err = pdTRUE;
        }
        // oled->Flag_RenewEntireScreen = 1;
        break;
    case Part_1_Update_Display:
        if (xSemaphoreTake(BinSemap_PartUpdate_1, (TickType_t)0) == pdTRUE) //同步更新显示
        {
            // printf("接收部分更新1二值信号量成功\r\n");
            err = pdTRUE;
        }
        break;
    case Part_2_Update_Display:
        if (xSemaphoreTake(BinSemap_PartUpdate_2, (TickType_t)0) == pdTRUE) //同步更新显示
        {
            // printf("接收部分更新2二值信号量成功\r\n");
            err = pdTRUE;
        }
        break;
    case Part_3_Update_Display:
        if (xSemaphoreTake(BinSemap_PartUpdate_3, (TickType_t)0) == pdTRUE) //同步更新显示
        {
            // printf("接收部分更新3二值信号量成功\r\n");
            err = pdTRUE;
        }
        break;
    default:
        break;
    }
    return err;
}

// 2.中位值滤波法
/*
 * description: 连续采样N次（N取奇数）,把N次采样值按大小排列，
 *              取中间值为本次有效值
 * advantage: 能够有效克服偶然因素引起的波动干扰，
 *            对温度、液位等变化缓慢的被测参数有良好的滤波效果
 * disadvantage: 对流量、速度等快速变化的参数不宜
 */
/* N值可根据实际情况调整
排序采用冒泡法*/
#define N 7

uint16_t filter(adc_channel_t adc_channel)
{
    uint16_t value_buf[N];
    uint8_t count, i, j;
    uint16_t temp;
    for (count = 0; count < N; count++)
    {
        // adc2_get_raw(adc_channel, ADC_WIDTH_BIT_9, &value_buf[count]);
        value_buf[count] = adc1_get_raw((adc1_channel_t)adc_channel);
        // value_buf[count] = GET_ADC_VALUE(adcSetting);
    }
    for (j = 0; j < N - 1; j++)
    {
        for (i = 0; i < N - j; i++)
        {
            if (value_buf[i] > value_buf[i + 1])
            {
                temp = value_buf[i];
                value_buf[i] = value_buf[i + 1];
                value_buf[i + 1] = temp;
            }
        }
    }
    return value_buf[(N - 1) / 2];
}

//==================
//读多次ad取平均处理函数
//==================
void ReadAdcAveValPro(adc_channel_t adc_channel, uint16_t *ReadAdc)
{
    static uint16_t Count = 0;
    static uint16_t max = 0;
    static uint16_t min = 0xff;
    uint16_t temp = 0;
    static uint32_t sum = 0;
    static uint8_t AveValStage = 0;

    if (AveValStage == 0) //初始化
    {
        temp = filter(adc_channel);
#if 0
    temp = 233;
#endif
        max = temp;
        min = temp;
        sum += temp; //读第一次
        AveValStage++;
    }
    else if (AveValStage == 1)
    {
        Count++;
        if (Count > 9) //读9次
        {
            Count = 0;
            AveValStage = 0;                //再次读ADC初始化
            sum = (sum - max) - min;        //去掉最大最小值
            *ReadAdc = (uint8_t)(sum >> 3); //等于除以8 //按位右移3位110 0101 1111
            *ReadAdc = 255 - *ReadAdc;      //值取反 这样环境光越强则值越大 更直观
            sum = 0;
        }
        else //还未读够10次
        {
            temp = filter(adc_channel);
#if 0
    temp = 233;
#endif
            if (temp > max)
            {
                max = temp;
            }

            if (temp < min)
            {
                min = temp;
            }
            sum += temp;
        }
    }
}

void LedcPwmInit(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t t_LedcLightTimer = {
        .speed_mode = LEDC_LIGHT_MODE,
        .timer_num = LEDC_LIGHT_TIMER,
        .duty_resolution = LEDC_LIGHT_DUTY_RES,
        .freq_hz = LEDC_LIGHT_FREQUENCY, // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&t_LedcLightTimer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t t_LedcLightChannel = {
        .speed_mode = LEDC_LIGHT_MODE,
        .channel = LEDC_LIGHT_CHANNEL,
        .timer_sel = LEDC_LIGHT_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_LIGHT_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&t_LedcLightChannel));

    // Prepare and then apply the LEDC PWM timer configuration//舵机驱动服务PWM
    ledc_timer_config_t t_LedcSteerGearTimer = {
        .speed_mode = LEDC_STEER_GEAR_MODE,
        .timer_num = LEDC_STEER_GEAR_TIMER,
        .duty_resolution = LEDC_STEER_GEAR_DUTY_RES,
        .freq_hz = LEDC_STEER_GEAR_FREQUENCY, // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&t_LedcSteerGearTimer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t t_LedcSteerGearChannel = {
        .speed_mode = LEDC_STEER_GEAR_MODE,
        .channel = LEDC_STEER_GEAR_CHANNEL,
        .timer_sel = LEDC_STEER_GEAR_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_STEER_GEAR_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&t_LedcSteerGearChannel));
}

//
void LedcLightPwmSetDuty(uint32_t duty) //设置灯光的pwm占空比
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LIGHT_MODE, LEDC_LIGHT_CHANNEL, duty));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LIGHT_MODE, LEDC_LIGHT_CHANNEL));
}

void LedcSteerGearPwmSetDuty(uint32_t duty) //设置舵机的pwm占空比
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_STEER_GEAR_MODE, LEDC_STEER_GEAR_CHANNEL, duty));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_STEER_GEAR_MODE, LEDC_STEER_GEAR_CHANNEL));
}

//读一次eeprom
// eeprom_dev eeprom的参数结构体
// Point_ReadEepromBuffer 字节指针buffur
// data_addr 读取的eeprom地址
// ReadSize 读取的字节数
// uint8_t 返回校验位 为宏定义DATA_VERIFICATION时则校验通过  为0则校验不通过
uint8_t ReadEeprom(EEPROM_t *eeprom_dev, uint8_t *Point_ReadEepromBuffer, uint16_t data_addr, uint8_t ReadSize)
{
    uint8_t ValidData = 0;
    // vPortFree(Point_ReadEepromBuffer);
    // Point_ReadEepromBuffer = (uint8_t *)pvPortMalloc(ReadSize * sizeof(uint8_t));
    // Point_ReadEepromBuffer = (uint8_t *)realloc(Point_ReadEepromBuffer, ReadSize * sizeof(uint8_t));
    ValidData = ReadMultipByteEeprom(eeprom_dev, data_addr, Point_ReadEepromBuffer, ReadSize);
    if (ValidData == DATA_VERIFICATION) //校验通过 是有效数据
    {
        // ESP_LOGI(TAG, "ValidData = 0x%x 校验成功 是有效数据", ValidData);
        return ValidData;
    }
    else
    {
        // ESP_LOGI(TAG, "ValidData = %x 校验失败", ValidData);
    }
    return ValidData;
}

void OledDisplayReceiveMessageQueuePro(OLEDDisplay_t *oled, QueueHandle_t xQueue_handle)
{
    typedef_stru_DisplayLine GetDisplayLineTemp = {0};
    BaseType_t xTaskWokenByReceive = pdFALSE;
    if (xQueue_handle != 0)
    { // Receive a message on the created queue. Block for 10 ticks if a // message is not immediately available.
        if (xQueuePeek(xQueue_handle, &GetDisplayLineTemp, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
          // by vATask.
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, GetDisplayLineTemp.x, GetDisplayLineTemp.y, &GetDisplayLineTemp.buffer[0]);
        }
    }
}

void SetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //设置窗口显示函数
{
    static typedef_enum_DisplaySetWindowSubOption DisplaySetWindowSubOption = sub_time_option;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        DisplaySetWindowSubOption = sub_time_option;
        oled->DisplayStep++;
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 3, 16 * 0, "设置");

        if (DisplaySetWindowSubOption == 0)
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, ShowSquareBox, 16 * 0, 16 * 1, &oled->DisplayLine_sub_option_buffer[0].buffer[0]);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, &oled->DisplayLine_sub_option_buffer[1].buffer[0]);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, &oled->DisplayLine_sub_option_buffer[2].buffer[0]);
        }
        else if (DisplaySetWindowSubOption == sub_option_Sum)
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, &oled->DisplayLine_sub_option_buffer[DisplaySetWindowSubOption - 2].buffer[0]);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, &oled->DisplayLine_sub_option_buffer[DisplaySetWindowSubOption - 1].buffer[0]);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, ShowSquareBox, 16 * 0, 16 * 3, &oled->DisplayLine_sub_option_buffer[DisplaySetWindowSubOption].buffer[0]);
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, &oled->DisplayLine_sub_option_buffer[DisplaySetWindowSubOption - 1].buffer[0]);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, ShowSquareBox, 16 * 0, 16 * 2, &oled->DisplayLine_sub_option_buffer[DisplaySetWindowSubOption].buffer[0]);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, &oled->DisplayLine_sub_option_buffer[DisplaySetWindowSubOption + 1].buffer[0]);
        }
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (DisplaySetWindowSubOption == sub_option_Sum)
        {
            oled->enum_DisplayWindow = Main_Window; //切换到窗口Main_Window
        }
        else
        {
            oled->enum_DisplayWindow = DisplaySetWindowSubOption + 10; //设置菜单的偏移量
        }
        SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (DisplaySetWindowSubOption < sub_option_Sum)
            {
                ++DisplaySetWindowSubOption;
                ESP_LOGI(TAG, " KeyTask DisplaySetWindowSubOption= %d", DisplaySetWindowSubOption);
                //整屏更新
            }
            else
            {
                DisplaySetWindowSubOption = 0;
            }
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            break;
        case key_down_left:
            if (DisplaySetWindowSubOption > 0)
            {
                --DisplaySetWindowSubOption;
                ESP_LOGI(TAG, " KeyTask DisplaySetWindowSubOption= %d", DisplaySetWindowSubOption);
                //整屏更新
            }
            else
            {
                DisplaySetWindowSubOption = sub_option_Sum;
            }
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);

            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void MainWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //主窗口显示函数
{
    static type_enum_MainWindowCursorPosition enum_MainWindowCursorPosition = cursor_title;
    type_stru_DS1302_DateTime stru_GetCurrentTime = {0}; //获取当前时间
    float ReadTemp;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        enum_MainWindowCursorPosition = cursor_title;
        oled->DisplayStep++;
    }
    // ESP_LOGI(TAG, "feed_time_addr = %d", feed_time_addr);
    // ESP_LOGI(TAG, "sizeof(type_stru_UpperLowerLimitTemperParam) = %d", sizeof(type_stru_UpperLowerLimitTemperParam));
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到 定时刷新
    {
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 1, ":");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 2, ":");

        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, enum_MainWindowCursorPosition == cursor_time ? ShowSquareBox : Normal, 16 * 0, 16 * 1, "时间");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, enum_MainWindowCursorPosition == cursor_water_temper ? ShowSquareBox : Normal, 16 * 0, 16 * 2, "水温");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, enum_MainWindowCursorPosition == main_window_cursor_sum ? ShowSquareBox : Normal, 16 * 0, 16 * 3, "按下进入设置");
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        // OledDisplayReceiveMessageQueuePro(oled, xQueue_DisplayLine_1);
        if (xQueuePeek(xQueue_CurrentTime, &stru_GetCurrentTime, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + 10, 16 * 1, "%02d:%02d:%02d", stru_GetCurrentTime.hour, stru_GetCurrentTime.minute, stru_GetCurrentTime.second);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        // OledDisplayReceiveMessageQueuePro(oled, xQueue_DisplayLine_2);
        if (xQueuePeek(xQueue_ReadTemp, &ReadTemp, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + 10, 16 * 2, "%02.02f℃  ", ReadTemp);
        }
    }
    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_MainWindowCursorPosition > cursor_title)
        {
            if (enum_MainWindowCursorPosition == main_window_cursor_sum)
            {
                oled->enum_DisplayWindow = Set_Window; //切换窗口
                ESP_LOGI(TAG, "key_short_press oled->enum_DisplayWindow = %d", oled->enum_DisplayWindow);
            }
            else
            {
                oled->enum_DisplayWindow = enum_MainWindowCursorPosition + 30; //详情窗口偏移量30
            }
        }
        else if (enum_MainWindowCursorPosition == cursor_title)
        {
            oled->enum_DisplayWindow = Set_Window; //切换窗口
        }
        SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_MainWindowCursorPosition < main_window_cursor_sum)
            {
                enum_MainWindowCursorPosition++;
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            }
            break;
        case key_down_left:
            if (enum_MainWindowCursorPosition > 0)
            {
                enum_MainWindowCursorPosition--;
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void TimeSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //子选项时间设置窗口显示函数
{
    static uint8_t TimeMethodIndex = 0;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        oled->DisplayStep++;
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "时间设置");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 1, "Method:");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, TimeMethodIndex == 0 ? ShowSquareBox : Normal, (16 * 0), 16 * 2, "手动设置时间");
        OledChineseEnglishMixedPrint(oled, clear_screen, TimeMethodIndex > 0 ? ShowSquareBox : Normal, (16 * 0), 16 * 3, "联网自动同步");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //如果需要整屏更新
    {
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (TimeMethodIndex == 0)
        {
            oled->enum_DisplayWindow = time_set_by_manual_window; //切换窗口
        }
        else
        {
            oled->enum_DisplayWindow = time_set_by_net_window; //切换窗口
            /* code */
        }
        SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            // TimeMethodIndex = !TimeMethodIndex;
            // SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
            // break;
            // fallthrought
        case key_down_left:
            TimeMethodIndex = !TimeMethodIndex;
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

esp_err_t Ds1302NetSyncTimePro(OLEDDisplay_t *oled, type_stru_DS1302_DateTime *dt) // ds1302联网自动同步处理函数
{
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    if (esp_qcloud_timesync_check() == false)
    {
        ESP_LOGE(TAG, "Time not synchronized ");
        return ESP_FAIL;
    }

    time(&now);
    // now = now + (CONFIG_TIMEZONE * 60 * 60);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "get current date/time is: %s", strftime_buf);

    ESP_LOGI(TAG, "timeinfo.tm_sec=%d", timeinfo.tm_sec);
    ESP_LOGI(TAG, "timeinfo.tm_min=%d", timeinfo.tm_min);
    ESP_LOGI(TAG, "timeinfo.tm_hour=%d", timeinfo.tm_hour);
    ESP_LOGI(TAG, "timeinfo.tm_wday=%d", timeinfo.tm_wday);
    ESP_LOGI(TAG, "timeinfo.tm_mday=%d", timeinfo.tm_mday);
    ESP_LOGI(TAG, "timeinfo.tm_mon=%d", timeinfo.tm_mon);
    ESP_LOGI(TAG, "timeinfo.tm_year=%d", timeinfo.tm_year);

    // Set date and time
    dt->second = timeinfo.tm_sec;
    dt->minute = timeinfo.tm_min;
    dt->hour = timeinfo.tm_hour;

    if (timeinfo.tm_wday == 0) //实际debug发现 7为 ds1302 的星期日 特殊处理
    {
        dt->dayWeek = 7; // 0= Sunday 1 = Monday
    }
    else
    {
        dt->dayWeek = timeinfo.tm_wday; // 0= Sunday 1 = Monday
    }

    dt->dayMonth = timeinfo.tm_mday;
    dt->month = (timeinfo.tm_mon + 1);
    dt->year = (timeinfo.tm_year + 1900);
    DS1302_setDateTime(oled->stru_TimeSetWindowParam.Point_OledDs1302dev, dt);
    return ESP_OK;
}

void TimeSetByNetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //联网自动同步时间窗口显示函数
{
    static uint8_t NetSetTimeCount = 0;
    static type_stru_DS1302_DateTime dt = {0};

    if (oled->DisplayStep == 0) //初始化赋值一次
    {

        // if (Ds1302NetSyncTimePro(oled, &dt) == ESP_OK)
        // {
        //     /* code */
        // }

        NetSetTimeCount = 20;
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        oled->DisplayStep++;
    }

    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到 定时刷新
    {
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        if (NetSetTimeCount > 0)
        {
            NetSetTimeCount--;
            if (NetSetTimeCount == 10)
            {
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
            }
        }
        else
        {
            oled->enum_DisplayWindow = Main_Window; //切换窗口
        }
        // SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "时间设置");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 1, "联网同步中...");

        // SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        // SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        if (Ds1302NetSyncTimePro(oled, &dt) == ESP_OK) //同步成功
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 1, "同步成功         ");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 0, 16 * 2, "%d-%02d-%02d  ", dt.year, dt.month, dt.dayMonth);
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 0, 16 * 3, "%d:%02d:%02d 星期%s", dt.hour, dt.minute, dt.second, &oled->stru_TimeSetWindowParam.Display_week_string_buffer[dt.dayWeek].buffer[0]);
        }
        else //同步失败
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 1, "同步失败         ");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 2, "当前未联网");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //如果需要整屏更新
    {
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (NetSetTimeCount < 10)
        {
            oled->enum_DisplayWindow = Main_Window; //切换窗口
        }
        // if (TimeMethodIndex == 0)
        // {
        //     oled->enum_DisplayWindow = time_set_by_manual_window; //切换窗口
        // }
        // else
        // {
        //     oled->enum_DisplayWindow = time_set_by_net_window; //切换窗口
        //     /* code */
        // }
        // SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void TimeSetByManualWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //手动设置时间设置窗口显示函数
{
    static typedef_enum_TimeSetCursorPosition enum_TimeSetCursorPosition = cursor_position_year;
    static typedef_enum_CursorSelecParam enum_TimeSetCursorSelecParam = cursor_not_selec_param;
    static type_stru_DS1302_DateTime DateTimeToBeSet = {
        .second = 22,  //!< Second 0..59
        .minute = 12,  //!< Minute 0..59
        .hour = 12,    //!< Hour 0..23
        .dayWeek = 2,  //!< Day of the week (0 as Sunday/ 1 as Monday)
        .dayMonth = 2, //!< Day of the month 1..31
        .month = 2,    //!< Month 1..12
        .year = 2022,  //!< Year 2000..2099
    };

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        if (xQueuePeek(xQueue_CurrentTime, &DateTimeToBeSet, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
        }
        // DateTimeToBeSet = *(type_stru_DS1302_DateTime *)oled->stru_TimeSetWindowParam.Point_OledUpdateDisplayTime;
        oled->DisplayStep++;
    }

    if (enum_TimeSetCursorSelecParam == cursor_selec_param) //选中选项则闪烁处理
    {
        if (get_counter(oled, options_selected_blink_timer) == 0)
        {
            register_counter(oled, options_selected_blink_timer, 3 * COUNT_100_MS);
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        // ESP_LOGI(TAG, "TimeSetWindowPro %d-%02d-%02d", DateTimeToBeSet->year, DateTimeToBeSet->month, DateTimeToBeSet->dayMonth);
        ESP_LOGI(TAG, "TimeSetWindowPro enum_TimeSetCursorPosition =%d", enum_TimeSetCursorPosition);
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "时间设置");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "    -   -  ");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "  :");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }

    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_TimeSetCursorPosition == cursor_position_year ? (enum_TimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 16 * 0, 16 * 1, "%d", DateTimeToBeSet.year);
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_TimeSetCursorPosition == cursor_position_month ? (enum_TimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 5, 16 * 1, "%02d", DateTimeToBeSet.month);
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_TimeSetCursorPosition == cursor_position_day ? (enum_TimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 8, 16 * 1, "%02d", DateTimeToBeSet.dayMonth);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_TimeSetCursorPosition == cursor_position_hour ? (enum_TimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 16 * 0, 16 * 2, "%02d", DateTimeToBeSet.hour);
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_TimeSetCursorPosition == cursor_position_minute ? (enum_TimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 3, 16 * 2, "%02d", DateTimeToBeSet.minute);
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_TimeSetCursorPosition == cursor_position_week ? (enum_TimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 6, 16 * 2, "星期%s", &oled->stru_TimeSetWindowParam.Display_week_string_buffer[DateTimeToBeSet.dayWeek].buffer[0]);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_TimeSetCursorPosition == cursor_complete_and_exit ? ShowSquareBox : Normal, 16 * 0, 16 * 3, "完成并退出");
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_TimeSetCursorPosition == cursor_complete_and_exit)
        {
            oled->enum_DisplayWindow = Set_Window;
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            DS1302_setDateTime(oled->stru_TimeSetWindowParam.Point_OledDs1302dev, &DateTimeToBeSet);
            enum_TimeSetCursorSelecParam = cursor_not_selec_param; //没有选中选项
        }
        else if (enum_TimeSetCursorSelecParam == cursor_not_selec_param)
        {
            enum_TimeSetCursorSelecParam = cursor_selec_param; //选中选项
        }
        else
        {
            enum_TimeSetCursorSelecParam = cursor_not_selec_param;           //没有选中选项
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
        }
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_TimeSetCursorSelecParam == cursor_not_selec_param)
            {
                if (enum_TimeSetCursorPosition < cursor_position_sum - 1)
                {
                    ++enum_TimeSetCursorPosition;
                }
                ESP_LOGI(TAG, " KeyTask enum_TimeSetCursorPosition= %d", enum_TimeSetCursorPosition);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_TimeSetCursorPosition)
                {
                case cursor_position_year:
                    if (DateTimeToBeSet.year < 2099)
                    {
                        ++DateTimeToBeSet.year; //!< Year 2000..2099
                    }
                    break;
                case cursor_position_month:
                    if (DateTimeToBeSet.month < 12)
                    {
                        ++DateTimeToBeSet.month; //!< Month 1..12
                    }
                    break;
                case cursor_position_day:
                    if (DateTimeToBeSet.dayMonth < 31) //!< Day of the month 1..31
                    {
                        ++DateTimeToBeSet.dayMonth;
                    }
                    break;
                case cursor_position_hour:
                    if (DateTimeToBeSet.hour < 23) ////!< Hour 0..23
                    {
                        ++DateTimeToBeSet.hour;
                    }
                    break;
                case cursor_position_minute:
                    if (DateTimeToBeSet.minute < 59) //!< Minute 0..59
                    {
                        ++DateTimeToBeSet.minute;
                    }
                    break;
                case cursor_position_week:
                    if (DateTimeToBeSet.dayWeek < 7) //!< Day of the week (0 as Sunday/ 1 as Monday)
                    {
                        ++DateTimeToBeSet.dayWeek;
                    }
                    break;
                case cursor_complete_and_exit:
                    break;

                default: //冗余
                    enum_TimeSetCursorPosition = cursor_position_year;
                    break;
                }
                if (enum_TimeSetCursorPosition < cursor_position_hour) //局部更新显示处理
                {
                    SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                }
                else
                {
                    if (enum_TimeSetCursorPosition == cursor_complete_and_exit)
                    {
                        /* code */
                    }
                    else
                    {
                        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
                    }
                }
            }
            break;
        case key_down_left:
            if (enum_TimeSetCursorSelecParam == cursor_not_selec_param)
            {
                if (enum_TimeSetCursorPosition > 0)
                {
                    --enum_TimeSetCursorPosition;
                }
                ESP_LOGI(TAG, " KeyTask enum_TimeSetCursorPosition= %d", enum_TimeSetCursorPosition);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_TimeSetCursorPosition)
                {
                case cursor_position_year:
                    if (DateTimeToBeSet.year > 2000)
                    {
                        --DateTimeToBeSet.year; //!< Year 2000..2099
                    }
                    break;
                case cursor_position_month:
                    if (DateTimeToBeSet.month > 1)
                    {
                        --DateTimeToBeSet.month; //!< Month 1..12
                    }
                    break;
                case cursor_position_day:
                    if (DateTimeToBeSet.dayMonth > 1) //!< Day of the month 1..31
                    {
                        --DateTimeToBeSet.dayMonth;
                    }
                    break;
                case cursor_position_hour:
                    if (DateTimeToBeSet.hour > 0) ////!< Hour 0..23
                    {
                        --DateTimeToBeSet.hour;
                    }
                    break;
                case cursor_position_minute:
                    if (DateTimeToBeSet.minute > 0) //!< Minute 0..59
                    {
                        --DateTimeToBeSet.minute;
                    }
                    break;
                case cursor_position_week:
                    if (DateTimeToBeSet.dayWeek > 1) //!< Day of the week (0 as Sunday/ 1 as Monday)
                    {
                        --DateTimeToBeSet.dayWeek;
                    }
                    break;
                case cursor_complete_and_exit:
                    break;

                default: //冗余
                    enum_TimeSetCursorPosition = cursor_position_year;
                    break;
                }
                if (enum_TimeSetCursorPosition < cursor_position_hour) //局部更新显示处理
                {
                    SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                }
                else
                {
                    if (enum_TimeSetCursorPosition == cursor_complete_and_exit)
                    {
                        /* code */
                    }
                    else
                    {
                        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
                    }
                }
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void WaterTemperSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //子选项时间设置窗口显示函数
{
    // static bool OptionsSelectedBlinkIndex = 0; //选项选中闪烁处理
    static type_enum_WaterTemperSetCursorPosition enum_WaterTemperSetCursorPosition = cursor_switch;
    static typedef_enum_CursorSelecParam enum_TemperSetCursorSelecParam = cursor_not_selec_param;
    static type_stru_UpperLowerLimitTemperParam stru_UpperLowerLimitTemperParam = {0};
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, temper_addr, sizeof(type_stru_UpperLowerLimitTemperParam)) == DATA_VERIFICATION)
        {
            stru_UpperLowerLimitTemperParam = *(type_stru_UpperLowerLimitTemperParam *)oled->Point_ReadEepromBuffer;
        }
        enum_WaterTemperSetCursorPosition = cursor_switch;
        enum_TemperSetCursorSelecParam = cursor_not_selec_param;
        oled->DisplayStep++;
    }

    if (enum_TemperSetCursorSelecParam == cursor_selec_param) //选中选项则闪烁处理
    {
        if (get_counter(oled, options_selected_blink_timer) == 0)
        {
            register_counter(oled, options_selected_blink_timer, 3 * COUNT_100_MS);
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "水温设置");

        if (enum_WaterTemperSetCursorPosition < water_temper_cursor_sum) //更新显示处理
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "功能使能:");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "温度上限:");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, "温度下限:");
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, enum_WaterTemperSetCursorPosition == water_temper_cursor_sum ? ShowSquareBox : Normal, (16 * 0), 16 * 1, "完成并退出");
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_WaterTemperSetCursorPosition == cursor_switch ? (enum_TemperSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 4) + 10, 16 * 1, "%s", stru_UpperLowerLimitTemperParam.Switch > 0 ? "开启" : "关闭");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_WaterTemperSetCursorPosition == cursor_temper_upper_limit ? (enum_TemperSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 4) + 10, 16 * 2, "%.1f", stru_UpperLowerLimitTemperParam.upper_limit);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {

        OledChineseEnglishMixedPrint(oled, clear_screen, enum_WaterTemperSetCursorPosition == cursor_temper_lower_limit ? (enum_TemperSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 4) + 10, 16 * 3, "%.1f", stru_UpperLowerLimitTemperParam.lower_limit);
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_WaterTemperSetCursorPosition == water_temper_cursor_sum)
        {
            oled->enum_DisplayWindow = Set_Window;
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);                                                                                            //整屏更新
            enum_TemperSetCursorSelecParam = cursor_not_selec_param;                                                                                                    //没有选中选项
            WriteMultipByteEeprom(oled->Point_EepromDev, temper_addr, (unsigned char *)&stru_UpperLowerLimitTemperParam, sizeof(type_stru_UpperLowerLimitTemperParam)); //将水温上下限设置参数存进eeprom
            if (xSemaphoreGive(BinSemap_SetSync) == pdTRUE)                                                                                                             //同步设置到触发处理
            {
                printf("发送设置同步信号量成功\r\n");
            }
        }
        else if (enum_TemperSetCursorSelecParam == cursor_not_selec_param)
        {
            enum_TemperSetCursorSelecParam = cursor_selec_param; //选中选项
        }
        else
        {
            enum_TemperSetCursorSelecParam = cursor_not_selec_param;         //没有选中选项
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
        }
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_TemperSetCursorSelecParam == cursor_not_selec_param)
            {
                if (enum_WaterTemperSetCursorPosition < water_temper_cursor_sum)
                {
                    ++enum_WaterTemperSetCursorPosition;
                }
                ESP_LOGI(TAG, " KeyTask enum_WaterTemperSetCursorPosition= %d", enum_WaterTemperSetCursorPosition);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_WaterTemperSetCursorPosition)
                {
                case cursor_switch:
                    stru_UpperLowerLimitTemperParam.Switch = !stru_UpperLowerLimitTemperParam.Switch;
                    break;
                case cursor_temper_upper_limit:
                    if (stru_UpperLowerLimitTemperParam.upper_limit < 35)
                    {
                        if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_UpperLowerLimitTemperParam.upper_limit < 34)
                        {
                            stru_UpperLowerLimitTemperParam.upper_limit += 1; //!< 水温 35
                        }
                        else
                        {
                            stru_UpperLowerLimitTemperParam.upper_limit += 0.1; //!< 水温 35
                        }
                    }
                    break;
                case cursor_temper_lower_limit:
                    if (stru_UpperLowerLimitTemperParam.lower_limit < 35)
                    {
                        if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_UpperLowerLimitTemperParam.lower_limit < 34)
                        {
                            stru_UpperLowerLimitTemperParam.lower_limit += 1; //!< 水温 35
                        }
                        else
                        {
                            stru_UpperLowerLimitTemperParam.lower_limit += 0.1; //!< 水温 35
                        }
                    }
                    break;

                default: //冗余
                    enum_WaterTemperSetCursorPosition = cursor_switch;
                    break;
                }
                if (enum_WaterTemperSetCursorPosition < water_temper_cursor_sum) //局部更新显示处理
                {
                    SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                    SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
                    SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
                }
                else
                {
                    SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
                }
            }
            break;
        case key_down_left:
            if (enum_TemperSetCursorSelecParam == cursor_not_selec_param)
            {
                if (enum_WaterTemperSetCursorPosition > 0)
                {
                    --enum_WaterTemperSetCursorPosition;
                }
                ESP_LOGI(TAG, " KeyTask enum_WaterTemperSetCursorPosition= %d", enum_WaterTemperSetCursorPosition);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_WaterTemperSetCursorPosition)
                {
                case cursor_switch:
                    stru_UpperLowerLimitTemperParam.Switch = !stru_UpperLowerLimitTemperParam.Switch;
                    break;
                case cursor_temper_upper_limit:
                    if (stru_UpperLowerLimitTemperParam.upper_limit > 0)
                    {
                        if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_UpperLowerLimitTemperParam.upper_limit > 1)
                        {
                            stru_UpperLowerLimitTemperParam.upper_limit -= 1; //!< 水温 35
                        }
                        else
                        {
                            stru_UpperLowerLimitTemperParam.upper_limit -= 0.1; //!< 水温 35
                        }
                    }
                    break;
                case cursor_temper_lower_limit:
                    if (stru_UpperLowerLimitTemperParam.lower_limit > 0)
                    {
                        if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_UpperLowerLimitTemperParam.lower_limit > 1)
                        {
                            stru_UpperLowerLimitTemperParam.lower_limit -= 1; //!< 水温 35
                        }
                        else
                        {
                            stru_UpperLowerLimitTemperParam.lower_limit -= 0.1; //!< 水温 35
                        }
                    }
                    break;

                default: //冗余
                    enum_WaterTemperSetCursorPosition = cursor_switch;
                    break;
                }
                if (enum_WaterTemperSetCursorPosition < water_temper_cursor_sum) //局部更新显示处理
                {
                    SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                    SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
                    SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
                }
                else
                {
                    SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
                }
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void TimeDetailWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //时间详情窗口显示函数
{
    static typedef_enum_TimeDetailCursorPosition enum_TimeDetailCursorPosition = time_detail_cursor_title;
    type_stru_DS1302_DateTime stru_GetCurrentTime = {0}; //获取当前时间
    if (oled->DisplayStep == 0)                          //初始化赋值一次
    {
        enum_TimeDetailCursorPosition = time_detail_cursor_title;
        oled->DisplayStep++;
    }
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到 定时刷新
    {
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "时间详情");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, enum_TimeDetailCursorPosition == time_detail_cursor_sum ? ShowSquareBox : Normal, 16 * 0, 16 * 3, "按下返回主窗口");
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        if (xQueuePeek(xQueue_CurrentTime, &stru_GetCurrentTime, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 0, 16 * 1, "%d-%02d-%02d  ", stru_GetCurrentTime.year, stru_GetCurrentTime.month, stru_GetCurrentTime.dayMonth);
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 0, 16 * 2, "%d:%02d:%02d 星期%s", stru_GetCurrentTime.hour, stru_GetCurrentTime.minute, stru_GetCurrentTime.second, &oled->stru_TimeSetWindowParam.Display_week_string_buffer[stru_GetCurrentTime.dayWeek].buffer[0]);
        }
        // OledDisplayReceiveMessageQueuePro(oled, xQueue_DisplayLine_1);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        // OledDisplayReceiveMessageQueuePro(oled, xQueue_DisplayLine_2);
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        oled->enum_DisplayWindow = Main_Window;
        // xQueueReset(xQueue_DisplayLine_1); //清空队列
        // xQueueReset(xQueue_DisplayLine_2);
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_TimeDetailCursorPosition < time_detail_cursor_sum)
            {
                enum_TimeDetailCursorPosition++;
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            }
            break;
        case key_down_left:
            if (enum_TimeDetailCursorPosition > 0)
            {
                enum_TimeDetailCursorPosition--;
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void TemperTrigWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //温度触发窗口显示函数
{
    // static typedef_enum_TemperTrigCursorPosition enum_TemperTrigCursorPosition = temper_trig_cursor_title;
    static type_stru_TemperTrigParam stru_TemperTrigParam = {0};
    // float ReadTemp = 0;

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        register_counter(oled, temper_trig_window_close_timer, 10 * COUNT_1_S);
        // enum_TemperTrigCursorPosition = temper_trig_cursor_title;
        oled->DisplayStep++;
    }

    if (get_counter(oled, options_selected_blink_timer) == 0)
    {
        register_counter(oled, options_selected_blink_timer, 5 * COUNT_100_MS);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
    }

    if (get_counter(oled, temper_trig_window_close_timer) == 0)
    {
        oled->enum_DisplayWindow = Main_Window;
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "时间详情");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "触发倥限:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 2, "加热棒");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, ShowInverseSquareBox, 16 * 1, 16 * 0, "温度触发动作");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        if (xQueuePeek(xQueue_TemperTrigParam, &stru_TemperTrigParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 1, stru_TemperTrigParam.enum_UpperOrLowerTrig == temper_upper_trig ? "上" : "下");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 4) + 10, 16 * 1, "%.1f℃", stru_TemperTrigParam.enum_UpperOrLowerTrig == temper_upper_trig ? stru_TemperTrigParam.UpperVal : stru_TemperTrigParam.LowerVal);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 2, stru_TemperTrigParam.enum_UpperOrLowerTrig == temper_upper_trig ? "关闭" : "开启");
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        oled->enum_DisplayWindow = Main_Window;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            // if (enum_TemperTrigCursorPosition < temper_trig_cursor_sum)
            // {
            //     enum_TemperTrigCursorPosition++;
            //     SelectPartOrFullScreenUpdateDisplay( Full_Screen_Update_Display);
            // }
            break;
        case key_down_left:
            // if (enum_TemperTrigCursorPosition > 0)
            // {
            //     enum_TemperTrigCursorPosition--;
            //     SelectPartOrFullScreenUpdateDisplay( Full_Screen_Update_Display);
            // }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void FeedTimeSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //喂食时间设置窗口显示函数
{
    static typedef_enum_FeedTimeSetCursorPosition enum_FeedTimeSetCursorPosition = feed_cursor_position_switch;
    static typedef_enum_CursorSelecParam enum_FeedTimeSetCursorSelecParam = cursor_not_selec_param;
    static uint8_t FeedTableIndex = 0;

    static typedef_stru_SetFeedTimeTable stru_SetFeedTimeTable = {0};

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        enum_FeedTimeSetCursorPosition = feed_cursor_position_switch;
        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, feed_time_addr, sizeof(typedef_stru_SetFeedTimeTable)) == DATA_VERIFICATION)
        {
            ESP_LOGI(TAG, "喂食时间 校验成功");
            stru_SetFeedTimeTable = *(typedef_stru_SetFeedTimeTable *)oled->Point_ReadEepromBuffer;
        }
        else //校验失败则给喂食时间一个初始值  为当前时间
        {
            ESP_LOGI(TAG, "喂食时间 校验失败");
            for (uint8_t i = 0; i < 3; i++)
            {
                stru_SetFeedTimeTable.stru_SetFeedTime[i].hour = 12;
                stru_SetFeedTimeTable.stru_SetFeedTime[i].minute = 12;
            }
        }
        if (stru_SetFeedTimeTable.stru_SetFeedTime[1].hour > 23)
        { //读出的参数混乱 给一个默认时间值
            for (uint8_t i = 0; i < 3; i++)
            {
                stru_SetFeedTimeTable.stru_SetFeedTime[i].hour = 12;
                stru_SetFeedTimeTable.stru_SetFeedTime[i].minute = 12;
            }
        }

#if 0

        for (uint8_t i = 0; i < 3; i++)
        {
            stru_SetFeedTimeTable.stru_SetFeedTime[i] = *(type_stru_DS1302_DateTime *)oled->stru_TimeSetWindowParam.Point_OledUpdateDisplayTime;
        }
#endif

        oled->DisplayStep++;
    }

    if (enum_FeedTimeSetCursorSelecParam == cursor_selec_param) //选中选项则闪烁处理
    {
        if (get_counter(oled, options_selected_blink_timer) == 0)
        {
            register_counter(oled, options_selected_blink_timer, 3 * COUNT_100_MS);
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        // ESP_LOGI(TAG, "TimeSetWindowPro %d-%02d-%02d", DateTimeToBeSet->year, DateTimeToBeSet->month, DateTimeToBeSet->dayMonth);
        // ESP_LOGI(TAG, "TimeSetWindowPro enum_TimeSetCursorPosition =%d", enum_TimeSetCursorPosition);
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "喂食时间设置");
        if (enum_FeedTimeSetCursorPosition < feed_cursor_position_duration) //第一页
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "功能使能:");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "第  组喂食时间:");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 10 * 6, 16 * 3, ":");
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "喂食时长:");
            // OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "  :");
        }

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }

    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        if (enum_FeedTimeSetCursorPosition < feed_cursor_position_duration) //第一页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FeedTimeSetCursorPosition == feed_cursor_position_switch ? (enum_FeedTimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 4) + 10, 16 * 1, "%s", stru_SetFeedTimeTable.FeedSwitch > 0 ? "开启" : "关闭");
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FeedTimeSetCursorPosition == feed_cursor_position_duration ? (enum_FeedTimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 4) + 10, 16 * 1, "%02dS", stru_SetFeedTimeTable.FeedDuration[FeedTableIndex]);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        if (enum_FeedTimeSetCursorPosition < feed_cursor_position_duration) //第一页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FeedTimeSetCursorPosition == feed_cursor_position_table ? (enum_FeedTimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 16 * 1, 16 * 2, "%02d", FeedTableIndex + 1);
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FeedTimeSetCursorPosition == feed_cursor_position_sum ? (enum_FeedTimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 0), 16 * 2, "完成并退出");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {
        if (enum_FeedTimeSetCursorPosition < feed_cursor_position_duration) //第一页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FeedTimeSetCursorPosition == feed_cursor_position_hour ? (enum_FeedTimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 4, 16 * 3, "%02d", stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].hour);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FeedTimeSetCursorPosition == feed_cursor_position_minute ? (enum_FeedTimeSetCursorSelecParam == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 7, 16 * 3, "%02d", stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].minute);
        }
        else
        {
        }
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_FeedTimeSetCursorPosition == feed_cursor_position_sum)
        {
            oled->enum_DisplayWindow = Set_Window;
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            WriteMultipByteEeprom(oled->Point_EepromDev, feed_time_addr, (unsigned char *)&stru_SetFeedTimeTable, sizeof(typedef_stru_SetFeedTimeTable)); //将喂食时间设置参数存进eeprom
            if (xSemaphoreGive(BinSemap_SetSync) == pdTRUE)                                                                                               //同步设置到触发处理
            {
                printf("发送设置同步信号量成功\r\n");
            }
            enum_FeedTimeSetCursorSelecParam = cursor_not_selec_param; //没有选中选项
        }
        else if (enum_FeedTimeSetCursorSelecParam == cursor_not_selec_param)
        {
            enum_FeedTimeSetCursorSelecParam = cursor_selec_param; //选中选项
        }
        else
        {
            enum_FeedTimeSetCursorSelecParam = cursor_not_selec_param;       //没有选中选项
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
        }
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_FeedTimeSetCursorSelecParam == cursor_not_selec_param)
            {
                if (enum_FeedTimeSetCursorPosition < feed_cursor_position_sum)
                {
                    ++enum_FeedTimeSetCursorPosition;
                }
                ESP_LOGI(TAG, " KeyTask enum_FeedTimeSetCursorPosition= %d", enum_FeedTimeSetCursorPosition);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_FeedTimeSetCursorPosition)
                {
                case feed_cursor_position_switch:
                    stru_SetFeedTimeTable.FeedSwitch = !stru_SetFeedTimeTable.FeedSwitch;
                    break;
                case feed_cursor_position_table:
                    if (FeedTableIndex < 2)
                    {
                        FeedTableIndex++;
                    }
                    break;
                // case feed_cursor_position_duration:
                //     if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].year < 2099)
                //     {
                //         ++stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].year; //!< Year 2000..2099
                //     }
                //     break;
                // case feed_cursor_position_month:
                //     if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].month < 12)
                //     {
                //         ++stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].month; //!< Month 1..12
                //     }
                //     break;
                // case feed_cursor_position_day:
                //     if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].dayMonth < 31) //!< Day of the month 1..31
                //     {
                //         ++stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].dayMonth;
                //     }
                //     break;
                case feed_cursor_position_hour:
                    if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].hour < 23) ////!< Hour 0..23
                    {
                        ++stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].hour;
                    }
                    break;
                case feed_cursor_position_minute:
                    if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].minute < 59) //!< Minute 0..59
                    {
                        ++stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].minute;
                    }
                    break;
                case feed_cursor_position_duration:
                    if (stru_SetFeedTimeTable.FeedDuration[FeedTableIndex] < 255)
                    {
                        stru_SetFeedTimeTable.FeedDuration[FeedTableIndex]++;
                    }
                    else
                    {
                        stru_SetFeedTimeTable.FeedDuration[FeedTableIndex] = 0;
                    }
                    break;
                case feed_cursor_position_sum:
                    break;

                default: //冗余
                    enum_FeedTimeSetCursorPosition = feed_cursor_position_switch;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        case key_down_left:
            if (enum_FeedTimeSetCursorSelecParam == cursor_not_selec_param)
            {
                if (enum_FeedTimeSetCursorPosition > 0)
                {
                    --enum_FeedTimeSetCursorPosition;
                }
                ESP_LOGI(TAG, " KeyTask enum_FeedTimeSetCursorPosition= %d", enum_FeedTimeSetCursorPosition);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_FeedTimeSetCursorPosition)
                {
                case feed_cursor_position_switch:
                    stru_SetFeedTimeTable.FeedSwitch = !stru_SetFeedTimeTable.FeedSwitch;
                    break;
                case feed_cursor_position_table:
                    if (FeedTableIndex > 0)
                    {
                        FeedTableIndex--;
                    }
                    break;
                // case feed_cursor_position_duration:
                //     // if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].year > 2000)
                //     // {
                //     //     --stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].year; //!< Year 2000..2099
                //     // }
                //     break;
                // case feed_cursor_position_month:
                //     if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].month > 1)
                //     {
                //         --stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].month; //!< Month 1..12
                //     }
                //     break;
                // case feed_cursor_position_day:
                //     if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].dayMonth > 1) //!< Day of the month 1..31
                //     {
                //         --stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].dayMonth;
                //     }
                //     break;
                case feed_cursor_position_hour:
                    if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].hour > 0) ////!< Hour 0..23
                    {
                        --stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].hour;
                    }
                    break;
                case feed_cursor_position_minute:
                    if (stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].minute > 0) //!< Minute 0..59
                    {
                        --stru_SetFeedTimeTable.stru_SetFeedTime[FeedTableIndex].minute;
                    }
                    break;
                case feed_cursor_position_duration:
                    if (stru_SetFeedTimeTable.FeedDuration[FeedTableIndex] > 0)
                    {
                        stru_SetFeedTimeTable.FeedDuration[FeedTableIndex]--;
                    }
                    else
                    {
                        stru_SetFeedTimeTable.FeedDuration[FeedTableIndex] = 255;
                    }
                    break;

                case feed_cursor_position_sum:
                    break;

                default: //冗余
                    enum_FeedTimeSetCursorPosition = feed_cursor_position_switch;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void FeedTrigWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //喂食时间设置窗口显示函数
{
    static type_stru_FeedTrigParam stru_FeedTrigParam = {0};
    static uint8_t TimeCount = 0;
    static uint8_t SteerGeatFlip = 0; //用于周期翻转舵机
    // float ReadTemp = 0;

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        if (xQueuePeek(xQueue_FeedTrigParam, &stru_FeedTrigParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
            TimeCount = stru_FeedTrigParam.FeedTrigDuration;
        }
        register_counter(oled, feed_trig_window_close_timer, 1 * COUNT_1_S);
        oled->DisplayStep++;
    }

    if (get_counter(oled, feed_trig_window_close_timer) == 0) //显示时间结束返回主窗口
    {
        register_counter(oled, feed_trig_window_close_timer, 1 * COUNT_1_S);
        if (SteerGeatFlip == 0) //
        {
            SteerGeatFlip = 1;
            LedcSteerGearPwmSetDuty(STEER_GEAR_ANGLE_0); //设置舵机的pwm占空比
        }
        else
        {
            SteerGeatFlip = 0;
            LedcSteerGearPwmSetDuty(STEER_GEAR_ANGLE_180); //设置舵机的pwm占空比
        }

        if (TimeCount > 0)
        {
            TimeCount--;
        }
        else //减到0则喂食结束 返回主窗口
        {
            oled->enum_DisplayWindow = Main_Window;
        }
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }

    if (get_counter(oled, options_selected_blink_timer) == 0)
    {
        register_counter(oled, options_selected_blink_timer, 5 * COUNT_100_MS);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "第  组时间触发");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 2, "开始喂食");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, ShowInverseSquareBox, 16 * 0, 16 * 0, "喂食时间触发动作");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        if (xQueuePeek(xQueue_FeedTrigParam, &stru_FeedTrigParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 1, 16 * 1, "%02d", stru_FeedTrigParam.FeedTrigIndex + 1);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (127 / 2) - ((10 * 3) / 2), 16 * 3, "%02dS", TimeCount);
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        oled->enum_DisplayWindow = Main_Window;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void LightSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //灯光设置窗口显示函数
{
    static typedef_enum_LightSetCursor enum_LightSetCursor = light_cursor_switch;
    static typedef_enum_CursorSelecParam enum_LightSetCursorSelec = cursor_not_selec_param;
    static typedef_stru_LightSet stru_LightSet = {0};

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        oled->DisplayStep++;
        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, light_addr, sizeof(typedef_stru_LightSet)) == DATA_VERIFICATION)
        {
            ESP_LOGI(TAG, "灯光 校验成功");
            stru_LightSet = *(typedef_stru_LightSet *)oled->Point_ReadEepromBuffer;
        }
    }

    if (enum_LightSetCursorSelec == cursor_selec_param) //选中选项则闪烁处理
    {
        if (get_counter(oled, options_selected_blink_timer) == 0)
        {
            register_counter(oled, options_selected_blink_timer, 3 * COUNT_100_MS);
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "灯光设置");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "功能使能:");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetCursor == light_cursor_switch ? (enum_LightSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 4) + 10, 16 * 1, "%s", stru_LightSet.LightSwitch > 0 ? "开启" : "关闭");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetCursor == light_cursor_mode ? (enum_LightSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 0), 16 * 2, "%s", stru_LightSet.enum_LightSetModeIndex == mode_index_by_time ? "按照时间段方式 " : "按照环境Lux方式");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetCursor == light_cursor_position_sum ? ShowSquareBox : Normal, (16 * 0), 16 * 3, "进入设置");
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_LightSetCursor == light_cursor_position_sum)
        {
            if (stru_LightSet.enum_LightSetModeIndex == mode_index_by_time)
            {
                oled->enum_DisplayWindow = light_set_by_time_window; //切换窗口
            }
            else if (stru_LightSet.enum_LightSetModeIndex == mode_index_by_lux)
            {
                oled->enum_DisplayWindow = light_set_by_lux_window; //切换窗口
            }
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);

            WriteMultipByteEeprom(oled->Point_EepromDev, light_addr, (unsigned char *)&stru_LightSet, sizeof(typedef_stru_LightSet)); //将灯光设置参数存进eeprom
            if (xSemaphoreGive(BinSemap_SetSync) == pdTRUE)                                                                           //同步设置到触发处理
            {
                printf("发送设置同步信号量成功\r\n");
            }
            // if (xSemaphoreGive(BinSemap_LightSetSync) == pdTRUE) //同步设置到灯光触发
            // {
            //     printf("发送灯光设置同步信号量成功\r\n");
            // }
            // oled->Flag_LightSetSync = 1;                       //同步设置到灯光触发
            enum_LightSetCursorSelec = cursor_not_selec_param; //没有选中选项
        }
        else if (enum_LightSetCursorSelec == cursor_not_selec_param)
        {
            enum_LightSetCursorSelec = cursor_selec_param; //选中选项
        }
        else
        {
            enum_LightSetCursorSelec = cursor_not_selec_param;               //没有选中选项
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
        }
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_LightSetCursorSelec == cursor_not_selec_param)
            {
                if (enum_LightSetCursor < light_cursor_position_sum)
                {
                    ++enum_LightSetCursor;
                }
                ESP_LOGI(TAG, " KeyTask enum_LightSetCursor= %d", enum_LightSetCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_LightSetCursor)
                {
                case light_cursor_switch:
                    stru_LightSet.LightSwitch = !stru_LightSet.LightSwitch;
                    break;
                case light_cursor_mode:
                    stru_LightSet.enum_LightSetModeIndex = !stru_LightSet.enum_LightSetModeIndex;
                    break;

                case light_cursor_position_sum:
                    break;

                default: //冗余
                    enum_LightSetCursor = light_cursor_switch;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        case key_down_left:
            if (enum_LightSetCursorSelec == cursor_not_selec_param)
            {
                if (enum_LightSetCursor > 0)
                {
                    --enum_LightSetCursor;
                }
                ESP_LOGI(TAG, " KeyTask enum_LightSetCursor= %d", enum_LightSetCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_LightSetCursor)
                {
                case light_cursor_switch:
                    stru_LightSet.LightSwitch = !stru_LightSet.LightSwitch;
                    break;
                case light_cursor_mode:
                    stru_LightSet.enum_LightSetModeIndex = !stru_LightSet.enum_LightSetModeIndex;
                    break;

                case light_cursor_position_sum:
                    break;

                default: //冗余
                    enum_LightSetCursor = light_cursor_switch;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void LightSetByTimeWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //按照时间段亮灯的灯光设置窗口显示函数
{
    static typedef_enum_LightSetByTimeCursor enum_LightSetByTimeCursor = light_by_time_cursor_interval_table;
    static typedef_enum_CursorSelecParam enum_LightSetByTimeCursorSelec = cursor_not_selec_param;
    static typedef_enum_LightSetIntervalIndex enum_LightSetIntervalIndex = light_interval_index_day;

    static typedef_stru_LightSet stru_LightSet = {0};

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        enum_LightSetByTimeCursor = light_by_time_cursor_interval_table;
        enum_LightSetByTimeCursorSelec = cursor_not_selec_param;

        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, light_addr, sizeof(typedef_stru_LightSet)) == DATA_VERIFICATION)
        {
            ESP_LOGI(TAG, "灯光 校验成功");
            stru_LightSet = *(typedef_stru_LightSet *)oled->Point_ReadEepromBuffer;
        }
        else //校验失败则给时间一个初始值
        {
            ESP_LOGI(TAG, "灯光 校验失败");
            for (uint8_t i = 0; i < light_Interval_index_sum; i++)
            {
                stru_LightSet.stru_LightByTime[i].stru_BeginTime.hour = 12;
                stru_LightSet.stru_LightByTime[i].stru_BeginTime.minute = 12;
                stru_LightSet.stru_LightByTime[i].stru_EndTime.hour = 12;
                stru_LightSet.stru_LightByTime[i].stru_EndTime.minute = 12;
            }
        }
        if (stru_LightSet.stru_LightByTime[0].stru_BeginTime.hour > 23) //读出的参数混乱 给一个默认时间值
        {
            for (uint8_t i = 0; i < light_Interval_index_sum; i++)
            {
                stru_LightSet.stru_LightByTime[i].stru_BeginTime.hour = 12;
                stru_LightSet.stru_LightByTime[i].stru_BeginTime.minute = 12;
                stru_LightSet.stru_LightByTime[i].stru_EndTime.hour = 12;
                stru_LightSet.stru_LightByTime[i].stru_EndTime.minute = 12;
            }
        }
#if 0
        for (uint8_t i = 0; i < light_Interval_index_sum; i++)
        {
            stru_LightSet.stru_LightByTime[i].stru_BeginTime.hour = 12;
            stru_LightSet.stru_LightByTime[i].stru_BeginTime.minute = 12;
            stru_LightSet.stru_LightByTime[i].stru_EndTime.hour = 12;
            stru_LightSet.stru_LightByTime[i].stru_EndTime.minute = 12;
        }
#endif
        oled->DisplayStep++;
    }

    if (enum_LightSetByTimeCursorSelec == cursor_selec_param) //选中选项则闪烁处理
    {
        if (get_counter(oled, options_selected_blink_timer) == 0)
        {
            register_counter(oled, options_selected_blink_timer, 3 * COUNT_100_MS);
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "灯光设置");

        if (enum_LightSetByTimeCursor < light_by_time_cursor_sum) //第一页
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "  :   -  :  ");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, "亮度:    佰");
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
        else //第二页
        {
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        if (enum_LightSetByTimeCursor < light_by_time_cursor_sum) //第一页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_interval_table ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 0), 16 * 1, "%s", enum_LightSetIntervalIndex == light_interval_index_day ? "日间" : (enum_LightSetIntervalIndex == light_Interval_index_dusk ? "傍晚" : "夜晚"));
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2), 16 * 1, "%s", stru_LightSet.enum_LightSetModeIndex == mode_index_by_time ? "时间区间:" : "Lux区间:");
        }
        else //第二页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, ShowSquareBox, (16 * 0), 16 * 1, "完成并退出");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        if (enum_LightSetIntervalIndex == light_interval_index_day) //只有日间才可以设置开始和结束 其它只能设置结束
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_begin_hour ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 0, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.hour);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_begin_minute ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 3, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.minute);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_end_hour ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 6, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.hour);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_end_minute ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 9, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.minute);
        }
        else
        {
            //上一个时间段的结束 作为本次的开始
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 10 * 0, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex - 1].stru_EndTime.hour);
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 10 * 3, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex - 1].stru_EndTime.minute);

            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_end_hour ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 6, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.hour);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_end_minute ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 9, 16 * 2, "%02d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.minute);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {

        OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByTimeCursor == light_by_time_cursor_bright ? (enum_LightSetByTimeCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 2) + 10, 16 * 3, "%03d", stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].LightBright);
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_LightSetByTimeCursor == light_by_time_cursor_sum)
        {
            oled->enum_DisplayWindow = Set_Window;
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            WriteMultipByteEeprom(oled->Point_EepromDev, light_addr, (unsigned char *)&stru_LightSet, sizeof(typedef_stru_LightSet)); //将灯光设置参数存进eeprom
            if (xSemaphoreGive(BinSemap_SetSync) == pdTRUE)                                                                           //同步设置到触发处理
            {
                printf("发送设置同步信号量成功\r\n");
            }
            if (xSemaphoreGive(BinSemap_LightSetSync) == pdTRUE) //同步设置到灯光触发
            {
                printf("发送灯光设置同步信号量成功\r\n");
            }
            // oled->Flag_LightSetSync = 1;                       //同步设置到灯光触发
            enum_LightSetByTimeCursorSelec = cursor_not_selec_param; //没有选中选项
        }
        else if (enum_LightSetByTimeCursorSelec == cursor_not_selec_param)
        {
            enum_LightSetByTimeCursorSelec = cursor_selec_param; //选中选项
        }
        else
        {
            enum_LightSetByTimeCursorSelec = cursor_not_selec_param;         //没有选中选项
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
        }
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_LightSetByTimeCursorSelec == cursor_not_selec_param)
            {
                if (enum_LightSetByTimeCursor < light_by_time_cursor_sum)
                {
                    ++enum_LightSetByTimeCursor;
                    if (enum_LightSetIntervalIndex == light_interval_index_day)
                    {
                    }
                    else //傍晚和夜晚时间段 只能设置其结束时间  开始时间为上一时间段的结束 特殊处理
                    {
                        if (enum_LightSetByTimeCursor == light_by_time_cursor_begin_hour)
                        {
                            enum_LightSetByTimeCursor = light_by_time_cursor_end_hour;
                        }
                    }
                }
                ESP_LOGI(TAG, " KeyTask enum_LightSetByTimeCursor= %d", enum_LightSetByTimeCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_LightSetByTimeCursor)
                {
                case light_by_time_cursor_interval_table:
                    if (enum_LightSetIntervalIndex < light_Interval_index_night) //
                    {
                        enum_LightSetIntervalIndex++;
                    }
                    break;
                case light_by_time_cursor_begin_hour:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.hour < 23) ////!< Hour 0..23
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.hour++;
                    }
                    break;
                case light_by_time_cursor_begin_minute:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.minute < 59) //!< Minute 0..59
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.minute++;
                    }
                    break;
                case light_by_time_cursor_end_hour:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.hour < 23) ////!< Hour 0..23
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.hour++;
                    }
                    break;
                case light_by_time_cursor_end_minute:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.minute < 59) //!< Minute 0..59
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.minute++;
                    }
                    break;
                case light_by_time_cursor_bright:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].LightBright < 100)
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].LightBright++;
                    }

                    break;
                case light_by_time_cursor_sum:
                    break;

                default: //冗余
                    enum_LightSetByTimeCursor = light_by_time_cursor_interval_table;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        case key_down_left:
            if (enum_LightSetByTimeCursorSelec == cursor_not_selec_param)
            {
                if (enum_LightSetByTimeCursor > 0)
                {
                    --enum_LightSetByTimeCursor;
                    if (enum_LightSetIntervalIndex == light_interval_index_day)
                    {
                    }
                    else //傍晚和夜晚时间段 只能设置其结束时间  开始时间为上一时间段的结束 特殊处理
                    {
                        if (enum_LightSetByTimeCursor == light_by_time_cursor_begin_minute)
                        {
                            enum_LightSetByTimeCursor = light_by_time_cursor_interval_table;
                        }
                    }
                }
                ESP_LOGI(TAG, " KeyTask enum_LightSetByTimeCursor= %d", enum_LightSetByTimeCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_LightSetByTimeCursor)
                {
                case light_by_time_cursor_interval_table:
                    if (enum_LightSetIntervalIndex > 0) //
                    {
                        enum_LightSetIntervalIndex--;
                    }
                    break;
                case light_by_time_cursor_begin_hour:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.hour > 0) ////!< Hour 0..23
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.hour--;
                    }
                    break;
                case light_by_time_cursor_begin_minute:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.minute > 0) //!< Minute 0..59
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_BeginTime.minute--;
                    }
                    break;
                case light_by_time_cursor_end_hour:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.hour > 0) ////!< Hour 0..23
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.hour--;
                    }
                    break;
                case light_by_time_cursor_end_minute:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.minute > 0) //!< Minute 0..59
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].stru_EndTime.minute--;
                    }
                    break;
                case light_by_time_cursor_bright:
                    if (stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].LightBright > 0)
                    {
                        stru_LightSet.stru_LightByTime[enum_LightSetIntervalIndex].LightBright--;
                    }

                    break;
                case light_by_time_cursor_sum:
                    break;

                default: //冗余
                    enum_LightSetByTimeCursor = light_by_time_cursor_interval_table;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }

            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void LightSetByLuxWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //按照环境光照Lux亮灯的灯光设置窗口显示函数
{
    static type_enum_LightSetByLuxCursor enum_LightSetByLuxCursor = light_by_lux_cursor_interval_table;
    static typedef_enum_CursorSelecParam enum_LightSetByLuxCursorSelec = cursor_not_selec_param;
    static typedef_enum_LightSetIntervalIndex enum_LightSetIntervalIndex = light_interval_index_day;

    static typedef_stru_LightSet stru_LightSet = {0};
    uint16_t ReadLuxAdcVal = 0;

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        oled->DisplayStep++;

        enum_LightSetByLuxCursor = light_by_lux_cursor_interval_table;
        enum_LightSetByLuxCursorSelec = cursor_not_selec_param;

        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, light_addr, sizeof(typedef_stru_LightSet)) == DATA_VERIFICATION)
        {
            ESP_LOGI(TAG, "灯光 校验成功");
            stru_LightSet = *(typedef_stru_LightSet *)oled->Point_ReadEepromBuffer;
        }
        else //校验失败
        {
            ESP_LOGI(TAG, "灯光 校验失败");
        }
    }

    if (enum_LightSetByLuxCursorSelec == cursor_selec_param) //选中选项则闪烁处理
    {
        if (get_counter(oled, options_selected_blink_timer) == 0)
        {
            register_counter(oled, options_selected_blink_timer, 3 * COUNT_100_MS);
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
    }

    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到
    {
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        if (enum_LightSetByLuxCursor < light_by_lux_cursor_sum) //第一页时才刷新显示当前lux
        {
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 2, 16 * 0, "灯光设置");

        if (enum_LightSetByLuxCursor < light_by_lux_cursor_sum) //第一页
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "   -");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, "亮度:");
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
        else //第二页
        {
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        if (enum_LightSetByLuxCursor < light_by_lux_cursor_sum) //第一页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByLuxCursor == light_by_lux_cursor_interval_table ? (enum_LightSetByLuxCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 0), 16 * 1, "%s", enum_LightSetIntervalIndex == light_interval_index_day ? "日间" : (enum_LightSetIntervalIndex == light_Interval_index_dusk ? "傍晚" : "夜晚"));
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2), 16 * 1, "%s", stru_LightSet.enum_LightSetModeIndex == mode_index_by_time ? "时间区间:" : "Lux区间:");
        }
        else //第二页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, ShowSquareBox, (16 * 0), 16 * 1, "完成并退出");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        if (enum_LightSetIntervalIndex == light_interval_index_day) //只有日间才可以设置上限和下限 其它只能设置下限
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByLuxCursor == light_by_lux_cursor_lower_val ? (enum_LightSetByLuxCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 0, 16 * 2, "%03d", stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByLuxCursor == light_by_lux_cursor_upper_val ? (enum_LightSetByLuxCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 4, 16 * 2, "%03d", stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux);
        }
        else
        {
            //上一个lux段的下限作为本次的上限
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 10 * 4, 16 * 2, "%03d", stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex - 1].LowerLux);

            OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByLuxCursor == light_by_lux_cursor_lower_val ? (enum_LightSetByLuxCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 0, 16 * 2, "%03d", stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {
        if (xQueuePeek(xQueue_LuxParam, &ReadLuxAdcVal, (TickType_t)10))
        {                                      // pcRxedMessage now points to the struct AMessage variable posted
                                               // by vATask.
            OLEDDisplay_setColor(oled, BLACK); //特殊处理 用于设置lux区间时  旁边显示当前的Lux值参考 高度10像素的小字显示
            OLEDDisplay_fillRect(oled, (10 * 7), 16 * 2, 127 - (10 * 8), 16);
            OLEDDisplay_fillRect(oled, (16 * 2) + (10 * 5), 16 * 3, 127 - ((16 * 2) + (10 * 5)), 16);
            OLEDDisplay_setColor(oled, WHITE);
            OLEDDisplay_setFont(oled, ArialMT_Plain_10);
            OLEDDisplay_drawStringf(oled, (10 * 7), 16 * 2, "CurrentLux:");
            OLEDDisplay_drawStringf(oled, (10 * 7), 16 * 2 + 10, "     %03d", ReadLuxAdcVal);
            OLEDDisplay_display(oled);
            OLEDDisplay_setFont(oled, ArialMT_Plain_16);
        }
        OledChineseEnglishMixedPrint(oled, clear_screen, enum_LightSetByLuxCursor == light_by_lux_cursor_bright ? (enum_LightSetByLuxCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 2) + 10, 16 * 3, "%03d佰", stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LightBright);
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_LightSetByLuxCursor == light_by_lux_cursor_sum)
        {
            oled->enum_DisplayWindow = Set_Window;
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            WriteMultipByteEeprom(oled->Point_EepromDev, light_addr, (unsigned char *)&stru_LightSet, sizeof(typedef_stru_LightSet)); //将灯光设置参数存进eeprom
            if (xSemaphoreGive(BinSemap_SetSync) == pdTRUE)                                                                           //同步设置到触发处理
            {
                printf("发送设置同步信号量成功\r\n");
            }
            if (xSemaphoreGive(BinSemap_LightSetSync) == pdTRUE) //同步设置到灯光触发
            {
                printf("发送灯光设置同步信号量成功\r\n");
            }
            // oled->Flag_LightSetSync = 1;                       //同步设置到灯光触发
            enum_LightSetByLuxCursorSelec = cursor_not_selec_param; //没有选中选项
        }
        else if (enum_LightSetByLuxCursorSelec == cursor_not_selec_param)
        {
            enum_LightSetByLuxCursorSelec = cursor_selec_param; //选中选项
        }
        else
        {
            enum_LightSetByLuxCursorSelec = cursor_not_selec_param;          //没有选中选项
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
        }
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_LightSetByLuxCursorSelec == cursor_not_selec_param)
            {
                if (enum_LightSetByLuxCursor < light_by_lux_cursor_sum)
                {
                    ++enum_LightSetByLuxCursor;
                    if (enum_LightSetIntervalIndex == light_interval_index_day)
                    {
                    }
                    else //傍晚和夜晚lux区间 只能设置其下限值  上限时间为上一lux区间的下限 特殊处理
                    {
                        if (enum_LightSetByLuxCursor == light_by_lux_cursor_upper_val)
                        {
                            enum_LightSetByLuxCursor = light_by_lux_cursor_bright;
                        }
                    }
                }
                ESP_LOGI(TAG, " KeyTask enum_LightSetByLuxCursor= %d", enum_LightSetByLuxCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_LightSetByLuxCursor)
                {
                case light_by_lux_cursor_interval_table:
                    if (enum_LightSetIntervalIndex < light_Interval_index_night) //
                    {
                        enum_LightSetIntervalIndex++;
                    }
                    break;
                case light_by_lux_cursor_lower_val:
                    if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux < 255 - 10) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux += 10;
                    }
                    else if (stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux < 255) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux++;
                    }
                    break;
                case light_by_lux_cursor_upper_val:
                    if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux < 255 - 10) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux += 10;
                    }
                    else if (stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux < 255) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux++;
                    }
                    break;
                case light_by_lux_cursor_bright:
                    if (stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LightBright < 100)
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LightBright++;
                    }

                    break;
                case light_by_lux_cursor_sum:
                    break;

                default: //冗余
                    enum_LightSetByLuxCursor = light_by_lux_cursor_interval_table;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        case key_down_left:
            if (enum_LightSetByLuxCursorSelec == cursor_not_selec_param)
            {
                if (enum_LightSetByLuxCursor > 0)
                {
                    --enum_LightSetByLuxCursor;
                    if (enum_LightSetIntervalIndex == light_interval_index_day)
                    {
                    }
                    else //傍晚和夜晚lux区间 只能设置其下限值  上限时间为上一lux区间的下限 特殊处理
                    {
                        if (enum_LightSetByLuxCursor == light_by_lux_cursor_upper_val)
                        {
                            enum_LightSetByLuxCursor = light_by_lux_cursor_lower_val;
                        }
                    }
                }
                ESP_LOGI(TAG, " KeyTask enum_LightSetByLuxCursor= %d", enum_LightSetByLuxCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_LightSetByLuxCursor)
                {
                case light_by_lux_cursor_interval_table:
                    if (enum_LightSetIntervalIndex > 0) //
                    {
                        enum_LightSetIntervalIndex--;
                    }
                    break;
                case light_by_lux_cursor_lower_val:
                    if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux > 0 + 10) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux -= 10;
                    }
                    else if (stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux > 0) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LowerLux--;
                    }
                    break;
                case light_by_lux_cursor_upper_val:
                    if (gpio_get_level(CONFIG_ROT_ENC_BUTTON_GPIO) == 0 && stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux > 0 + 10) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux -= 10;
                    }
                    else if (stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux > 0) //
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].UpperLux--;
                    }
                    break;
                case light_by_lux_cursor_bright:
                    if (stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LightBright > 0)
                    {
                        stru_LightSet.stru_LightByLux[enum_LightSetIntervalIndex].LightBright--;
                    }

                    break;
                case light_by_lux_cursor_sum:
                    break;

                default: //冗余
                    enum_LightSetByLuxCursor = light_by_lux_cursor_interval_table;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void LightTrigWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //灯光触发窗口显示函数
{
    static type_stru_LightTrigParam stru_LightTrigParam = {0};

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        register_counter(oled, temper_trig_window_close_timer, 10 * COUNT_1_S);
        // enum_TemperTrigCursorPosition = temper_trig_cursor_title;
        oled->DisplayStep++;
    }
    if (get_counter(oled, temper_trig_window_close_timer) == 0)
    {
        oled->enum_DisplayWindow = Main_Window;
    }

    if (get_counter(oled, options_selected_blink_timer) == 0)
    {
        register_counter(oled, options_selected_blink_timer, 5 * COUNT_100_MS);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "亮度:   佰");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, ShowInverseSquareBox, 16 * 1, 16 * 0, "灯光触发动作");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
        if (xQueuePeek(xQueue_LightTrigParam, &stru_LightTrigParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 1, "%s", stru_LightTrigParam.LightTrigIntervalIndex == light_interval_index_day ? "日间" : (stru_LightTrigParam.LightTrigIntervalIndex == light_Interval_index_dusk ? "傍晚" : "夜晚"));
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2), 16 * 1, "%s", stru_LightTrigParam.LightTrigModeIndex == mode_index_by_time ? "区间触发:" : "Lux触发:");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + 10, 16 * 2, "%03d", stru_LightTrigParam.LightTrigBright);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        oled->enum_DisplayWindow = Main_Window;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void SystemStatusWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //系统状态查看窗口显示函数
{
    static type_stru_SystemStatusWindowParam stru_SystemStatusWindowParam = {0};
    static type_stru_TemperTrigParam stru_TemperTrigParam = {0};

    // static type_enum_MainWindowCursorPosition enum_MainWindowCursorPosition = cursor_title;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        // enum_MainWindowCursorPosition = cursor_title;
        if (xQueuePeek(xqueue_SystemStatusParam, &stru_SystemStatusWindowParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
        }
        if (xQueuePeek(xQueue_TemperTrigParam, &stru_TemperTrigParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
        }
        oled->DisplayStep++;
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "水温状态:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 2, "加热棒");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
        if (stru_SystemStatusWindowParam.WaterTemperSwitch == 0)
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 1, "关闭");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 2, "关闭");
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 1, stru_TemperTrigParam.enum_UpperOrLowerTrig == temper_upper_trig ? "触发上限:" : "触发下限:");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 4) + 10, 16 * 1, "%.1f℃", stru_TemperTrigParam.enum_UpperOrLowerTrig == temper_upper_trig ? stru_TemperTrigParam.UpperVal : stru_TemperTrigParam.LowerVal);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 2, stru_TemperTrigParam.enum_UpperOrLowerTrig == temper_upper_trig ? "关闭" : "开启");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void SystemStatusWindow_2Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //系统状态查看窗口2显示函数
{
    uint16_t ReadLuxAdcVal = 0;
    static type_stru_SystemStatusWindowParam stru_SystemStatusWindowParam = {0};
    static type_stru_LightTrigParam stru_LightTrigParam = {0};

    // static type_enum_MainWindowCursorPosition enum_MainWindowCursorPosition = cursor_title;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        // enum_MainWindowCursorPosition = cursor_title;
        if (xQueuePeek(xqueue_SystemStatusParam, &stru_SystemStatusWindowParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
        }
        if (xQueuePeek(xQueue_LightTrigParam, &stru_LightTrigParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
        }
        oled->DisplayStep++;
    }
    // ESP_LOGI(TAG, "feed_time_addr = %d", feed_time_addr);
    // ESP_LOGI(TAG, "sizeof(type_stru_UpperLowerLimitTemperParam) = %d", sizeof(type_stru_UpperLowerLimitTemperParam));
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到
    {
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "灯光状态:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "亮度:   ");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, "当前Lux:");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
        if (stru_SystemStatusWindowParam.LightSwitch == 0)
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 1, "关闭");
            // OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 2) + 10, 16 * 2, "关闭");
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 1, "%s", stru_LightTrigParam.LightTrigIntervalIndex == light_interval_index_day ? "日间" : (stru_LightTrigParam.LightTrigIntervalIndex == light_Interval_index_dusk ? "傍晚" : "夜晚"));
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2), 16 * 1, "%s", stru_LightTrigParam.LightTrigModeIndex == mode_index_by_time ? "时间区间触发" : "Lux区间触发");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + 10, 16 * 2, "%03d佰", stru_LightTrigParam.LightTrigBright);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
        if (xQueuePeek(xQueue_LuxParam, &ReadLuxAdcVal, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
          // by vATask.
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + (10 * 4), 16 * 3, "%03d", ReadLuxAdcVal);
        }
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void SystemStatusWindow_3Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //系统状态查看窗口3显示函数
{
    static type_stru_SystemStatusWindowParam stru_SystemStatusWindowParam = {0};
    static type_stru_FeedTrigParam stru_FeedTrigParam = {0};

    // static type_enum_MainWindowCursorPosition enum_MainWindowCursorPosition = cursor_title;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        // enum_MainWindowCursorPosition = cursor_title;
        if (xQueuePeek(xqueue_SystemStatusParam, &stru_SystemStatusWindowParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
        }
        if (xQueuePeek(xQueue_FeedTrigParam, &stru_FeedTrigParam, (TickType_t)10))
        { // pcRxedMessage now points to the struct AMessage variable posted
        }
        oled->DisplayStep++;
    }
    // ESP_LOGI(TAG, "feed_time_addr = %d", feed_time_addr);
    // ESP_LOGI(TAG, "sizeof(type_stru_UpperLowerLimitTemperParam) = %d", sizeof(type_stru_UpperLowerLimitTemperParam));
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到
    {
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "喂食状态:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "触发时间:");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
        if (stru_SystemStatusWindowParam.FeedSwitch == 0)
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 1, "关闭");
        }
        else
        {
            // OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 1, "%s", stru_LightTrigParam.LightTrigIntervalIndex == light_interval_index_day ? "日间" : (stru_LightTrigParam.LightTrigIntervalIndex == light_Interval_index_dusk ? "傍晚" : "夜晚"));
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 10 * 2, 16 * 1, "组为下次触发");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "%02d", stru_SystemStatusWindowParam.ToBeFeedTrigIndex + 1);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 4) + 10, 16 * 2, "%02d:%02d", stru_SystemStatusWindowParam.stru_ToBeTriFeedTime.hour, stru_SystemStatusWindowParam.stru_ToBeTriFeedTime.minute);
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void SystemStatusWindow_4Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //系统状态查看窗口3显示函数
{

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        oled->DisplayStep++;
    }
    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "过滤状态:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "当前在设置时间");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 2, "水泵");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
        if (gpio_get_level(FILTER_DRIVE_GPIO_PIN) == FILTER_DRIVE_ON)
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 7), 16 * 1, "内");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 2, "开启");
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 7), 16 * 1, "外");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, (16 * 0), 16 * 2, "关闭");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void FilterSetWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //过滤水泵设置窗口显示函数
{
    static type_enum_FilterSetCursor enum_FilterSetCursor = filter_cursor_switch;
    static typedef_enum_CursorSelecParam enum_FilterSetCursorSelec = cursor_not_selec_param;

    static type_stru_FilterSet stru_FilterSet = {0};
    // static typedef_stru_LightSet stru_FilterSet = {0};
    // uint16_t ReadLuxAdcVal = 0;

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        enum_FilterSetCursor = filter_cursor_switch;

        if (ReadEeprom(oled->Point_EepromDev, oled->Point_ReadEepromBuffer, filter_addr, sizeof(type_stru_FilterSet)) == DATA_VERIFICATION)
        {
            ESP_LOGI(TAG, "ReadEeprom 过滤参数  校验成功");
            stru_FilterSet = *(type_stru_FilterSet *)oled->Point_ReadEepromBuffer;
        }
        else //校验失败则给时间一个初始值
        {
            ESP_LOGI(TAG, "ReadEeprom 过滤参数 校验失败");
            stru_FilterSet.stru_FilterBeginTime.hour = 12;
            stru_FilterSet.stru_FilterBeginTime.minute = 12;
            stru_FilterSet.stru_FilterEndTime.hour = 12;
            stru_FilterSet.stru_FilterEndTime.minute = 12;
        }
        if (stru_FilterSet.stru_FilterBeginTime.hour > 23) //读出的参数混乱 给一个默认时间值
        {
            stru_FilterSet.stru_FilterBeginTime.hour = 12;
            stru_FilterSet.stru_FilterBeginTime.minute = 12;
            stru_FilterSet.stru_FilterEndTime.hour = 12;
            stru_FilterSet.stru_FilterEndTime.minute = 12;
        }
#if 0
        for (uint8_t i = 0; i < light_Interval_index_sum; i++)
        {
            stru_FilterSet.stru_LightByTime[i].stru_BeginTime.hour = 12;
            stru_FilterSet.stru_LightByTime[i].stru_BeginTime.minute = 12;
            stru_FilterSet.stru_LightByTime[i].stru_EndTime.hour = 12;
            stru_FilterSet.stru_LightByTime[i].stru_EndTime.minute = 12;
        }
#endif
        oled->DisplayStep++;
    }

    if (enum_FilterSetCursorSelec == cursor_selec_param) //选中选项则闪烁处理
    {
        if (get_counter(oled, options_selected_blink_timer) == 0)
        {
            register_counter(oled, options_selected_blink_timer, 3 * COUNT_100_MS);
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
            SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
        }
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "过滤水泵设置");
        if (enum_FilterSetCursor < filter_cursor_position_sum) //第一页
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "功能使能:");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "开过滤时间区间:");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, "  :   -  :  ");
        }
        else //第二页
        {
        }

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }

    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE)
    {
        if (enum_FilterSetCursor < filter_cursor_position_sum) //第一页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FilterSetCursor == filter_cursor_switch ? (enum_FilterSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 4) + 10, 16 * 1, "%s", stru_FilterSet.FilterSwitch > 0 ? "开启" : "关闭");
        }
        else //第二页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FilterSetCursor == filter_cursor_position_sum ? (enum_FilterSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, (16 * 0), 16 * 1, "完成并退出");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE)
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE)
    {
        if (enum_FilterSetCursor < filter_cursor_position_sum) //第一页
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FilterSetCursor == filter_cursor_begin_hour ? (enum_FilterSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 0, 16 * 3, "%02d", stru_FilterSet.stru_FilterBeginTime.hour);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FilterSetCursor == filter_cursor_begin_minute ? (enum_FilterSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 3, 16 * 3, "%02d", stru_FilterSet.stru_FilterBeginTime.minute);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FilterSetCursor == filter_cursor_end_hour ? (enum_FilterSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 6, 16 * 3, "%02d", stru_FilterSet.stru_FilterEndTime.hour);
            OledChineseEnglishMixedPrint(oled, clear_screen, enum_FilterSetCursor == filter_cursor_end_minute ? (enum_FilterSetCursorSelec == cursor_selec_param ? ShowInverseSquareBox : ShowSquareBox) : Normal, 10 * 9, 16 * 3, "%02d", stru_FilterSet.stru_FilterEndTime.minute);
        }
        else //第二页
        {
        }
    }

    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        stru_KeyValue->enum_KeyPress = key_not_press;
        if (enum_FilterSetCursor == filter_cursor_position_sum)
        {
            oled->enum_DisplayWindow = Set_Window;
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
            WriteMultipByteEeprom(oled->Point_EepromDev, filter_addr, (unsigned char *)&stru_FilterSet, sizeof(type_stru_FilterSet)); //将灯光设置参数存进eeprom
            if (xSemaphoreGive(BinSemap_SetSync) == pdTRUE)                                                                           //同步设置到触发处理
            {
                printf("发送设置同步信号量成功\r\n");
            }
            // if (xSemaphoreGive(BinSemap_LightSetSync) == pdTRUE) //同步设置到灯光触发
            // {
            //     printf("发送灯光设置同步信号量成功\r\n");
            // }
            // oled->Flag_LightSetSync = 1;                       //同步设置到灯光触发
            enum_FilterSetCursorSelec = cursor_not_selec_param; //没有选中选项
        }
        else if (enum_FilterSetCursorSelec == cursor_not_selec_param)
        {
            enum_FilterSetCursorSelec = cursor_selec_param; //选中选项
        }
        else
        {
            enum_FilterSetCursorSelec = cursor_not_selec_param;              //没有选中选项
            SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display); //整屏更新
        }
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            if (enum_FilterSetCursorSelec == cursor_not_selec_param)
            {
                if (enum_FilterSetCursor < filter_cursor_position_sum)
                {
                    ++enum_FilterSetCursor;
                }
                ESP_LOGI(TAG, " KeyTask enum_FilterSetCursor= %d", enum_FilterSetCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_FilterSetCursor)
                {
                case filter_cursor_switch:
                    stru_FilterSet.FilterSwitch = !stru_FilterSet.FilterSwitch;
                    break;
                case filter_cursor_begin_hour:
                    if (stru_FilterSet.stru_FilterBeginTime.hour < 23) ////!< Hour 0..23
                    {
                        stru_FilterSet.stru_FilterBeginTime.hour++;
                    }
                    break;
                case filter_cursor_begin_minute:
                    if (stru_FilterSet.stru_FilterBeginTime.minute < 59) //!< Minute 0..59
                    {
                        stru_FilterSet.stru_FilterBeginTime.minute++;
                    }
                    break;
                case filter_cursor_end_hour:
                    if (stru_FilterSet.stru_FilterEndTime.hour < 23) ////!< Hour 0..23
                    {
                        stru_FilterSet.stru_FilterEndTime.hour++;
                    }
                    break;
                case filter_cursor_end_minute:
                    if (stru_FilterSet.stru_FilterEndTime.minute < 59) //!< Minute 0..59
                    {
                        stru_FilterSet.stru_FilterEndTime.minute++;
                    }
                    break;
                case filter_cursor_position_sum:
                    break;

                default: //冗余
                    enum_FilterSetCursor = filter_cursor_switch;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        case key_down_left:
            if (enum_FilterSetCursorSelec == cursor_not_selec_param)
            {
                if (enum_FilterSetCursor > 0)
                {
                    --enum_FilterSetCursor;
                }
                ESP_LOGI(TAG, " KeyTask enum_FilterSetCursor= %d", enum_FilterSetCursor);
                SelectPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display);
                //整屏更新
            }
            else
            {
                switch (enum_FilterSetCursor)
                {
                case filter_cursor_switch:
                    stru_FilterSet.FilterSwitch = !stru_FilterSet.FilterSwitch;
                    break;
                case filter_cursor_begin_hour:
                    if (stru_FilterSet.stru_FilterBeginTime.hour > 0) ////!< Hour 0..23
                    {
                        stru_FilterSet.stru_FilterBeginTime.hour--;
                    }
                    break;
                case filter_cursor_begin_minute:
                    if (stru_FilterSet.stru_FilterBeginTime.minute > 0) //!< Minute 0..59
                    {
                        stru_FilterSet.stru_FilterBeginTime.minute--;
                    }
                    break;
                case filter_cursor_end_hour:
                    if (stru_FilterSet.stru_FilterEndTime.hour > 0) ////!< Hour 0..23
                    {
                        stru_FilterSet.stru_FilterEndTime.hour--;
                    }
                    break;
                case filter_cursor_end_minute:
                    if (stru_FilterSet.stru_FilterEndTime.minute > 0) //!< Minute 0..59
                    {
                        stru_FilterSet.stru_FilterEndTime.minute--;
                    }
                    break;
                case filter_cursor_position_sum:
                    break;

                default: //冗余
                    enum_FilterSetCursor = filter_cursor_switch;
                    break;
                }
                SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display); //局部更新显示
                SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display); //局部更新显示
            }
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void DemonstratWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //演示demo窗口显示函数
{
    static uint8_t DemoTimeCount = 0;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        DemoTimeCount = 5;
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        oled->DisplayStep++;
    }
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到
    {
        DemoTimeCount--;
        if (DemoTimeCount == 0)
        {
            oled->enum_DisplayWindow = demonstrat_window_2;
        }

        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "水温演示:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "触发");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 2, "加热继电器");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
        if (DemoTimeCount > 2)
        {
            gpio_set_level(HEATING_BASEBALL_GPIO_PIN, HEATING_BASEBALL_ON); //开启加热棒
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 2, 16 * 1, "下限:23.5℃");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 2, "开启");
        }
        else
        {
            gpio_set_level(HEATING_BASEBALL_GPIO_PIN, HEATING_BASEBALL_OFF); //关闭加热棒
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 2, 16 * 1, "上限:32.5℃");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 2, "关闭");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        oled->enum_DisplayWindow = demonstrat_window_2;
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void DemonstratWindow_2Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //演示demo窗口_2显示函数
{
    static uint8_t DemoTimeCount = 0;
    static uint8_t SteerGeatFlip = 0; //用于周期翻转舵机

    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        DemoTimeCount = 10;
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        oled->DisplayStep++;
    }

    if (get_counter(oled, window_refresh_timer) == 0) //显示时间结束返回主窗口
    {
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        if (SteerGeatFlip == 0) //
        {
            SteerGeatFlip = 1;
            LedcSteerGearPwmSetDuty(STEER_GEAR_ANGLE_0); //设置舵机的pwm占空比
        }
        else
        {
            SteerGeatFlip = 0;
            LedcSteerGearPwmSetDuty(STEER_GEAR_ANGLE_180); //设置舵机的pwm占空比
        }

        if (DemoTimeCount > 0)
        {
            DemoTimeCount--;
        }
        else //减到0则演示结束
        {
            oled->enum_DisplayWindow = demonstrat_window_3;
        }
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "喂食演示:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "喂食开始:");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (127 / 2) - ((10 * 3) / 2), 16 * 2, "%02dS", DemoTimeCount);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        oled->enum_DisplayWindow = demonstrat_window_3;
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void DemonstratWindow_3Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //演示demo窗口_3显示函数
{
    static uint8_t DemoTimeCount = 0;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        DemoTimeCount = 7;
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        oled->DisplayStep++;
    }
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到
    {
        DemoTimeCount--;
        if (DemoTimeCount == 0)
        {
            oled->enum_DisplayWindow = demonstrat_window_4;
        }

        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "灯光演示:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 1, "时间触发");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "亮度:");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
        if (DemoTimeCount > 4)
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 0, 16 * 1, "日间");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + 10, 16 * 2, "100佰");
            LedcLightPwmSetDuty((uint32_t)((8190 * 100) / 100)); //按照设置的pwm百分比输出pwm
        }
        else if (DemoTimeCount > 2)
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 0, 16 * 1, "傍晚");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + 10, 16 * 2, "60佰 ");
            LedcLightPwmSetDuty((uint32_t)((8190 * 60) / 100)); //按照设置的pwm百分比输出pwm
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 0, 16 * 1, "夜晚");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 2) + 10, 16 * 2, "20佰 ");
            LedcLightPwmSetDuty((uint32_t)((8190 * 20) / 100)); //按照设置的pwm百分比输出pwm
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        oled->enum_DisplayWindow = demonstrat_window_4;
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void DemonstratWindow_4Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //演示demo窗口_4显示函数
{
    static uint8_t DemoTimeCount = 0;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        DemoTimeCount = 5;
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        oled->DisplayStep++;
    }
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到
    {
        DemoTimeCount--;
        if (DemoTimeCount == 0)
        {
            oled->enum_DisplayWindow = demonstrat_window_5;
        }

        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "过滤演示:");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "当前在设置时间");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 2, 16 * 2, "过滤继电器");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
        if (DemoTimeCount > 2)
        {
            gpio_set_level(FILTER_DRIVE_GPIO_PIN, FILTER_DRIVE_ON); //开启过滤驱动器 继电器
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 7, 16 * 1, "内");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 2, "开启");
        }
        else
        {
            gpio_set_level(FILTER_DRIVE_GPIO_PIN, FILTER_DRIVE_OFF); //关闭喂食驱动器 继电器
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, 16 * 7, 16 * 1, "外");
            OledChineseEnglishMixedPrint(oled, clear_screen, Normal, (16 * 0), 16 * 2, "关闭");
        }
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        oled->enum_DisplayWindow = demonstrat_window_5;
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void DemonstratWindow_5Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //演示demo窗口_5显示函数
{
    static uint8_t DemoTimeCount = 0;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        DemoTimeCount = 3;
        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        oled->DisplayStep++;
    }
    if (get_counter(oled, window_refresh_timer) == 0) //计时时间到
    {
        DemoTimeCount--;
        if (DemoTimeCount == 0)
        {
            if (xSemaphoreGive(BinSemap_DemoExecutSync) == pdTRUE) //同步到触发处理
            {
                // printf("发送云端指令同步信号量成功\r\n");
            }
            oled->enum_DisplayWindow = Main_Window;
        }

        register_counter(oled, window_refresh_timer, 1 * COUNT_1_S);
        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "演示demo结束");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        oled->enum_DisplayWindow = Main_Window;
        if (xSemaphoreGive(BinSemap_DemoExecutSync) == pdTRUE) //同步到触发处理
        {
            // printf("发送云端指令同步信号量成功\r\n");
        }
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void CloudFeedByManualSwitchPro(bool OnOff) //云端下发手动喂食开关指令处理函数
{
    if (OnOff == 1)
    {
        if (xSemaphoreGive(BinSemap_CloudOrderSync) == pdTRUE) //同步设置到触发处理
        {
            // printf("发送云端指令同步信号量成功\r\n");
        }
        enum_CloudOrder = cloud_order_feed;
    }
}
int CloudGetFeedTime(void) //上报给云端开始喂食时间处理函数
{
    return stru_CloudFeedByManualParam.FeedTrigDuration; //喂食时长
}

void CloudFeedByManualTimePro(int feedtime) //云端下发 喂食时间指令处理函数
{
    stru_CloudFeedByManualParam.FeedTrigDuration = feedtime; //喂食时长
}

void CloudLightByManualSwitchPro(bool OnOff) //云端下发 手动开灯指令处理函数
{
    if (OnOff == 1)
    {
        stru_CloudLightByManualParam.LightByManualSwitch = 1; //手动开灯标志位
    }
    else
    {
        stru_CloudLightByManualParam.LightByManualSwitch = 0; //手动开灯标志位
        /* code */
    }
    if (xSemaphoreGive(BinSemap_CloudOrderSync) == pdTRUE) //同步设置到触发处理
    {
        // printf("发送云端指令同步信号量成功\r\n");
    }
    enum_CloudOrder = cloud_order_light;
}

int CloudGetLightBright(void) //上报给云端开始亮度值处理函数
{
    return stru_CloudLightByManualParam.LightByManualBright; //亮度值
}

void CloudLightByManualBrightPro(int bright) //云端下发 手动开灯亮度指令处理函数
{
    stru_CloudLightByManualParam.LightByManualBright = bright; //亮度
    if (xSemaphoreGive(BinSemap_CloudOrderSync) == pdTRUE)     //同步设置到触发处理
    {
        // printf("发送云端指令同步信号量成功\r\n");
    }
    enum_CloudOrder = cloud_order_light;
}

void IndicatorMqttPro(OLEDDisplay_t *oled) // Mqtt连接状态指示灯处理函数
{
    static bool IndicatorIndex = 0;
    if (stru_DistributNetWindowParam.MqttNetConnect == 1) //如果连接成功  则常亮
    {
        gpio_set_level(INDICATOR_MQTT_GPIO_PIN, INDICATOR_MQTT_ON); //
    }
    else //连接失败  闪烁
    {
        if (get_counter(oled, indicator_mqtt_blink_timer) == 0) //计时时间到 定时刷新
        {
            register_counter(oled, indicator_mqtt_blink_timer, 5 * COUNT_100_MS);
            if (IndicatorIndex == 0)
            {
                IndicatorIndex = 1;
                gpio_set_level(INDICATOR_MQTT_GPIO_PIN, INDICATOR_MQTT_ON); //
            }
            else
            {
                IndicatorIndex = 0;
                gpio_set_level(INDICATOR_MQTT_GPIO_PIN, INDICATOR_MQTT_OFF); //
            }
        }
    }
}

void DistributNetworkWindowPro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //配网状态窗口显示函数
{
    wifi_config_t wifi_cfg = {0};
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        oled->DisplayStep++;
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");
        if (stru_DistributNetWindowParam.MqttNetConnect == 1)
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "已连接到Mqtt");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "当前Wifi和密码:");
            if (xQueuePeek(xqueue_WifiConfigParam, &wifi_cfg, (TickType_t)10))
            { // pcRxedMessage now points to the struct AMessage variable posted
                OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "%s", wifi_cfg.sta.ssid);
                OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, "%s", wifi_cfg.sta.password);
            }
        }
        else
        {
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 0, "未连接到Mqtt");
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 1, "配网AP和BleId:");

            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 2, "ApId:%s", CONFIG_LIGHT_PROVISIONING_SOFTAPCONFIG_SSID);
            OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, Normal, 16 * 0, 16 * 3, "BleId:%s", CONFIG_LIGHT_PROVISIONING_BLECONFIG_NAME);
        }

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        // oled->enum_DisplayWindow = Main_Window;
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}

void DistributNetworkWindow_2Pro(OLEDDisplay_t *oled, type_stru_KeyValue *stru_KeyValue) //配网状态窗口_2显示函数
{
    static uint8_t CursorIndex = 1;
    if (oled->DisplayStep == 0) //初始化赋值一次
    {
        CursorIndex = 1;
        oled->DisplayStep++;
    }

    if (GetPartOrFullScreenUpdateDisplay(Full_Screen_Update_Display) == pdTRUE) //如果需要整屏更新
    {
        OLEDDisplay_resetDisplay(oled);
        // ReverseCodeDisplayPro(oled, 16 * 1, 16 * 0, "鱼缸管理系统");

        SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
        SelectPartOrFullScreenUpdateDisplay(Part_2_Update_Display);
        SelectPartOrFullScreenUpdateDisplay(Part_3_Update_Display);
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_1_Update_Display) == pdTRUE) //局部更新
    {
        OledChineseEnglishMixedPrint(oled, clear_screen, CursorIndex == 0 ? ShowSquareBox : Normal, 16 * 0, 16 * 1, "复位并重新配网");
        OledChineseEnglishMixedPrint(oled, clear_screen, CursorIndex == 1 ? ShowSquareBox : Normal, 16 * 0, 16 * 2, "返回主窗口");
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_2_Update_Display) == pdTRUE) //局部更新
    {
    }
    if (GetPartOrFullScreenUpdateDisplay(Part_3_Update_Display) == pdTRUE) //局部更新
    {
    }

    // ESP_LOGI(TAG, "enum_MainWindowCursorPosition= %d \n", enum_MainWindowCursorPosition);
    if (stru_KeyValue->enum_Rotation == key_not_rotat && stru_KeyValue->enum_KeyPress == key_not_press) //只有按键按下才处理
    {
        return;
    }

    if (stru_KeyValue->enum_KeyPress == key_short_press)
    {
        if (CursorIndex == 0)
        {
            // OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, ShowSquareBox, 16 * 0, 16 * 1, "复位并重新配网");
            ESP_ERROR_CHECK(esp_wifi_disconnect()); //复位
            esp_qcloud_storage_erase(CONFIG_QCLOUD_NVS_NAMESPACE);
            abort();
            ESP_LOGI(TAG, "esp_wifi_disconnect  断开WiFi");
            ESP_LOGI(TAG, "esp_qcloud_storage_erase  CONFIG_QCLOUD_NVS_NAMESPACE  清除wifi配置信息");
            ESP_LOGI(TAG, "abort()");
        }
        else
        {
            // OledChineseEnglishMixedPrint(oled, dont_need_clear_screen, ShowSquareBox, 16 * 0, 16 * 2, "返回主窗口");
            oled->enum_DisplayWindow = Main_Window;
        }
        stru_KeyValue->enum_KeyPress = key_not_press;
    }
    else
    {
        switch (stru_KeyValue->enum_Rotation)
        {
        case key_up_right:
            CursorIndex = !CursorIndex;
            SelectPartOrFullScreenUpdateDisplay(Part_1_Update_Display); //局部更新显示
            break;
        case key_down_left:
            break;
        default:
            break;
        }
        stru_KeyValue->enum_Rotation = key_not_rotat;
    }
}
