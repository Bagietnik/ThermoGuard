#include "http.h"

char on_resp[] = "";

char off_resp[] = "";

char temp[] = "<!DOCTYPE html><html><head><style type=\"text/css\">html {font-family:Arial,sans-serif;display:inline-block;margin:0px auto;text-align:center;background-color:#d7d0d0}h1{color:#070812;padding:2vh}.content{padding:50px}.card-container{display:grid;grid-template-columns:repeat(1,1fr);grid-gap:20px}.card{background-color:white;box-shadow:2px 2px 12px 1px rgba(140,140,140,.5);padding:30px;border-radius:8px}.temperature-value,.humidity-value{font-size:3rem;font-weight:bold;color:#ff9900}.temperature-label,.humidity-label{font-size:1.5rem;color:#555}</style><title>X003</title><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><link rel=\"icon\" href=\"data:,\"><link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\"><link rel=\"stylesheet\" type=\"text/css\"></head><body><h1>ESP32 WebServer</h1><div class=\"content\"><div class=\"card-container\"><div class=\"card\"><div class=\"temperature-value\">20°C</div><div class=\"temperature-label\">Aktualna temperatura</div></div></div></div></body></html>";

#define LED_PIN 2

int led_state = 0;

esp_err_t send_web_page(httpd_req_t *req)
{
    int response;
    if (led_state == 0)
        response = httpd_resp_send(req, off_resp, HTTPD_RESP_USE_STRLEN);
    else
        response = httpd_resp_send(req, on_resp, HTTPD_RESP_USE_STRLEN);
    return response;
}

esp_err_t get_req_handler(httpd_req_t *req)
{
    return send_web_page(req);
}

esp_err_t led_on_handler(httpd_req_t *req)
{
    gpio_set_level(LED_PIN, 1);
    led_state = 1;
    return send_web_page(req);
}

esp_err_t led_off_handler(httpd_req_t *req)
{
    gpio_set_level(LED_PIN, 0);
    led_state = 0;
    return send_web_page(req);
}

httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_req_handler,
    .user_ctx = NULL};

httpd_uri_t uri_on = {
    .uri = "/led2on",
    .method = HTTP_GET,
    .handler = led_on_handler,
    .user_ctx = NULL};

httpd_uri_t uri_off = {
    .uri = "/led2off",
    .method = HTTP_GET,
    .handler = led_off_handler,
    .user_ctx = NULL};

httpd_handle_t setup_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_on);
        httpd_register_uri_handler(server, &uri_off);
    }

    return server;
}