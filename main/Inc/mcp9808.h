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
}MCP9808RegisterInfo_t;


typedef struct {
    float temp;
    uint8_t temp_sign;
    float temp_upp;
    uint8_t temp_upp_sign;
    float temp_low;
    uint8_t temp_low_sign;
    float temp_crit;
    uint8_t temp_crit_sign;
}ProcessValues_t;

typedef struct {
    bool alert_mode;
    bool alert_pol;
    bool alert_sel;
    bool alert_cnt;
    bool alert_stat;
    bool int_clear;
    bool win_lock;
    bool crit_lock;
    bool shdn;
    bool t_hyst_1;
    bool t_hyst_2;
}ConfigRegisterValues_t;

void mcp9808_init();

esp_err_t i2c_master_init(void);

void mcp9808_setResolution();

uint8_t mcp9808_get_resolution(void);

esp_err_t mcp9808_register_write_byte(uint8_t reg_addr, uint8_t data);

esp_err_t mcp9808_register_read(uint8_t reg_addr, uint8_t *data, size_t len);

void get_new_temp(void *pvParameters);

void mcp9808_set_config_register();

void mcp9808_get_config_register();

float get_temp();

void get_upper_temp();

void get_lower_temp();

void set_upper_temp();

void set_lower_temp();

void get_alarm_output_state();

void get_crit_temp();

void set_crit_temp();