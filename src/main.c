#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <unistd.h>

#define I2C_MASTER_SCL_IO GPIO_NUM_22     /*!< GPIO number for I2C master clock */
#define I2C_MASTER_SDA_IO GPIO_NUM_21    /*!< GPIO number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */

#define SLAVE_ADDRESS_LCD 0x27 /*!< I2C address of the PCF8574T chip */

static const char *TAG = "i2c_lcd";

static esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t lcd_send_cmd(char cmd) {
    esp_err_t err;
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C; // en=1, rs=0
    data_t[1] = data_u | 0x08; // en=0, rs=0
    data_t[2] = data_l | 0x0C; // en=1, rs=0
    data_t[3] = data_l | 0x08; // en=0, rs=0

    err = i2c_master_write_to_device(I2C_MASTER_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
    if (err != ESP_OK) ESP_LOGI(TAG, "Error no. %d in command", err);
    return err;
}

static esp_err_t lcd_send_data(char data) {
    esp_err_t err;
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D; // en=1, rs=1
    data_t[1] = data_u | 0x09; // en=0, rs=1
    data_t[2] = data_l | 0x0D; // en=1, rs=1
    data_t[3] = data_l | 0x09; // en=0, rs=1

    err = i2c_master_write_to_device(I2C_MASTER_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
    if (err != ESP_OK) ESP_LOGI(TAG, "Error no. %d in command", err);
    return err;
}

static void lcd_init(void) {
   usleep(50000);
    lcd_send_cmd(0x30);
   usleep(5000);
    lcd_send_cmd(0x30);
   usleep(200);
    lcd_send_cmd(0x30);
   usleep(10000);
    lcd_send_cmd(0x20); // 4-bit mode
   usleep(10000);

    // display initialization
    lcd_send_cmd(0x28); // Function set: 4-bit mode, 2 lines, 5x8 dots
   usleep(1000);
    lcd_send_cmd(0x08); // Display off
   usleep(1000);
    lcd_send_cmd(0x01); // Clear display
   usleep(1000);
   usleep(1000);
    lcd_send_cmd(0x06); // Entry mode: increment cursor, no shift
   usleep(1000);
    lcd_send_cmd(0x0C); // Display on, cursor off, blink off
   usleep(1000);
}

static void lcd_send_string(char *str) {
    while (*str) lcd_send_data(*str++);
}

void lcd_clear(void){
    lcd_send_cmd(0x80);
    for (int i = 0; i < 70; i++){
        lcd_send_data(' ');
    }
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}

void app_main(void) {
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_init();
    ESP_LOGI(TAG, "LCD initialized successfully");

    lcd_send_cmd(0x80); // Move cursor to the beginning of the first line
    lcd_send_string("Hello World!");

    lcd_send_cmd(0xC0); // Move cursor to the beginning of the second line
    lcd_send_string("from ESP32");
}