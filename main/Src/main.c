#include "main.h"
void app_main(void)
{    
    //connect_wifi();

    //setup_server();

    mcp9808_init();

    get_lower_temp();

    set_lower_temp();

    get_lower_temp();

    get_upper_temp();

    set_upper_temp();

    get_upper_temp();

    get_crit_temp();

    set_crit_temp();

    get_crit_temp();

    //printf("Conf reg: %d\n", mcp9808_get_config_register());

    xTaskCreate(get_new_temp, "get temp", 4096, NULL, 2, NULL);
}