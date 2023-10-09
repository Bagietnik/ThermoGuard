#include "mcp9808.h"

static const char *TAG = "X003";

/* I2C Config params */
#define I2C_MASTER_SCL_IO           22                         /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           21                         /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000


/* mcp9808 sensor register addreses*/
#define MCP9808_SENSOR_ADDR                 0x18        /*!< Slave address of the mcp9808 sensor */

#define MCP9808_REG_CONFIG 0x01 // CONFIG
#define MCP9808_REG_T_UPPER 0x02 // T upper
#define MCP9808_REG_T_LOWER 0x03 //T lower
#define MCP9808_REG_T_CRIT 0x04   //T crit
#define MCP9808_REG_AMB_TEMP 0x05 //Ta
#define MCP9808_REG_MAN_ID 0x06  //Manufacturer ID 
#define MCP9808_REG_DEV_ID 0x07  //Device ID / Device Revision
#define MCP9808_REG_RES 0x08 //Resolution 

#define TEMPERATURE_READ_INTERVAL_SEC 5

#define MCP9808_RES_0_5         0x0
#define MCP9808_RES_0_25        0x1
#define MCP9808_RES_0_125       0x2
#define MCP9808_RES_0_0_625     0x3

/**
 * @brief Read a sequence of bytes from a MCP9808 sensor registers
 */
esp_err_t mcp9808_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, MCP9808_SENSOR_ADDR, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * @brief Write a byte to a MCP9808 sensor register
 */
esp_err_t mcp9808_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, MCP9808_SENSOR_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}

unsigned mcp9808_get_resolution(void)
{   
    uint8_t resolution = 0;
    mcp9808_register_read(MCP9808_REG_RES, resolution, 1);
    return resolution; 
}

void mcp9808_setResolution(uint8_t resolution)
{
    ESP_ERROR_CHECK(mcp9808_register_write_byte(MCP9808_REG_RES, resolution));
    ESP_LOGI(TAG, "New resolution has been saved");
    printf("Value: %d", mcp9808_get_resolution());
}

/**
 * @brief i2c master initialization
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

void mcp9808_init()
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
}

/**
* @brief Temperature reading
**/
void read_temperature_task(void *pvParameters)
{
    uint8_t sign;
    uint8_t data[2];
    float temperature;

    while (1)
    {   data[0] = 0; //UpperByte
        data[1] = 0; //LowerByte

        // Wykonaj odczyt temperatury
        ESP_ERROR_CHECK(mcp9808_register_read(MCP9808_REG_AMB_TEMP, data, 2));
        data[0] = data[0] & 0x1F;
        temperature = data[0] * 16.0 + data[1] / 16.0; 
        sign = data[0] & 0x10;
        ESP_LOGI(TAG, "Temperature = %f°C", temperature); // Zakładając, że wynik jest w formie bajtu
        ESP_LOGI(TAG, "Sign = %d°C", sign); // Zakładając, że wynik jest w formie bajtu

        // Odczekaj 5 sekund przed kolejnym odczytem
        vTaskDelay(TEMPERATURE_READ_INTERVAL_SEC * 1000 / portTICK_PERIOD_MS);
    }
}