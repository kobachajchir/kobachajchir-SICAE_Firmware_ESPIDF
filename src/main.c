#include <stdio.h>
#include <globals.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/i2c.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <unistd.h>
#include "lcd_utils.h" // Include the header for LCD functions
#include "wifi_utils.h" // Include the header for WiFi functions
#include "driver/gpio.h"
#include "globals.h"

static const char *TAG = "main";

myByte btnFlag = {0};
myByte btnFlag2 = {0};

static uint32_t lastTime = 0;
static uint32_t btnEnterDuration = 0;
static uint32_t btnUpDuration = 0;
static uint32_t btnDownDuration = 0;

char linea[16];

void init_gpio() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << PIN_BTN_UP) | (1ULL << PIN_BTN_DOWN) | (1ULL << PIN_BTN_ENTER);
    io_conf.pull_down_en = 0; // Disable internal pull-down resistors
    io_conf.pull_up_en = 0;   // Disable internal pull-up resistors
    gpio_config(&io_conf);
}

bool debounce(int pin) {
    int count = 0;
    const int sample_count = 10; // Reduced sample count to 10
    const int debounce_delay = 10; // Reduced delay to 10 ms

    for (int i = 0; i < sample_count; i++) {
        int pin_state = gpio_get_level(pin); // Get the raw pin state
        if (pin_state == 1) { // Check for high level (button pressed)
            count++;
        }
        vTaskDelay(pdMS_TO_TICKS(debounce_delay)); // Delay for 10 ms
    }
    bool result = count > (sample_count / 2); // Return true if the pin reads high more than half the time
    //ESP_LOGI(TAG, "Debounce result for pin %d: %d", pin, result);
    return result;
}

void check_buttons() {
    uint32_t currentTime = xTaskGetTickCount() * portTICK_PERIOD_MS;

    // BTN ENTER
    if (debounce(PIN_BTN_ENTER) && !BTN_ENTER_PRESSED) {
        BTN_ENTER_PRESSED = 1;
        btnEnterDuration = currentTime;
        ESP_LOGI(TAG, "BTN_ENTER Pressed");
    } else if (!debounce(PIN_BTN_ENTER) && BTN_ENTER_PRESSED) {
        BTN_ENTER_PRESSED = 0;
        if ((currentTime - btnEnterDuration) >= BTN_PRESS_TIME && (currentTime - btnEnterDuration) < BTN_LONGPRESS_TIME) {
            ESP_LOGI(TAG, "BTN_ENTER shortpress");
            BTN_ENTER_RELEASED = 1;
        }else if ((currentTime - btnEnterDuration) >= BTN_LONGPRESS_TIME) {
            ESP_LOGI(TAG, "BTN_ENTER longpress");
            BTN_ENTER_RELEASED_LONGPRESS = 1;
        }
        btnEnterDuration = 0;
    }

    // BTN UP
    if (debounce(PIN_BTN_UP) && !BTN_UP_PRESSED) {
        BTN_UP_PRESSED = 1;
        btnUpDuration = currentTime;
        ESP_LOGI(TAG, "BTN_UP Pressed");
    } else if (!debounce(PIN_BTN_UP) && BTN_UP_PRESSED) {
        BTN_UP_PRESSED = 0;
        if ((currentTime - btnUpDuration) >= BTN_PRESS_TIME && (currentTime - btnUpDuration) < BTN_LONGPRESS_TIME) {
            ESP_LOGI(TAG, "BTN_UP shortpress");
            BTN_UP_RELEASED = 1;
        }else if ((currentTime - btnUpDuration) >= BTN_LONGPRESS_TIME) {
            ESP_LOGI(TAG, "BTN_UP longpress");
            BTN_UP_RELEASED_LONGPRESS = 1;
        }
        btnUpDuration = 0;
    }

    // BTN DOWN
    if (debounce(PIN_BTN_DOWN) && !BTN_DOWN_PRESSED) {
        BTN_DOWN_PRESSED = 1;
        btnDownDuration = currentTime;
        ESP_LOGI(TAG, "BTN_DOWN Pressed");
    } else if (!debounce(PIN_BTN_DOWN) && BTN_DOWN_PRESSED) {
        BTN_DOWN_PRESSED = 0;
        if ((currentTime - btnDownDuration) >= BTN_PRESS_TIME && (currentTime - btnDownDuration) < BTN_LONGPRESS_TIME) {
            ESP_LOGI(TAG, "BTN_DOWN shortpress");
            BTN_DOWN_RELEASED = 1;
        }else if ((currentTime - btnDownDuration) >= BTN_LONGPRESS_TIME) {
            ESP_LOGI(TAG, "BTN_DOWN longpress");
            BTN_DOWN_RELEASED_LONGPRESS = 1;
        }
        btnDownDuration = 0;
    }
}

void button_task(void *arg) {
    while (1) {
        check_buttons();
        vTaskDelay(pdMS_TO_TICKS(100)); // Adjust delay for main loop to 100 ms
    }
}

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
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_LOGI(TAG, "Initializing GPIO...");
    init_gpio();
    ESP_LOGI(TAG, "GPIO Initialized with external pull-down resistors");

    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_init();
    ESP_LOGI(TAG, "LCD initialized successfully");
    lcd_put_cur(0, 0); // Move cursor to the beginning of the first line
    lcd_send_string("INICIANDO");
    wifi_init_sta();
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
    ESP_LOGI(TAG, "Buttons tasks created");
    ESP_LOGI(TAG, "WiFi initialized successfully");
}