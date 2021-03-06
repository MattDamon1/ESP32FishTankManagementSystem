#ifndef OLED_CHINESE_SIMPLE_WORD_H
#define OLED_CHINESE_SIMPLE_WORD_H

typedef struct
{
    unsigned int Index;    //汉字内码
    unsigned char Msk[32]; //字模
} Typedef_Stru_CN16_Msk;

const Typedef_Stru_CN16_Msk CN16_Msk[] = {
    {'鱼',
     {0x40, 0x40, 0x20, 0x40, 0xF0, 0x4F, 0x28, 0x49, 0x24, 0x49, 0x27, 0x49, 0x24, 0x49, 0xE4, 0x4F,
      0x24, 0x49, 0x34, 0x49, 0x2C, 0x49, 0x20, 0x49, 0xE0, 0x4F, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00}},
    {'缸',
     {0x50, 0x00, 0x48, 0x3E, 0x47, 0x20, 0x44, 0x20, 0xFC, 0x3F, 0x44, 0x10, 0x44, 0x10, 0x44, 0x7E,
      0x00, 0x20, 0x04, 0x20, 0x04, 0x20, 0xFC, 0x3F, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x00, 0x00}},

    {'管',
     {0x88, 0x00, 0x64, 0x00, 0x23, 0x00, 0xA2, 0xFF, 0xA6, 0x4A, 0xAA, 0x4A, 0xA2, 0x4A, 0xBA, 0x4A,
      0xA4, 0x4A, 0xA3, 0x4A, 0xA2, 0x4B, 0x26, 0xF8, 0x2A, 0x00, 0xA2, 0x00, 0x62, 0x00, 0x00, 0x00}},

    {'理',
     {0x04, 0x20, 0x84, 0x60, 0x84, 0x20, 0xFC, 0x1F, 0x84, 0x10, 0x84, 0x10, 0x00, 0x40, 0xFE, 0x44,
      0x92, 0x44, 0x92, 0x44, 0xFE, 0x7F, 0x92, 0x44, 0x92, 0x44, 0xFE, 0x44, 0x00, 0x40, 0x00, 0x00}},

    {'系',
     {0x00, 0x00, 0x00, 0x42, 0x22, 0x22, 0x32, 0x13, 0x2A, 0x0B, 0xA6, 0x42, 0xA2, 0x82, 0x62, 0x7E,
      0x21, 0x02, 0x11, 0x02, 0x09, 0x0A, 0x81, 0x12, 0x01, 0x23, 0x00, 0x46, 0x00, 0x00, 0x00, 0x00}},

    {'统',
     {0x20, 0x22, 0x30, 0x67, 0xAC, 0x22, 0x63, 0x12, 0x30, 0x12, 0x00, 0x80, 0x88, 0x40, 0xC8, 0x30,
      0xA8, 0x0F, 0x99, 0x00, 0x8E, 0x00, 0x88, 0x3F, 0xA8, 0x40, 0xC8, 0x40, 0x88, 0x71, 0x00, 0x00}},

    {'时',
     {0x00, 0x00, 0xFC, 0x3F, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0xFC, 0x3F, 0x00, 0x00, 0x10, 0x00,
      0x10, 0x01, 0x10, 0x06, 0x10, 0x40, 0x10, 0x80, 0xFF, 0x7F, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00}},

    {'间',
     {0x00, 0x00, 0xF8, 0xFF, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x12, 0x11, 0x12, 0x11,
      0x12, 0x11, 0xF2, 0x1F, 0x02, 0x00, 0x02, 0x40, 0x02, 0x80, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00}},

    {'水',
     {0x00, 0x20, 0x20, 0x10, 0x20, 0x08, 0x20, 0x06, 0xA0, 0x01, 0x60, 0x40, 0x00, 0x80, 0xFF, 0x7F,
      0x60, 0x00, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x18, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00}},

    {'温',
     {0x10, 0x04, 0x60, 0x04, 0x02, 0x7E, 0x8C, 0x01, 0x00, 0x40, 0x00, 0x7E, 0xFE, 0x42, 0x92, 0x42,
      0x92, 0x7E, 0x92, 0x42, 0x92, 0x7E, 0x92, 0x42, 0xFE, 0x42, 0x00, 0x7E, 0x00, 0x40, 0x00, 0x00}},

    {'按',
     {0x10, 0x02, 0x10, 0x42, 0x10, 0x81, 0xFF, 0x7F, 0x90, 0x00, 0x20, 0x00, 0x98, 0x80, 0x88, 0x84,
      0x88, 0x4B, 0xE9, 0x28, 0x8E, 0x10, 0x88, 0x28, 0x88, 0x47, 0xA8, 0x80, 0x98, 0x00, 0x00, 0x00}},

    {'下',
     {0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0xFE, 0xFF, 0x02, 0x00,
      0x02, 0x00, 0x42, 0x00, 0x82, 0x00, 0x02, 0x01, 0x02, 0x06, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00}},

    {'进',
     {0x40, 0x00, 0x40, 0x40, 0x42, 0x20, 0xCC, 0x1F, 0x00, 0x20, 0x80, 0x40, 0x88, 0x50, 0x88, 0x4C,
      0xFF, 0x43, 0x88, 0x40, 0x88, 0x40, 0xFF, 0x5F, 0x88, 0x40, 0x88, 0x40, 0x80, 0x40, 0x00, 0x00}},

    {'入',
     {0x00, 0x80, 0x00, 0x40, 0x00, 0x20, 0x00, 0x10, 0x00, 0x0C, 0x01, 0x03, 0xE2, 0x00, 0x1C, 0x00,
      0xE0, 0x00, 0x00, 0x03, 0x00, 0x0C, 0x00, 0x30, 0x00, 0x40, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00}},

    {'设',
     {0x40, 0x00, 0x40, 0x00, 0x42, 0x00, 0xCC, 0x3F, 0x00, 0x90, 0x40, 0x88, 0xA0, 0x40, 0x9E, 0x43,
      0x82, 0x2C, 0x82, 0x10, 0x82, 0x28, 0x9E, 0x46, 0xA0, 0x41, 0x20, 0x80, 0x20, 0x80, 0x00, 0x00}},

    {'置',
     {0x00, 0x40, 0x17, 0x40, 0x15, 0x40, 0xD5, 0x7F, 0x55, 0x55, 0x57, 0x55, 0x55, 0x55, 0x7D, 0x55,
      0x55, 0x55, 0x57, 0x55, 0x55, 0x55, 0xD5, 0x7F, 0x15, 0x40, 0x17, 0x40, 0x00, 0x40, 0x00, 0x00}},
    {'喂',
     {0x00, 0x00, 0xFC, 0x0F, 0x04, 0x04, 0x04, 0x04, 0xFC, 0x0F, 0x00, 0x02, 0xFE, 0xFE, 0x92, 0x42,
      0x92, 0x22, 0xFE, 0x0E, 0x92, 0x12, 0x92, 0x22, 0xFE, 0x52, 0x00, 0x8A, 0x00, 0x82, 0x00, 0x00}},

    {'食',
     {0x20, 0x00, 0x20, 0x00, 0x10, 0x00, 0xF0, 0xFF, 0xA8, 0x82, 0xA4, 0x42, 0xAA, 0x22, 0xB1, 0x0A,
      0xA2, 0x0A, 0xA4, 0x12, 0xA8, 0x12, 0xF0, 0x2B, 0x10, 0x44, 0x20, 0x80, 0x20, 0x00, 0x00, 0x00}},
    {'灯',
     {0x80, 0x80, 0x70, 0x60, 0x00, 0x18, 0xFF, 0x07, 0x20, 0x08, 0x10, 0x30, 0x04, 0x00, 0x04, 0x00,
      0x04, 0x40, 0x04, 0x80, 0xFC, 0x7F, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00}},
    {'光',
     {0x40, 0x80, 0x40, 0x80, 0x42, 0x40, 0x44, 0x20, 0x58, 0x18, 0xC0, 0x07, 0x40, 0x00, 0x7F, 0x00,
      0x40, 0x00, 0xC0, 0x3F, 0x50, 0x40, 0x48, 0x40, 0x46, 0x40, 0x40, 0x40, 0x40, 0x78, 0x00, 0x00}},
    {'退',
     {0x40, 0x00, 0x40, 0x40, 0x42, 0x20, 0xCC, 0x1F, 0x00, 0x20, 0x00, 0x40, 0xFF, 0x5F, 0x49, 0x48,
      0x49, 0x44, 0xC9, 0x40, 0x49, 0x41, 0x49, 0x42, 0x7F, 0x45, 0x80, 0x58, 0x00, 0x40, 0x00, 0x00}},

    {'出',
     {0x00, 0x00, 0x00, 0x7C, 0x7C, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xFF, 0x7F,
      0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xFC, 0x40, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00}},
    {'星',
     {0x00, 0x00, 0x00, 0x44, 0x00, 0x42, 0xBE, 0x49, 0x2A, 0x49, 0x2A, 0x49, 0x2A, 0x49, 0xEA, 0x7F,
      0x2A, 0x49, 0x2A, 0x49, 0x2A, 0x49, 0x3E, 0x49, 0x00, 0x41, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00}},

    {'期',
     {0x00, 0x88, 0x04, 0x48, 0xFF, 0x2F, 0x24, 0x09, 0x24, 0x09, 0x24, 0x19, 0xFF, 0xAF, 0x04, 0x48,
      0x00, 0x30, 0xFE, 0x0F, 0x22, 0x02, 0x22, 0x42, 0x22, 0x82, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00}},
    {'一',
     {0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00,
      0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00}},

    {'二',
     {0x00, 0x10, 0x00, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10,
      0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00}},

    {'三',
     {0x00, 0x20, 0x04, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20,
      0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x04, 0x20, 0x00, 0x20, 0x00, 0x00}},

    {'四',
     {0x00, 0x00, 0xFC, 0x7F, 0x04, 0x28, 0x04, 0x24, 0x04, 0x23, 0xFC, 0x20, 0x04, 0x20, 0x04, 0x20,
      0x04, 0x20, 0xFC, 0x21, 0x04, 0x22, 0x04, 0x22, 0x04, 0x22, 0xFC, 0x7F, 0x00, 0x00, 0x00, 0x00}},

    {'五',
     {0x00, 0x40, 0x02, 0x40, 0x42, 0x40, 0x42, 0x40, 0x42, 0x78, 0xC2, 0x47, 0x7E, 0x40, 0x42, 0x40,
      0x42, 0x40, 0x42, 0x40, 0x42, 0x40, 0xC2, 0x7F, 0x02, 0x40, 0x02, 0x40, 0x00, 0x40, 0x00, 0x00}},

    {'六',
     {0x20, 0x00, 0x20, 0x40, 0x20, 0x20, 0x20, 0x10, 0x20, 0x0C, 0x20, 0x03, 0x21, 0x00, 0x22, 0x00,
      0x2C, 0x00, 0x20, 0x01, 0x20, 0x02, 0x20, 0x04, 0x20, 0x18, 0x20, 0x60, 0x20, 0x00, 0x00, 0x00}},

    {'日',
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x82, 0x40, 0x82, 0x40, 0x82, 0x40, 0x82, 0x40,
      0x82, 0x40, 0x82, 0x40, 0x82, 0x40, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {'完',
     {0x10, 0x00, 0x0C, 0x81, 0x04, 0x81, 0x24, 0x41, 0x24, 0x31, 0x24, 0x0F, 0x25, 0x01, 0x26, 0x01,
      0x24, 0x01, 0x24, 0x7F, 0x24, 0x81, 0x24, 0x81, 0x04, 0x81, 0x14, 0xF1, 0x0C, 0x00, 0x00, 0x00}},

    {'成',
     {0x00, 0x80, 0x00, 0x60, 0xF8, 0x1F, 0x88, 0x00, 0x88, 0x10, 0x88, 0x20, 0x88, 0x1F, 0x08, 0x80,
      0x08, 0x40, 0xFF, 0x21, 0x08, 0x16, 0x09, 0x18, 0x0A, 0x26, 0xC8, 0x41, 0x08, 0xF8, 0x00, 0x00}},

    {'并',
     {0x00, 0x02, 0x10, 0x82, 0x10, 0x42, 0x11, 0x32, 0xF6, 0x0F, 0x10, 0x02, 0x10, 0x02, 0x10, 0x02,
      0x10, 0x02, 0x10, 0x02, 0xF4, 0xFF, 0x13, 0x02, 0x10, 0x02, 0x10, 0x02, 0x00, 0x02, 0x00, 0x00}},

    {'退',
     {0x40, 0x00, 0x40, 0x40, 0x42, 0x20, 0xCC, 0x1F, 0x00, 0x20, 0x00, 0x40, 0xFF, 0x5F, 0x49, 0x48,
      0x49, 0x44, 0xC9, 0x40, 0x49, 0x41, 0x49, 0x42, 0x7F, 0x45, 0x80, 0x58, 0x00, 0x40, 0x00, 0x00}},

    {'℃',
     {0x06, 0x00, 0x09, 0x00, 0x09, 0x00, 0xE6, 0x07, 0xF8, 0x1F, 0x0C, 0x30, 0x04, 0x20, 0x02, 0x40,
      0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x04, 0x20, 0x1E, 0x10, 0x00, 0x00, 0x00, 0x00}},

    {'功',
     {0x08, 0x10, 0x08, 0x30, 0x08, 0x10, 0xF8, 0x1F, 0x08, 0x08, 0x08, 0x88, 0x08, 0x48, 0x10, 0x30,
      0x10, 0x0E, 0xFF, 0x01, 0x10, 0x40, 0x10, 0x80, 0x10, 0x40, 0xF0, 0x3F, 0x00, 0x00, 0x00, 0x00}},

    {'能',
     {0x08, 0x00, 0xCC, 0xFF, 0x4A, 0x12, 0x49, 0x12, 0x48, 0x52, 0x4A, 0x92, 0xCC, 0x7F, 0x18, 0x00,
      0x00, 0x00, 0x7F, 0x7E, 0x88, 0x88, 0x88, 0x88, 0x84, 0x84, 0x82, 0x82, 0xE0, 0xE0, 0x00, 0x00}},

    {'使',
     {0x80, 0x00, 0x60, 0x00, 0xF8, 0xFF, 0x07, 0x00, 0x04, 0x80, 0xE4, 0x81, 0x24, 0x45, 0x24, 0x29,
      0x24, 0x11, 0xFF, 0x2F, 0x24, 0x41, 0x24, 0x41, 0x24, 0x81, 0xE4, 0x81, 0x04, 0x80, 0x00, 0x00}},
    {'度',
     {0x00, 0x40, 0x00, 0x30, 0xFC, 0x8F, 0x24, 0x80, 0x24, 0x84, 0x24, 0x4C, 0xFC, 0x55, 0x25, 0x25,
      0x26, 0x25, 0x24, 0x25, 0xFC, 0x55, 0x24, 0x4C, 0x24, 0x80, 0x24, 0x80, 0x04, 0x80, 0x00, 0x00}},

    {'上',
     {0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0xFF, 0x7F, 0x40, 0x40,
      0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00}},

    {'限',
     {0x00, 0x00, 0xFE, 0xFF, 0x22, 0x04, 0x5A, 0x08, 0x86, 0x07, 0x00, 0x00, 0xFE, 0xFF, 0x92, 0x40,
      0x92, 0x20, 0x92, 0x03, 0x92, 0x0C, 0x92, 0x14, 0xFE, 0x22, 0x00, 0x41, 0x00, 0x40, 0x00, 0x00}},
    {'开',
     {0x80, 0x00, 0x82, 0x80, 0x82, 0x40, 0x82, 0x30, 0xFE, 0x0F, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00,
      0x82, 0x00, 0x82, 0x00, 0xFE, 0xFF, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x80, 0x00, 0x00, 0x00}},

    {'启',
     {0x00, 0x40, 0x00, 0x20, 0x00, 0x18, 0xFC, 0x07, 0x44, 0x00, 0x44, 0xFC, 0x44, 0x44, 0x45, 0x44,
      0x46, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x7C, 0xFC, 0x00, 0x00, 0x00, 0x00}},

    {'关',
     {0x00, 0x81, 0x00, 0x81, 0x10, 0x41, 0x11, 0x41, 0x16, 0x21, 0x10, 0x11, 0x10, 0x0D, 0xF0, 0x03,
      0x10, 0x0D, 0x10, 0x11, 0x14, 0x21, 0x13, 0x41, 0x10, 0x41, 0x00, 0x81, 0x00, 0x81, 0x00, 0x00}},

    {'闭',
     {0x00, 0x00, 0xF8, 0xFF, 0x01, 0x00, 0x22, 0x08, 0x20, 0x04, 0x22, 0x02, 0x22, 0x11, 0xA2, 0x20,
      0xFA, 0x1F, 0x22, 0x00, 0x22, 0x00, 0x22, 0x40, 0x02, 0x80, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00}},

    {'详',
     {0x40, 0x00, 0x40, 0x00, 0x42, 0x00, 0xCC, 0x7F, 0x00, 0x20, 0x10, 0x14, 0x91, 0x04, 0x96, 0x04,
      0x90, 0x04, 0xF0, 0xFF, 0x90, 0x04, 0x94, 0x04, 0x93, 0x04, 0x10, 0x04, 0x00, 0x04, 0x00, 0x00}},

    {'情',
     {0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x10, 0x00, 0x64, 0x00, 0x54, 0xFF, 0x54, 0x15,
      0x54, 0x15, 0x7F, 0x15, 0x54, 0x55, 0x54, 0x95, 0x54, 0x7F, 0x44, 0x00, 0x40, 0x00, 0x00, 0x00}},

    {'返',
     {0x40, 0x00, 0x40, 0x40, 0x42, 0x20, 0xCC, 0x1F, 0x00, 0x20, 0x00, 0x58, 0xFC, 0x47, 0x24, 0x50,
      0xA4, 0x48, 0x24, 0x45, 0x22, 0x42, 0x22, 0x45, 0xA3, 0x48, 0x62, 0x50, 0x00, 0x40, 0x00, 0x00}},

    {'回',
     {0x00, 0x00, 0x00, 0x00, 0xFE, 0x7F, 0x02, 0x20, 0x02, 0x20, 0xF2, 0x27, 0x12, 0x24, 0x12, 0x24,
      0x12, 0x24, 0xF2, 0x27, 0x02, 0x20, 0x02, 0x20, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    {'主',
     {0x00, 0x40, 0x08, 0x40, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x09, 0x41, 0xFA, 0x7F,
      0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x40, 0x00, 0x40, 0x00, 0x00}},

    {'窗',
     {0x10, 0x00, 0x0C, 0x00, 0xC4, 0xFF, 0x54, 0x40, 0x4C, 0x44, 0x64, 0x62, 0xD5, 0x55, 0x46, 0x49,
      0x44, 0x55, 0x44, 0x43, 0x4C, 0x40, 0x54, 0x40, 0xC4, 0xFF, 0x14, 0x00, 0x0C, 0x00, 0x00, 0x00}},

    {'口',
     {0x00, 0x00, 0x00, 0x00, 0xFC, 0x7F, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
      0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0xFC, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {'触',
     {0x10, 0x80, 0xEC, 0x7F, 0x27, 0x09, 0xF4, 0x3F, 0x2C, 0x89, 0xE0, 0xFF, 0x00, 0x20, 0xF8, 0x63,
      0x08, 0x21, 0x08, 0x21, 0xFF, 0x3F, 0x08, 0x21, 0x08, 0x29, 0xF8, 0x33, 0x00, 0x60, 0x00, 0x00}},

    {'发',
     {0x00, 0x00, 0x00, 0x20, 0x18, 0x10, 0x16, 0x8C, 0x10, 0x83, 0xD0, 0x80, 0xB8, 0x41, 0x97, 0x46,
      0x90, 0x28, 0x90, 0x10, 0x90, 0x28, 0x92, 0x44, 0x94, 0x43, 0x10, 0x80, 0x00, 0x80, 0x00, 0x00}},

    {'动',
     {0x40, 0x10, 0x44, 0x3C, 0xC4, 0x13, 0x44, 0x10, 0x44, 0x14, 0x44, 0xB8, 0x40, 0x40, 0x10, 0x30,
      0x10, 0x0E, 0xFF, 0x01, 0x10, 0x40, 0x10, 0x80, 0x10, 0x40, 0xF0, 0x3F, 0x00, 0x00, 0x00, 0x00}},

    {'作',
     {0x00, 0x01, 0x80, 0x00, 0x60, 0x00, 0xF8, 0xFF, 0x07, 0x00, 0x40, 0x00, 0x30, 0x00, 0x0F, 0x00,
      0xF8, 0xFF, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00}},

    {'倥', //"倥"单人旁的倥代表显示一个16x16的空格
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {'继',
     {0x20, 0x22, 0x30, 0x67, 0xAC, 0x22, 0x63, 0x12, 0x10, 0x12, 0x00, 0x00, 0xFE, 0x7F, 0x48, 0x44,
      0x50, 0x42, 0x40, 0x41, 0xFF, 0x7F, 0x40, 0x41, 0x50, 0x42, 0x48, 0x44, 0x00, 0x40, 0x00, 0x00}},

    {'电',
     {0x00, 0x00, 0x00, 0x00, 0xF8, 0x1F, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0x88, 0x08, 0xFF, 0x7F,
      0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0xF8, 0x9F, 0x00, 0x80, 0x00, 0xF0, 0x00, 0x00}},

    {'器',
     {0x80, 0x08, 0x80, 0x08, 0x9E, 0xF4, 0x92, 0x94, 0x92, 0x92, 0x92, 0x92, 0x9E, 0xF1, 0xE0, 0x00,
      0x80, 0x01, 0x9E, 0xF2, 0xB2, 0x92, 0xD2, 0x94, 0x92, 0x94, 0x9E, 0xF8, 0x80, 0x08, 0x00, 0x00}},

    {'合',
     {0x40, 0x00, 0x40, 0x00, 0x20, 0x00, 0x20, 0xFE, 0x50, 0x42, 0x48, 0x42, 0x44, 0x42, 0x43, 0x42,
      0x44, 0x42, 0x48, 0x42, 0x50, 0x42, 0x20, 0xFE, 0x20, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00}},
    {'断',
     {0x00, 0x00, 0xFC, 0x3F, 0x20, 0x22, 0x24, 0x21, 0xA8, 0x20, 0xFF, 0x2F, 0xA8, 0x20, 0x24, 0xA3,
      0x00, 0x60, 0xFC, 0x1F, 0x44, 0x00, 0x44, 0x00, 0xC4, 0xFF, 0x42, 0x00, 0x40, 0x00, 0x00, 0x00}},

    {'开',
     {0x80, 0x00, 0x82, 0x80, 0x82, 0x40, 0x82, 0x30, 0xFE, 0x0F, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00,
      0x82, 0x00, 0x82, 0x00, 0xFE, 0xFF, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x80, 0x00, 0x00, 0x00}},
    {'加',
     {0x10, 0x80, 0x10, 0x40, 0x10, 0x30, 0xFF, 0x0F, 0x10, 0x40, 0x10, 0x80, 0xF0, 0x7F, 0x00, 0x00,
      0x00, 0x00, 0xF8, 0x7F, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0xF8, 0x7F, 0x00, 0x00, 0x00, 0x00}},

    {'热',
     {0x08, 0x81, 0x08, 0x65, 0x88, 0x08, 0xFF, 0x07, 0x48, 0x20, 0x48, 0xC0, 0x00, 0x08, 0x08, 0x04,
      0x48, 0x23, 0xFF, 0xC0, 0x08, 0x03, 0x08, 0x00, 0xF8, 0x23, 0x00, 0xC4, 0x00, 0x0F, 0x00, 0x00}},

    {'棒',
     {0x10, 0x04, 0x10, 0x03, 0xD0, 0x00, 0xFF, 0xFF, 0x90, 0x00, 0x10, 0x01, 0x44, 0x12, 0x54, 0x11,
      0xD4, 0x14, 0x74, 0x14, 0x5F, 0xFF, 0x54, 0x14, 0xD4, 0x14, 0x54, 0x11, 0x44, 0x12, 0x00, 0x00}},
    {'第',
     {0x08, 0x40, 0x04, 0x40, 0x93, 0x47, 0x92, 0x24, 0x96, 0x24, 0x9A, 0x14, 0x92, 0x0C, 0xFA, 0xFF,
      0x94, 0x04, 0x93, 0x04, 0x92, 0x24, 0x96, 0x44, 0xFA, 0x24, 0x02, 0x1C, 0x02, 0x00, 0x00, 0x00}},

    {'组',
     {0x20, 0x22, 0x30, 0x67, 0xAC, 0x22, 0x63, 0x12, 0x30, 0x12, 0x00, 0x40, 0x00, 0x40, 0xFE, 0x7F,
      0x22, 0x42, 0x22, 0x42, 0x22, 0x42, 0x22, 0x42, 0xFE, 0x7F, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00}},

    {'长',
     {0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xFF, 0xFF, 0x80, 0x40, 0x80, 0x21, 0xA0, 0x12,
      0x90, 0x04, 0x88, 0x08, 0x84, 0x10, 0x82, 0x20, 0x80, 0x20, 0x80, 0x40, 0x80, 0x40, 0x00, 0x00}},
    {'始',
     {0x10, 0x40, 0x10, 0x22, 0xF0, 0x15, 0x1F, 0x08, 0x10, 0x16, 0xF0, 0x21, 0x00, 0x00, 0x40, 0x00,
      0xE0, 0xFE, 0x58, 0x42, 0x47, 0x42, 0x40, 0x42, 0x50, 0x42, 0x60, 0xFE, 0xC0, 0x00, 0x00, 0x00}},
    {'模',
     {0x10, 0x04, 0x10, 0x03, 0xD0, 0x00, 0xFF, 0xFF, 0x90, 0x00, 0x14, 0x89, 0xE4, 0x4B, 0xAF, 0x2A,
      0xA4, 0x1A, 0xA4, 0x0E, 0xA4, 0x1A, 0xAF, 0x2A, 0xE4, 0x4B, 0x04, 0x88, 0x00, 0x80, 0x00, 0x00}},

    {'式',
     {0x10, 0x00, 0x10, 0x20, 0x90, 0x60, 0x90, 0x20, 0x90, 0x3F, 0x90, 0x10, 0x90, 0x10, 0x10, 0x10,
      0x10, 0x00, 0xFF, 0x03, 0x10, 0x0C, 0x10, 0x10, 0x11, 0x20, 0x16, 0x40, 0x10, 0xF8, 0x00, 0x00}},
    {'区',
     {0x00, 0x00, 0xFE, 0x7F, 0x02, 0x40, 0x02, 0x40, 0x02, 0x48, 0x12, 0x44, 0x22, 0x42, 0x42, 0x41,
      0x82, 0x40, 0x42, 0x41, 0x22, 0x42, 0x1A, 0x4C, 0x02, 0x40, 0x02, 0x40, 0x00, 0x40, 0x00, 0x00}},
    {'照',
     {0x00, 0x80, 0xFE, 0x6F, 0x42, 0x08, 0x42, 0x08, 0x42, 0x28, 0xFE, 0xCF, 0x00, 0x00, 0x42, 0x00,
      0xA2, 0x2F, 0x9E, 0xC8, 0x82, 0x08, 0xA2, 0x08, 0xC2, 0x28, 0xBE, 0xCF, 0x00, 0x00, 0x00, 0x00}},

    {'段',
     {0x00, 0x10, 0x00, 0x10, 0xFE, 0xFF, 0x22, 0x11, 0x22, 0x09, 0x21, 0x09, 0x01, 0x80, 0x40, 0x81,
      0x3E, 0x47, 0x02, 0x29, 0x02, 0x11, 0x02, 0x29, 0x1E, 0x45, 0x20, 0x83, 0x20, 0x80, 0x00, 0x00}},
    {'亮',
     {0x00, 0x84, 0x04, 0x83, 0x04, 0x41, 0x74, 0x21, 0x54, 0x1D, 0x54, 0x05, 0x55, 0x05, 0x56, 0x05,
      0x54, 0x05, 0x54, 0x05, 0x54, 0x7D, 0x74, 0x81, 0x04, 0x81, 0x04, 0x85, 0x00, 0xE3, 0x00, 0x00}},
    {'傍',
     {0x80, 0x00, 0x60, 0x00, 0xF8, 0xFF, 0x07, 0x00, 0x80, 0x00, 0x64, 0x82, 0x2C, 0x62, 0x34, 0x1E,
      0xA5, 0x0A, 0x26, 0x0B, 0x24, 0x4A, 0x34, 0x8A, 0x2C, 0x7A, 0xA4, 0x02, 0x60, 0x00, 0x00, 0x00}},

    {'晚',
     {0x00, 0x00, 0xFC, 0x3F, 0x84, 0x10, 0x84, 0x10, 0xFC, 0x3F, 0x10, 0x80, 0xE8, 0x43, 0x24, 0x32,
      0x27, 0x0E, 0xE4, 0x03, 0x24, 0x3E, 0x34, 0x42, 0x2C, 0x42, 0xE0, 0x43, 0x00, 0x70, 0x00, 0x00}},

    {'夜',
     {0x04, 0x02, 0x04, 0x01, 0x84, 0x00, 0xE4, 0xFF, 0x1C, 0x00, 0x04, 0x82, 0x05, 0x81, 0xC6, 0x46,
      0x3C, 0x28, 0xA4, 0x10, 0x24, 0x29, 0x24, 0x46, 0xE4, 0x41, 0x04, 0x80, 0x04, 0x80, 0x00, 0x00}},

    {'佰', //用'佰'代表打印一个%符号
     {0xF0, 0x00, 0x08, 0x31, 0xF0, 0x0C, 0x80, 0x03, 0x60, 0x1E, 0x18, 0x21, 0x00, 0x1E, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {'环',
     {0x04, 0x20, 0x84, 0x60, 0x84, 0x20, 0xFC, 0x1F, 0x84, 0x10, 0x84, 0x10, 0x00, 0x04, 0x04, 0x02,
      0x04, 0x01, 0x84, 0x00, 0xE4, 0xFF, 0x1C, 0x00, 0x84, 0x00, 0x04, 0x01, 0x04, 0x06, 0x00, 0x00}},

    {'境',
     {0x10, 0x10, 0x10, 0x30, 0xFF, 0x1F, 0x10, 0x08, 0x10, 0x88, 0x20, 0x80, 0xA4, 0x4F, 0xAC, 0x3A,
      0xB5, 0x0A, 0xA6, 0x0A, 0xB4, 0x7A, 0xAC, 0x8A, 0xA4, 0x8F, 0x20, 0x80, 0x20, 0xE0, 0x00, 0x00}},
    {'状',
     {0x00, 0x04, 0x08, 0x04, 0x30, 0x02, 0x00, 0x01, 0xFF, 0xFF, 0x20, 0x80, 0x20, 0x40, 0x20, 0x30,
      0x20, 0x0E, 0xFF, 0x01, 0x20, 0x06, 0x20, 0x18, 0x22, 0x20, 0x2C, 0x40, 0x20, 0x80, 0x00, 0x00}},
    {'态',
     {0x00, 0x41, 0x04, 0x39, 0x84, 0x00, 0x84, 0x00, 0x44, 0x3C, 0x24, 0x40, 0x54, 0x40, 0x8F, 0x42,
      0x14, 0x4C, 0x24, 0x40, 0x44, 0x40, 0x84, 0x70, 0x84, 0x04, 0x04, 0x09, 0x00, 0x31, 0x00, 0x00}},
    {'当',
     {0x00, 0x00, 0x40, 0x40, 0x42, 0x44, 0x44, 0x44, 0x58, 0x44, 0x40, 0x44, 0x40, 0x44, 0x7F, 0x44,
      0x40, 0x44, 0x40, 0x44, 0x50, 0x44, 0x48, 0x44, 0xC6, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    {'前',
     {0x08, 0x00, 0x08, 0x00, 0xE8, 0xFF, 0x29, 0x09, 0x2E, 0x49, 0x28, 0x89, 0xE8, 0x7F, 0x08, 0x00,
      0x08, 0x00, 0xC8, 0x0F, 0x0C, 0x40, 0x0B, 0x80, 0xE8, 0x7F, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00}},
    {'为',
     {0x00, 0x80, 0x20, 0x40, 0x22, 0x20, 0x2C, 0x10, 0x20, 0x08, 0x20, 0x06, 0xE0, 0x01, 0x3F, 0x00,
      0x20, 0x01, 0x20, 0x46, 0x20, 0x80, 0x20, 0x40, 0xE0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    {'次',
     {0x00, 0x02, 0x02, 0x02, 0x0C, 0x7E, 0x80, 0x01, 0x60, 0x80, 0x80, 0x40, 0x40, 0x20, 0x30, 0x18,
      0x0F, 0x06, 0xC8, 0x01, 0x08, 0x06, 0x08, 0x18, 0x28, 0x20, 0x18, 0x40, 0x00, 0x80, 0x00, 0x00}},
    {'过',
     {0x40, 0x00, 0x40, 0x40, 0x42, 0x20, 0xCC, 0x1F, 0x00, 0x20, 0x08, 0x40, 0x48, 0x40, 0x88, 0x41,
      0x08, 0x40, 0x08, 0x48, 0x08, 0x50, 0xFF, 0x4F, 0x08, 0x40, 0x08, 0x40, 0x08, 0x40, 0x00, 0x00}},

    {'滤',
     {0x10, 0x04, 0x60, 0x04, 0x02, 0x7E, 0x8C, 0x81, 0x00, 0x60, 0xF8, 0x1F, 0x48, 0x80, 0x48, 0x70,
      0x48, 0x00, 0xFF, 0x78, 0x2A, 0x83, 0x2A, 0x8D, 0x0A, 0xC1, 0xCA, 0x09, 0x18, 0x70, 0x00, 0x00}},

    {'泵',
     {0x42, 0x40, 0x42, 0x44, 0x22, 0x24, 0x12, 0x14, 0xFA, 0x0C, 0x96, 0x44, 0x92, 0x80, 0x92, 0x7F,
      0x92, 0x04, 0x92, 0x08, 0x92, 0x10, 0xF2, 0x28, 0x02, 0x24, 0x02, 0x42, 0x02, 0x40, 0x00, 0x00}},
    {'演',
     {0x10, 0x04, 0x60, 0x04, 0x02, 0x7E, 0x8C, 0x01, 0x00, 0x00, 0x0C, 0x80, 0xA4, 0x4F, 0xA4, 0x2A,
      0xA5, 0x0A, 0xE6, 0x0F, 0xA4, 0x0A, 0xA4, 0x2A, 0xA4, 0x4F, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x00}},

    {'示',
     {0x40, 0x20, 0x40, 0x10, 0x42, 0x08, 0x42, 0x06, 0x42, 0x00, 0x42, 0x40, 0x42, 0x80, 0xC2, 0x7F,
      0x42, 0x00, 0x42, 0x00, 0x42, 0x00, 0x42, 0x02, 0x42, 0x04, 0x40, 0x08, 0x40, 0x30, 0x00, 0x00}},
    {'结',
     {0x20, 0x22, 0x30, 0x67, 0xAC, 0x22, 0x63, 0x12, 0x20, 0x12, 0x18, 0x12, 0x08, 0x00, 0x48, 0xFE,
      0x48, 0x42, 0x48, 0x42, 0x7F, 0x42, 0x48, 0x42, 0x48, 0x42, 0x48, 0xFE, 0x08, 0x00, 0x00, 0x00}},

    {'束',
     {0x04, 0x40, 0x04, 0x40, 0xE4, 0x27, 0x24, 0x22, 0x24, 0x12, 0x24, 0x0A, 0x24, 0x06, 0xFF, 0xFF,
      0x24, 0x06, 0x24, 0x0A, 0x24, 0x12, 0x24, 0x22, 0xE4, 0x27, 0x04, 0x40, 0x04, 0x40, 0x00, 0x00}},
    {'在',
     {0x08, 0x02, 0x08, 0x01, 0x88, 0x00, 0xC8, 0xFF, 0x38, 0x40, 0x0C, 0x41, 0x0B, 0x41, 0x08, 0x41,
      0x08, 0x41, 0xE8, 0x7F, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x41, 0x08, 0x40, 0x00, 0x00}},

    {'内',
     {0x00, 0x00, 0xF8, 0xFF, 0x08, 0x00, 0x08, 0x08, 0x08, 0x04, 0x08, 0x02, 0x88, 0x01, 0x7F, 0x00,
      0x88, 0x00, 0x08, 0x01, 0x08, 0x02, 0x08, 0x4C, 0x08, 0x80, 0xF8, 0x7F, 0x00, 0x00, 0x00, 0x00}},

    {'外',
     {0x00, 0x81, 0xC0, 0x40, 0x30, 0x21, 0x1F, 0x12, 0x10, 0x0C, 0x10, 0x03, 0xF0, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xFF, 0xFF, 0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00}},
    {'已',
     {0x00, 0x00, 0x00, 0x00, 0xE2, 0x3F, 0x82, 0x40, 0x82, 0x40, 0x82, 0x40, 0x82, 0x40, 0x82, 0x40,
      0x82, 0x40, 0x82, 0x40, 0x82, 0x40, 0xFE, 0x40, 0x00, 0x40, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00}},

    {'连',
     {0x40, 0x00, 0x40, 0x40, 0x42, 0x20, 0xCC, 0x1F, 0x00, 0x20, 0x04, 0x44, 0x44, 0x44, 0x64, 0x44,
      0x5C, 0x44, 0x47, 0x44, 0xF4, 0x7F, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x04, 0x44, 0x00, 0x00}},

    {'接',
     {0x10, 0x04, 0x10, 0x44, 0x10, 0x82, 0xFF, 0x7F, 0x10, 0x01, 0x50, 0x82, 0x44, 0x82, 0x54, 0x4A,
      0x65, 0x56, 0xC6, 0x23, 0x44, 0x22, 0x64, 0x52, 0x54, 0x4E, 0x44, 0x82, 0x40, 0x02, 0x00, 0x00}},

    {'到',
     {0x42, 0x40, 0x62, 0xC4, 0x52, 0x44, 0x4A, 0x44, 0xC6, 0x7F, 0x42, 0x24, 0x52, 0x24, 0x62, 0x24,
      0xC2, 0x20, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x40, 0x00, 0x80, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00}},
    {'配',
     {0xF2, 0xFF, 0x12, 0x4A, 0xFE, 0x49, 0x12, 0x48, 0xFE, 0x49, 0x12, 0x49, 0xF2, 0xFF, 0x00, 0x00,
      0x84, 0x3F, 0x84, 0x40, 0x84, 0x40, 0x84, 0x40, 0xFC, 0x41, 0x00, 0x40, 0x00, 0x70, 0x00, 0x00}},

    {'网',
     {0x00, 0x00, 0xFE, 0xFF, 0x02, 0x10, 0x22, 0x08, 0x42, 0x06, 0x82, 0x01, 0x72, 0x0E, 0x02, 0x10,
      0x22, 0x08, 0x42, 0x06, 0x82, 0x01, 0x72, 0x4E, 0x02, 0x80, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00}},
    {'未',
     {0x80, 0x20, 0x80, 0x20, 0x88, 0x10, 0x88, 0x08, 0x88, 0x04, 0x88, 0x02, 0x88, 0x01, 0xFF, 0xFF,
      0x88, 0x01, 0x88, 0x02, 0x88, 0x04, 0x88, 0x08, 0x88, 0x10, 0x80, 0x20, 0x80, 0x20, 0x00, 0x00}},
    {'和',
     {0x20, 0x10, 0x24, 0x08, 0x24, 0x06, 0xA4, 0x01, 0xFE, 0xFF, 0x23, 0x01, 0x22, 0x06, 0x20, 0x00,
      0x00, 0x00, 0xF8, 0x3F, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0xF8, 0x3F, 0x00, 0x00, 0x00, 0x00}},

    {'密',
     {0x10, 0x02, 0x8C, 0x02, 0x44, 0x7A, 0x04, 0x41, 0xE4, 0x41, 0x04, 0x43, 0x95, 0x42, 0xA6, 0x7E,
      0x44, 0x42, 0x24, 0x42, 0x14, 0x42, 0x84, 0x43, 0x44, 0xF8, 0x94, 0x00, 0x0C, 0x00, 0x00, 0x00}},

    {'码',
     {0x04, 0x02, 0x84, 0x01, 0xE4, 0x7F, 0x5C, 0x10, 0x44, 0x10, 0xC4, 0x3F, 0x00, 0x00, 0x02, 0x08,
      0xF2, 0x08, 0x82, 0x08, 0x82, 0x08, 0x82, 0x48, 0xFE, 0x88, 0x80, 0x40, 0x80, 0x3F, 0x00, 0x00}},
    {'复',
     {0x20, 0x80, 0x10, 0x90, 0x08, 0x90, 0xF7, 0x49, 0x54, 0x4D, 0x54, 0x57, 0x54, 0x25, 0x54, 0x25,
      0x54, 0x25, 0x54, 0x55, 0x54, 0x4D, 0xF4, 0x45, 0x04, 0x80, 0x04, 0x80, 0x00, 0x80, 0x00, 0x00}},

    {'位',
     {0x00, 0x01, 0x80, 0x00, 0x60, 0x00, 0xF8, 0xFF, 0x07, 0x40, 0x10, 0x40, 0x90, 0x41, 0x10, 0x5E,
      0x11, 0x40, 0x16, 0x40, 0x10, 0x70, 0x10, 0x4E, 0xD0, 0x41, 0x10, 0x40, 0x00, 0x40, 0x00, 0x00}},

    {'重',
     {0x10, 0x40, 0x10, 0x40, 0x14, 0x50, 0xD4, 0x57, 0x54, 0x55, 0x54, 0x55, 0x54, 0x55, 0xFC, 0x7F,
      0x52, 0x55, 0x52, 0x55, 0x52, 0x55, 0xD3, 0x57, 0x12, 0x50, 0x10, 0x40, 0x10, 0x40, 0x00, 0x00}},

    {'新',
     {0x40, 0x20, 0x44, 0x12, 0x54, 0x4A, 0x65, 0x82, 0xC6, 0x7F, 0x64, 0x02, 0x54, 0x0A, 0x44, 0x92,
      0x00, 0x60, 0xFC, 0x1F, 0x44, 0x00, 0x44, 0x00, 0xC4, 0xFF, 0x42, 0x00, 0x40, 0x00, 0x00, 0x00}},
    {'手',
     {0x00, 0x02, 0x00, 0x02, 0x24, 0x02, 0x24, 0x02, 0x24, 0x02, 0x24, 0x42, 0x24, 0x82, 0xFC, 0x7F,
      0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x23, 0x02, 0x22, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00}},

    {'联',
     {0x02, 0x10, 0xFE, 0x1F, 0x92, 0x08, 0x92, 0x08, 0xFE, 0xFF, 0x02, 0x04, 0x00, 0x81, 0x10, 0x41,
      0x11, 0x31, 0x16, 0x0D, 0xF0, 0x03, 0x14, 0x0D, 0x13, 0x31, 0x10, 0x41, 0x00, 0x81, 0x00, 0x00}},

    {'自',
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x88, 0x44, 0x8C, 0x44, 0x8A, 0x44, 0x89, 0x44,
      0x88, 0x44, 0x88, 0x44, 0x88, 0x44, 0xF8, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    {'同',
     {0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x02, 0x00, 0x12, 0x00, 0x92, 0x1F, 0x92, 0x08, 0x92, 0x08,
      0x92, 0x08, 0x92, 0x08, 0x92, 0x1F, 0x12, 0x40, 0x02, 0x80, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00}},

    {'步',
     {0x40, 0x80, 0x40, 0x90, 0x40, 0x88, 0x7C, 0x46, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0xFF, 0x2F,
      0x44, 0x10, 0x44, 0x10, 0x44, 0x08, 0x44, 0x04, 0x44, 0x02, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00}},
    {'中',
     {0x00, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0xFF, 0xFF,
      0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {'失',
     {0x00, 0x81, 0x40, 0x81, 0x30, 0x41, 0x1E, 0x21, 0x10, 0x11, 0x10, 0x0D, 0x10, 0x03, 0xFF, 0x01,
      0x10, 0x03, 0x10, 0x0D, 0x10, 0x11, 0x10, 0x21, 0x10, 0x41, 0x00, 0x81, 0x00, 0x81, 0x00, 0x00}},

    {'败',
     {0x00, 0x80, 0xFE, 0x47, 0x02, 0x30, 0xFA, 0x0F, 0x02, 0x10, 0xFE, 0x67, 0x40, 0x80, 0x20, 0x40,
      0xD8, 0x21, 0x17, 0x16, 0x10, 0x08, 0x10, 0x16, 0xF0, 0x21, 0x10, 0x40, 0x10, 0x80, 0x00, 0x00}},
    {'方',
     {0x08, 0x00, 0x08, 0x80, 0x08, 0x40, 0x08, 0x20, 0x08, 0x18, 0xF8, 0x07, 0x89, 0x00, 0x8E, 0x00,
      0x88, 0x40, 0x88, 0x80, 0x88, 0x40, 0x88, 0x3F, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00}},

};

#endif
