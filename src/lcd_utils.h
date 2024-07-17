#pragma once

#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <esp_err.h>

// Initialize the LCD
void lcd_init(void);

// Send a command to the LCD
esp_err_t lcd_send_cmd(char cmd);

// Send data to the LCD
esp_err_t lcd_send_data(char data);

// Send a string to the LCD
extern void lcd_send_string(char *str);

// Clear the LCD display
void lcd_clear(void);

// Set the cursor position on the LCD
extern void lcd_put_cur(int row, int col);

#endif // WIFI_UTILS_H