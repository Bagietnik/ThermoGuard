#include "main.h"

void app_main(void)
{
    connect_wifi();

    setup_server();

    mcp9808_init();

    xTaskCreate(get_new_temp, "get temp", 4096, NULL, 2, NULL);
}