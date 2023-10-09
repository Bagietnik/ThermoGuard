#pragma once

//WiFi
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"

//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include <stdio.h>
#include <string.h>

//Main wifi connect function
void connect_wifi(void);

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
