#include "main.h"
void app_main(void)
{    
    connect_wifi();
    setup_server();

    mcp9808_init();

    xTaskCreate(mcp9808_read_temp, "get temp", 4096, NULL, 2, NULL);
}