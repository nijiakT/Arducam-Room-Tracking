extern "C"
{
#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "time.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <sys/time.h>
#include "ArduCAM_esp_ov2640.h"
#include "mbedtls/base64.h"
#include "cJSON.h"
#include "esp_task_wdt.h"
#include "libattopng.h"
#include "esp_jpg_decode.h"
#include "img_converters.h"

#include "interface_i2c.h"
#include "interface_spi.h"
#include "network_common.h"
#include "esp_timer.h"
#include "vibe-background-sequential.h"
#include "helper.h"
#include "detect.h"
#include "feature_extraction.h"
#include "sntp_controller.h"
}

#include "human_object.hpp"
#include "tracking.hpp"
#include "macro.h"
