#pragma once

#include "driver/i2c.h"
#include "esp_log.h"

typedef struct {
    uint8_t sensor_address;
    uint8_t config_register;
    uint8_t t_upper_register;
    uint8_t t_lower_register;
    uint8_t t_crit_register;
    uint8_t amb_temp_register;
    uint8_t man_id_register;
    uint8_t dev_id_register;
    uint8_t res_register;
}MCP9808RegisterInfo;


typedef struct {
    float temperature;
    uint8_t sign;
}ProcessValues;

void mcp9808_init();

esp_err_t i2c_master_init(void);

void mcp9808_setResolution();

uint8_t mcp9808_get_resolution(void);

esp_err_t mcp9808_register_write_byte(uint8_t reg_addr, uint8_t data);

esp_err_t mcp9808_register_read(uint8_t reg_addr, uint8_t *data, size_t len);

void get_new_temp(void *pvParameters);

float get_temp();
