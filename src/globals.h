#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "myByte.h"

/* Pin declarations */
#define PIN_BTN_ENTER 39
#define PIN_BTN_UP 35
#define PIN_BTN_DOWN 34

/* Timer defines */
#define MINMSTIME 10 //Time in ms for entering the loop
#define BTN_PRESS_TIME 30 //Time short press 30ms
#define BTN_LONGPRESS_TIME 1500 //Time long press 1.5s

/* Buttons flags*/
extern myByte btnFlag;
extern myByte btnFlag2;

/* Buttons flags defines*/
#define BTN_UP_PRESSED btnFlag.bits.bit0 //Button Up pressed
#define BTN_UP_RELEASED btnFlag.bits.bit1 //Button Up released
#define BTN_DOWN_PRESSED btnFlag.bits.bit2 //Button Down pressed
#define BTN_DOWN_RELEASED btnFlag.bits.bit3 //Button Down released
#define BTN_ENTER_PRESSED btnFlag.bits.bit4 //Button OK/Back pressed
#define BTN_ENTER_RELEASED btnFlag.bits.bit5 //Button OK/Back released
#define BTN_UP_RELEASED_LONGPRESS btnFlag.bits.bit6 //Button Up released long press
#define BTN_DOWN_RELEASED_LONGPRESS btnFlag.bits.bit7 //Button Down released long press
#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit0 //Button Enter released long press
//*#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit1 //Unused
//*#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit2 //Unused
//*#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit3 //Unused
//*#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit4 //Unused
//*#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit5 //Unused
//*#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit6 //Unused
//*#define BTN_ENTER_RELEASED_LONGPRESS btnFlag2.bits.bit7 //Unused

#endif // GLOBALS_H
