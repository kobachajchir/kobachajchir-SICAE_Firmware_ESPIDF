#pragma once

#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include "esp_err.h"
#include "lcd_utils.h"

void wifi_init_softap(void);
void wifi_init_sta(void);

#endif // WIFI_UTILS_H
