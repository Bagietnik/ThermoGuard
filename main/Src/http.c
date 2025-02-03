#include "http.h"

const char web_page[] = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"  <style type=\"text/css\">"
"    html {"
"      font-family: Arial, sans-serif;"
"      display: inline-block;"
"      margin: 0px auto;"
"      text-align: center;"
"      background-color: #d7d0d0;"
"    }"
"    h1 {"
"      color: #070812;"
"      padding: 2vh;"
"    }"
"    .content {"
"      padding: 50px;"
"    }"
""
"   .header-bar {"
      "background-color: rgba(80, 65, 65, 0.7); /* Czarny z przezroczystością */"
      "color: white;"
      "padding: 10px;"
      "font-size: 1.5rem;"
      "margin-bottom: 20px; /* Dodaj margines na dole header bara */"
      "border-radius: 8px;"
    "}"
"    .card-container {"
"    display: grid;"
"    grid-template-columns: repeat(1, 1fr); /* Trzy kafelki w jednym rzędzie */"
"    grid-gap: 20px; /* Odstęp między kafelkami */"
"    }"
""
"    .card {"
"      background-color: white;"
"      box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);"
"      padding: 30px;"
"      border-radius: 8px;"
"    }"
""
"    .temperature-value {"
"      font-size: 3rem;"
"      font-weight: bold;"
"      color: #ff9900;"
"    }"
"    .temperature-label {"
"      font-size: 1.5rem;"
"      color: #555;"
"    }"
"  </style>"
"  <title>X003</title>"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
"  <link rel=\"icon\" href=\"data:,\">"
"  <link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\""
"    integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">"
"  <link rel=\"stylesheet\" type=\"text/css\">"
"</head>"
"<body>"
"  <h1>ESP32 WebServer</h1>"
"  <div class=\"content\">"
"   <div class='header-bar'>Status</div>"
"    <div class=\"card-container\">"
"      <div class=\"card\">"
"          <div class=\"temperature-value\">%.2f&degC</div>"
"          <div class=\"temperature-label\">Temperature</div>"
"      </div>"
"    </div>"
"  </div>"
"   <script>"
"        function refreshPage() {"
"            location.reload();"
"        }"    
"    setTimeout(refreshPage, 60000);"
"  </script>"
"</body>"
"</html>";

esp_err_t send_web_page(httpd_req_t *req)
{
    char html_response[sizeof(web_page) + 20];
    //snprintf(html_response, sizeof(html_response), web_page_template, get_temp(), get_temp());
    snprintf(html_response, sizeof(html_response), web_page, get_temp());
    httpd_resp_send(req, html_response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t send_json_response(httpd_req_t *req) {

    ProcessValues_t *values = get_process_values();

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temp", values->temp);
    cJSON_AddNumberToObject(root, "temp_sign", values->temp_sign);
    cJSON_AddNumberToObject(root, "temp_upp", values->temp_upp);
    cJSON_AddNumberToObject(root, "temp_upp_sign", values->temp_upp_sign);
    cJSON_AddNumberToObject(root, "temp_low", values->temp_low);
    cJSON_AddNumberToObject(root, "temp_low_sign", values->temp_low_sign);
    cJSON_AddNumberToObject(root, "temp_crit", values->temp_crit);
    cJSON_AddNumberToObject(root, "temp_crit_sign", values->temp_crit_sign);
    cJSON_AddNumberToObject(root, "resolution", values->resolution);
    cJSON_AddNumberToObject(root, "config_upper_byte", values->config_upper_byte);
    cJSON_AddNumberToObject(root, "config_lower_byte", values->config_lower_byte);
    cJSON_AddNumberToObject(root, "alarm_state", values->alarm_state);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, json_str);
    free(json_str);

    return ESP_OK;
}

esp_err_t get_req_handler(httpd_req_t *req)
{
    return send_web_page(req);
}

httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_req_handler,
    .user_ctx = NULL
};

httpd_uri_t uri_get_json = {
    .uri = "/json",
    .method = HTTP_GET,
    .handler = send_json_response,
    .user_ctx = NULL
};


httpd_handle_t setup_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_get_json);
    }

    return server;
}