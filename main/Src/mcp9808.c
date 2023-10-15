#include "mcp9808.h"

static const char *TAG = "X003";

MCP9808RegisterInfo mcp9808_registers = {
    .sensor_address =       0x18,            //I2C slave adress
    .config_register =      0x01,            //Config register
    .t_upper_register =     0x02,            //T upper
    .t_lower_register =     0x03,            //T lower
    .t_crit_register =      0x04,            //T crit
    .amb_temp_register =    0x05,            //Ta
    .man_id_register =      0x06,            //Manufacturer ID 
    .dev_id_register =      0x07,            //Device ID / Device Revision
    .res_register =         0x08,            //Resolution 
};

ProcessValues process_values = {
    .temperature = 0,                       //Official temp value
    .sign = 0,                              //Official sign value (0 -> +, 1 -> -)
};

// I2C config params
#define I2C_MASTER_SCL_IO                   22                         // GPIO number used for I2C master clock 
#define I2C_MASTER_SDA_IO                   21                         // GPIO number used for I2C master data 
#define I2C_MASTER_NUM                      0                          // I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip
#define I2C_MASTER_FREQ_HZ                  400000                     // I2C master clock frequency 
#define I2C_MASTER_TX_BUF_DISABLE           0                          // I2C master doesn't need buffer 
#define I2C_MASTER_RX_BUF_DISABLE           0                          // I2C master doesn't need buffer 
#define I2C_MASTER_TIMEOUT_MS               1000

// Values to write in the res_register
#define TEMP_MEAS_PERIOD                    CONFIG_TEMP_MEAS_PERIOD
#define MCP9808_RES_0_5                     0x0                        // Temp measurement resolution -> 0.5 °C
#define MCP9808_RES_0_25                    0x1                        // Temp measurement resolution -> 0.25 °C
#define MCP9808_RES_0_125                   0x2                        // Temp measurement resolution -> 0.125 °C
#define MCP9808_RES_0_0_625                 0x3                        // Temp measurement resolution -> 0.0625 °C

/**
 * @brief Read a sequence of bytes from a MCP9808 sensor registers
 */
esp_err_t mcp9808_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, mcp9808_registers.sensor_address, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * @brief Write a byte to a MCP9808 sensor register
 */
esp_err_t mcp9808_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, mcp9808_registers.sensor_address, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}

/**
 * @brief Get the register value - resolution of the temperature measurments on mcp9808 sensor
 */
uint8_t mcp9808_get_resolution(void)
{   
    uint8_t resolution = 0;
    mcp9808_register_read(mcp9808_registers.res_register, &resolution, 1);
    return resolution; 
}

/**
 * @brief Set the register value - resolution of the temperature measurments on mcp9808 sensor
 */
void mcp9808_setResolution()
{
    static uint8_t MCP9808_TEMP_RES = 0x0;
    
    #if CONFIG_TEMP_RESOLUTION_0_25
        MCP9808_TEMP_RES = MCP9808_RES_0_25;
    #endif

    #if CONFIG_TEMP_RESOLUTION_0_125
        MCP9808_TEMP_RES = MCP9808_RES_0_125;
    #endif

    #if CONFIG_TEMP_RESOLUTION_0_0_625
        MCP9808_TEMP_RES = MCP9808_RES_0_0_625;
    #endif

    ESP_ERROR_CHECK(mcp9808_register_write_byte(mcp9808_registers.res_register, MCP9808_TEMP_RES));
    ESP_LOGI(TAG, "New resolution has been saved");
    printf("Value: %d", mcp9808_get_resolution());
}

/**
 * @brief Get the current temperature value
 */
float get_temp()
{
    return process_values.temperature;
}

/**
 * @brief I2c master initialization
 */
esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/**
* @brief Initialization of the mcp9808 sensor (I2C + resolution)
**/
void mcp9808_init()
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    mcp9808_setResolution();
}

/**
* @brief Get the register value - temperature
**/
void get_new_temp(void *pvParameters)
{
    while (1)
    {   
        uint8_t *data = (uint8_t*)pvPortMalloc(2*sizeof(uint8_t));

        ESP_ERROR_CHECK(mcp9808_register_read(mcp9808_registers.amb_temp_register, data, 2));
        data[0] = data[0] & 0x1F;
        process_values.temperature = data[0] * 16.0 + data[1] / 16.0; 
        process_values.sign = data[0] & 0x10;
        if(process_values.sign == 0) {ESP_LOGI(TAG, "Temperature = + %f °C", process_values.temperature); } else { ESP_LOGI(TAG, "Temperature = - %f °C", process_values.temperature); }
        
        vPortFree(data);
        vTaskDelay(TEMP_MEAS_PERIOD * 1000 / portTICK_PERIOD_MS);
    }   
}