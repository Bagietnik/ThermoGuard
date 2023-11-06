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
    char temp_sign;
    float temp_upp;
    char temp_upp_sign;
    float temp_low;
    char temp_low_sign;
    float temp_crit;
    char temp_crit_sign;
    float resolution;
    uint8_t config_upper_byte;
    uint8_t config_lower_byte;
    uint8_t alarm_state;
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

void mcp9808_set_config_register();
void mcp9808_set_resolution();
void mcp9808_set_Tl();
void mcp9808_set_Tu();
void mcp9808_set_Tc();

void mcp9808_read_config_register();
void mcp9808_read_resolution();
void mcp9808_read_Tl();
void mcp9808_read_Tu();
void mcp9808_read_Tc();

void mcp9808_read_temp(void *pvParameters);
void mcp9808_read_alarm_output_state();

void read_process_values();

float get_temp();

uint8_t get_alarm_state();

ProcessValues_t *get_process_values();