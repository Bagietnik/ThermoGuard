#pragma once

#include "driver/i2c.h"
#include "esp_log.h"

void read_temperature_task(void *pvParameters);

void mcp9808_init();

static esp_err_t i2c_master_init(void);

void mcp9808_setResolution(uint8_t resolution);

unsigned mcp9808_get_resolution(void);

esp_err_t mcp9808_register_write_byte(uint8_t reg_addr, uint8_t data);

esp_err_t mcp9808_register_read(uint8_t reg_addr, uint8_t *data, size_t len);