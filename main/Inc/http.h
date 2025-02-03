#pragma once

#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>
#include <esp_http_server.h>
#include "mcp9808.h"
#include <cJSON.h>

httpd_handle_t setup_server(void);

esp_err_t send_web_page(httpd_req_t *req);

esp_err_t get_req_handler(httpd_req_t *req);

esp_err_t send_json_response(httpd_req_t *req);


