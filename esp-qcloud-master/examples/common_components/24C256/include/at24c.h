#ifndef AT24C_H_
#define AT24C_H_

#define I2C_FREQUENCY 100000
#define ACK_CHECK_EN 0x1  /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0       /*!< I2C ack value */
#define NACK_VAL 0x1      /*!< I2C nack value */

#define DATA_VERIFICATION 0x77   //数据有效性判断标志

typedef struct
{
    uint16_t _i2c_port;
    uint16_t _chip_addr;
    uint16_t _size;
    uint16_t _bytes;
} EEPROM_t;

void i2c_master_driver_initialize(EEPROM_t *dev, int16_t size, i2c_port_t i2c_port, int chip_addr, int i2c_gpio_sda, int i2c_gpio_scl);
uint16_t MaxAddress(EEPROM_t *dev);
static esp_err_t ReadReg8(EEPROM_t *dev, i2c_port_t i2c_port, int chip_addr, uint8_t data_addr, uint8_t *data);
static esp_err_t WriteReg8(EEPROM_t *dev, i2c_port_t i2c_port, int chip_addr, uint8_t data_addr, uint8_t data);
static esp_err_t ReadReg16(EEPROM_t *dev, uint16_t data_addr, uint8_t *data);
static esp_err_t WriteReg16(EEPROM_t *dev, uint16_t data_addr, uint8_t data);
esp_err_t ReadRom(EEPROM_t *dev, uint16_t data_addr, uint8_t *data);
esp_err_t WriteRom(EEPROM_t *dev, uint16_t data_addr, uint8_t data);
void WriteRomAndReadRomTest(EEPROM_t *dev, uint16_t data_addr, uint8_t WriteData, uint8_t *ReadData);
void WriteMultipByteEeprom(EEPROM_t *dev, uint16_t data_addr, uint8_t *PointWrite8Bit, uint8_t WriteCount);
uint8_t ReadMultipByteEeprom(EEPROM_t *dev, uint16_t data_addr, uint8_t *PointRead8Bit, uint8_t ReadCount);

#endif /* AT24C_H_ */
