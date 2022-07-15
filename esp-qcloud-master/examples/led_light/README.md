
@[TOC](鱼缸管理系统)

# 欢迎浏览本人项目

你好！ 关于本项目,是本人一时兴趣使然创作,因为平时比较喜欢养鱼,所以想试着做一个鱼缸管理系统,水平有限,用的是ESP32开发模块,配合一些外围的元器件实现一些功能.

## 实现功能
- oled人机交互界面:通过旋转编码器的左右旋转及按下可与oled界面进行交互
- 自动灯光:根据外界环境亮度自动调节灯光亮度(占空比)
- 定时定量投喂:3组不同的喂食时间,并且投喂的量可设置
- 温度监测:将水温度控制在一个范围内,设置其上下限(如26.3℃~~~29.6℃之间)
- 水泵控制:可设置开启水泵的时间段
- 通过腾讯连连小程序配网、控制

## 硬件资源
- ESP32_Dev开发模块
- EC11旋转编码器模块
- OLED128x64模块
- DS1302时钟模块
- EEPROM_24C256模块
- DS18B20温度传感器
- G90小舵机
- 5V继电器 x 2

## 软件编译环境
- 乐鑫官方ESPIDF编译环境:cd到Tencent_qcloud-master\esp-qcloud-master\examples\led_light进行编译

## 实物演示视频链接与图片

视频链接: [link](https://www.csdn.net/).

[image](https://github.com/MattDamon1/ESP32FishTankManagementSystem/blob/main/esp-qcloud-master/examples/led_light/public/image/%E5%AE%9E%E7%89%A9%E5%9B%BE.png)
