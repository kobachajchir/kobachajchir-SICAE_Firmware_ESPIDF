#include <stdio.h>
#include <globals.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <unistd.h>
#include "lcd_utils.h" // Include the header for LCD functions
#include "wifi_utils.h" // Include the header for WiFi functions

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

void app_main(void) {
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_init();
    ESP_LOGI(TAG, "LCD initialized successfully");

    wifi_init_sta();
    ESP_LOGI(TAG, "WiFi initialized successfully");

    lcd_put_cur(0, 0); // Move cursor to the beginning of the first line
    lcd_send_string("Hello World!");

    lcd_put_cur(1, 0); // Move cursor to the beginning of the second line
    lcd_send_string("from ESP32");
}
