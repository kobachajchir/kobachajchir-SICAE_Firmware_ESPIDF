#pragma once

#define I2C_MASTER_SCL_IO GPIO_NUM_22     /*!< GPIO number for I2C master clock */
#define I2C_MASTER_SDA_IO GPIO_NUM_21    /*!< GPIO number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define SLAVE_ADDRESS_LCD 0x27 /*!< I2C address of the PCF8574T chip */
