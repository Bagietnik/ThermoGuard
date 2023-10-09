#include "main.h"

void app_main(void)
{
    connect_wifi();

    mcp9808_init();

    esp_rom_gpio_pad_select_gpio(2);

    gpio_set_direction(2, GPIO_MODE_OUTPUT);

    mcp9808_setResolution(0x1);

    xTaskCreate(read_temperature_task, "temperature_task", 4096, NULL, 5, NULL);
    
    setup_server();
}