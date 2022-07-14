#include "main.hpp"

#define CS_PIN GPIO_NUM_5
#define EX_UART_NUM UART_NUM_0
#define INT_PIN GPIO_NUM_4

struct batch_info
{
    int label;
    int batch_no;
    size_t batch_num_total;
    size_t batch_binary_len;
};

struct blob_frame
{
    long long int timestamp;
    uint8_t* buffer;
};

static esp_mqtt_client_handle_t client = NULL;
esp_mqtt_client_handle_t iot_client = NULL;
xQueueHandle q_listen = NULL;
ObjectList tracking;
volatile int RTC_NOINIT_ATTR global_count;
TaskHandle_t sleepTimer = NULL;
TaskHandle_t signalInterruptTaskHandle;

ArduCAM_OV2640 myCAM(CS_PIN);

void IRAM_ATTR reset_count(void* arg){
    xTaskNotify(signalInterruptTaskHandle, 0, eNoAction);
}

void signalInterruptTask(void *pvParameters){

    while (1){
        // Wait 5 seconds before resetting global count to 0
        vTaskDelay(pdMS_TO_TICKS(5000));
        global_count = 0;
        char count_msg_buf[200];
        memset(count_msg_buf, 0, sizeof(count_msg_buf));
        sprintf(count_msg_buf, "%d", global_count);
        mqtt_send(client, "ovcount", count_msg_buf, 0, 10);
        memset(count_msg_buf, 0, sizeof(count_msg_buf));
        struct timeval now;
        gettimeofday(&now, NULL);
        int64_t timestamp_ms = ((int64_t)now.tv_sec) * 1000 + now.tv_usec / 1000;
        sprintf(count_msg_buf, IOT_MESSAGE_PATTERN, IOT_USERNAME_PUB, IOT_SENSOR, global_count, IOT_DEVICE_ID, timestamp_ms);
        mqtt_send(iot_client, IOT_TOPIC, count_msg_buf, 0, strlen(count_msg_buf));
        xTaskNotifyWait(0x00,ULONG_MAX,NULL,portMAX_DELAY);
    }
}

void OV2640_valid_check()
{
    // Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    uint8_t temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55)
        printf("SPI: interface Error 0x%2x!\n", temp);
    else
        printf("SPI: interface OK\n");

    // Check if the camera module type is OV2640
    uint8_t vid, pid;
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26) && ((pid != 0x41) || (pid != 0x42)))
        printf("I2C: Can't find OV2640 module!\n");
    else
        printf("I2C: OV2640 detected.\n");
}

void capture_one_frame()
{
    //printf("start capture\n");
    myCAM.clear_fifo_flag();
    myCAM.start_capture();
    while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
        vTaskDelay(pdMS_TO_TICKS(2));
}

libattopng_t* frame_to_png(unsigned char *image, int width, int height){
		
	libattopng_t *png = libattopng_new(width, height, PNG_RGB);

    if (png==NULL || png->data==NULL){
        ESP_LOGE("test", "PNG not proper allocated");
        while (true){}
    }

	int x, y;
    uint8_t red, green, blue;
    uint32_t color;
    int index = 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
            red = image[index];
            index++;
            green = image[index];
            index++;
            blue = image[index];
            index++;
            color= red << 16 | green << 8 | blue;
            libattopng_set_pixel(png, x, y, color);
            
        }
	}

    return(png);

}

void get_image_data(uint8_t* pic_buf){
    size_t len = myCAM.read_fifo_length();
    uint8_t *buffer = (uint8_t *) malloc(len);
    assert(buffer);
 
    if (buffer==NULL){
        ESP_LOGE("test", "buffer not allocated");
        while (true){}
    }

    if (len >= MAX_FIFO_SIZE)
    {
        printf("Over size.\n");
        return;
    }
    else if (len == 0)
    {
        printf("Size is 0.\n");
        return;
    }
    size_t batch_num = len / SPI_MAX_TRANS_SIZE;
    size_t tail_len = len % SPI_MAX_TRANS_SIZE;
    uint8_t *start=buffer;
    
    for (int i = 0; i < batch_num; i++)
    {
        myCAM.CS_LOW();
        // Copy data to buffer
        spi_transfer_bytes(BURST_FIFO_READ, start, start, SPI_MAX_TRANS_SIZE);
        myCAM.CS_HIGH();
        start=start+SPI_MAX_TRANS_SIZE;
    }
    if (tail_len != 0)
    {
        myCAM.CS_LOW();
        // Copy data to buffer
        spi_transfer_bytes(BURST_FIFO_READ, start, start, tail_len);
        myCAM.CS_HIGH();
        start=start+tail_len;
    }
    // Convert jpeg to RGB888
    fmt2rgb888(buffer, len, PIXFORMAT_JPEG, pic_buf);
    free(buffer);
}

void take_rgb(void *_){
    // Allocate memory for segmentation map, background model and image frame buffer
    uint8_t *segmentation_map = (uint8_t*)malloc(160 * 120 * sizeof(uint8_t));
    vibeModel_Sequential_t *model = NULL;
    uint8_t *pic_buf = (uint8_t *) malloc(3* 160 * 120 * sizeof(uint8_t));
    assert(pic_buf);
    char count_msg_buf[200];
    memset(count_msg_buf, 0, sizeof(count_msg_buf));
    // Capture first frame to set up background
    capture_one_frame();
    get_image_data(pic_buf);
    // Off auto exposure
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.wrSensorReg8_8(0x13, 0x00);
    
    // Initialise background model
    model = (vibeModel_Sequential_t *)libvibeModel_Sequential_New();
    libvibeModel_Sequential_AllocInit_8u_C3R(model, pic_buf, 160, 120);
    libvibeModel_Sequential_SetMatchingThreshold(model, 30);

    // Uncomment to send background image to Node-Red for storage
    
    // mqtt_send(client, "ovSavePic", (char*) pic_buf, 0, 3*160*120);

    int refresh = 0;
    uint32_t history_pixels = 0;
    //printf("Start counting\n");
    
    // Uncomment to enable storage of image frames when foreground objects are detected
    // up to 15 image frames are stored in a buffer, and are only sent to Node-Red when
    // the buffer is full or when no foreground objects are detected. This allows the
    // algorithm to continue tracking objects accurately, without being delayed due to
    // sending the image frames. (Part 1/3)

    // int log_index = 0;
    // blob_frame* log = (blob_frame *) malloc(15 * sizeof(blob_frame));
    // for(int i = 0; i < 15; i++){
    //     log[i].timestamp = 0;
    //     log[i].buffer = (uint8_t*) malloc(3 * 160 * 120 * sizeof(uint8_t));
    // }
    while (1)
    {
        capture_one_frame();
        // image frame is stored in pic_buf as RGB888
        get_image_data(pic_buf);

        // Segmentation step: produces the output mask
        uint32_t pixels = libvibeModel_Sequential_Segmentation_8u_C3R(model, pic_buf, segmentation_map);
        if(history_pixels > 0 && pixels > 0 && ((history_pixels - pixels < 150) || (history_pixels - pixels > (- 150)))){
            refresh++;
        } else {
            history_pixels = pixels;
            refresh = 0;
        }
        
        // Reinitialise background if it has changed. Background change is detected when:
        // 1. Number of foreground pixels is significantly large (>= 90% of the total frame), or
        // 2. The number of foreground pixels remains relatively constant in the last 15 frames
        if(refresh >= 15 || pixels >= 17280){
            printf("-------------Reinitialise----------\n");
            libvibeModel_Sequential_Free(model);
            model = (vibeModel_Sequential_t *)libvibeModel_Sequential_New();
            libvibeModel_Sequential_AllocInit_8u_C3R(model, pic_buf, 160, 120);
            libvibeModel_Sequential_SetMatchingThreshold(model, 30);
            // Uncomment to send new background image for storage
            // mqtt_send(client, "ovSavePic", (char*) gray_buf, 0, 160*120);
            refresh = 0;
            history_pixels = 0;
            continue;
        }        
        // Update the background model
        libvibeModel_Sequential_Update_8u_C3R(model, pic_buf, segmentation_map);
        int n_blobs = blob_detection(NULL, segmentation_map, 0);
        struct timeval now;
        gettimeofday(&now, NULL);
        int64_t timestamp_ms = ((int64_t)now.tv_sec) * 1000 + now.tv_usec / 1000;

        // Uncomment to enable storage of image frames when foreground objects are detected
        // up to 15 image frames are stored in a buffer, and are only sent to Node-Red when
        // the buffer is full or when no foreground objects are detected. This allows the
        // algorithm to continue tracking objects accurately, without being delayed due to
        // sending the image frames. (Part 2/3)

        // if(n_blobs > 0){
        //     log[log_index].timestamp = timestamp_ms;
        //     memcpy(log[log_index].buffer, pic_buf, 3*160*120);
        //     log_index++;
        //     if(log_index >= 15){
        //         for(int i = 0; i < 15; i++){
        //             char time_buf[50];
        //             sprintf(time_buf, "Timestamp: %lld\n", log[i].timestamp);
        //             mqtt_send(client, "ovTimeStamp", time_buf, 0, strlen(time_buf));
        //             mqtt_send(client, "ovSavePic", (char*) log[i].buffer, 0, 3*160*120);

        //         }
        //         log_index = 0;
        //     }
        // } else if(log_index > 0){
        //     for(int i = 0; i < log_index; i++){
        //         char time_buf[50];
        //         sprintf(time_buf, "Timestamp: %lld\n", log[i].timestamp);
        //         mqtt_send(client, "ovTimeStamp", time_buf, 0, strlen(time_buf));
        //         mqtt_send(client, "ovSavePic", (char*) log[i].buffer, 0, 3*160*120);
        //     }
        //     log_index = 0;
        // }

        // Uncomment to send obtained segmentation map to Node-Red for visualization
        // mqtt_send(client, "ov2640/segmap", (char *) segmentation_map, 0, 160*120);

        Blob *blob_list = extract_feature(segmentation_map, n_blobs, IM_W, IM_H);
        tracking.matching(blob_list, n_blobs);
        delete_blob_list(blob_list, n_blobs);
        int count = tracking.get_count();
        if (count == global_count)
        {
            continue;
        }
        sprintf(count_msg_buf, "%d", count);
        
        // Send occupancy count information to Node-Red and IoT Platform
        mqtt_send(client, "ovcount", count_msg_buf, 0, 10);
        memset(count_msg_buf, 0, sizeof(count_msg_buf));
        global_count = count;
        sprintf(count_msg_buf, IOT_MESSAGE_PATTERN, IOT_USERNAME_PUB, IOT_SENSOR, count, IOT_DEVICE_ID, timestamp_ms);
        mqtt_send(iot_client, IOT_TOPIC, count_msg_buf, 0, strlen(count_msg_buf));
        memset(count_msg_buf, 0, sizeof(count_msg_buf));
        vTaskDelay(pdMS_TO_TICKS(2));
    }
    // Uncomment to enable storage of image frames when foreground objects are detected
    // up to 15 image frames are stored in a buffer, and are only sent to Node-Red when
    // the buffer is full or when no foreground objects are detected. This allows the
    // algorithm to continue tracking objects accurately, without being delayed due to
    // sending the image frames. (Part 3/3)

    // for(int i = 0; i < 15; i++){
    //     log[i].timestamp = 0;
    //     free(log[i].buffer);
    // }
    // free(log);
    libvibeModel_Sequential_Free(model);
    free(pic_buf);
    free(segmentation_map);
}

void listen_rgb(void *_)
{
    struct mqtt_msg msg;
    uint8_t* buf = (uint8_t *) malloc(57600);
    uint8_t *segmentation_map = (uint8_t*)malloc(160 * 120 * sizeof(uint8_t));
    vibeModel_Sequential_t *model = NULL;
    printf("entered task listen\n");
    char count_msg_buf[200];
    memset(count_msg_buf, 0, sizeof(count_msg_buf));
    int setup = 1;
    int refresh = 0;
    uint32_t history_pixels = 0;
    while (1)
    {
        // Receive image frame as MQTT message via Node-Red
        if (xQueueReceive(q_listen, &msg, portMAX_DELAY))
        {
            if (strcmp(msg.topic, "ov2640/simul") == 0)
            {
                memcpy(buf + msg.offset, msg.msg, msg.size);
                // Enter if block once the full image frame is received
                if(msg.offset + msg.size == 57600){
                    // Convert RGB888 to PNG, then send to Node-Red for image visualization
                    libattopng_t *png = frame_to_png((unsigned char *) buf, 160, 120);
                    static const char *topic = "ov2640/base64";
                    size_t encode_len;
                    size_t dataLen=0;
  	                const unsigned char *data = (unsigned char*)libattopng_get_data(png, &dataLen);
                    size_t base64_buf_length = ((dataLen) / 3 + 2) * 4;
                    uint8_t *base64_buf = (uint8_t *) malloc(base64_buf_length);
                    if (base64_buf==NULL){
                        ESP_LOGE("test", "base64_buf not allocated");
                        while (true){}
                    } 
                    if (mbedtls_base64_encode(base64_buf, base64_buf_length, &encode_len, data, dataLen)!=0){
                        printf("Error in encoding\n\n");
                    }
                    base64_buf[encode_len] = '\0';
                    mqtt_send(client, topic, (char *)base64_buf, 0, encode_len);
                    free(base64_buf);
                    libattopng_destroy(png);
                    printf("Frame sent\n");

                    // First image frame received is used to initialize background model
                    if(setup){
                        printf("Initialising model\n");
                        model = (vibeModel_Sequential_t *)libvibeModel_Sequential_New();
                        libvibeModel_Sequential_AllocInit_8u_C3R(model, buf, 160, 120);
                        libvibeModel_Sequential_SetMatchingThreshold(model, 20);
                        setup = 0;
                        continue;   
                    }
                    // Setup has previously been done, segment new frame here
                    printf("Getting mask:\n");
                    uint32_t pixels = libvibeModel_Sequential_Segmentation_8u_C3R(model, buf, segmentation_map);
                    
                    // Reinitialise background if it has changed. Background change is detected when:
                    // 1. Number of foreground pixels is significantly large (>= 90% of the total frame), or
                    // 2. The number of foreground pixels remains relatively constant in the last 15 frames
                    if(history_pixels > 0 && pixels > 0 && ((history_pixels - pixels < 150) || (history_pixels - pixels > (- 150)))){
                        refresh++;
                    } else {
                        history_pixels = pixels;
                        refresh = 0;
                    }
                    if(refresh >= 15 || pixels >= 17280){
                        printf("-------------Reinitialise----------\n");
                        printf("Refresh: %d, history: %d, pixels: %d\n", refresh, history_pixels, pixels);
                        libvibeModel_Sequential_Free(model);
                        model = (vibeModel_Sequential_t *)libvibeModel_Sequential_New();
                        libvibeModel_Sequential_AllocInit_8u_C3R(model, buf, 160, 120);
                        libvibeModel_Sequential_SetMatchingThreshold(model, 30);
                        refresh = 0;
                        history_pixels = 0;
                        continue;
                    }
                    // Update the background model
                    libvibeModel_Sequential_Update_8u_C3R(model, buf, segmentation_map);
                    int n_blobs = blob_detection(NULL, segmentation_map, 0);
                    printf("n_blobs is %d\n", n_blobs);
                    // Send obtained segmentation map to Node-Red for visualization
                    mqtt_send(client, "ov2640/segmap", (char *) segmentation_map, 0, 160*120);
                    Blob *blob_list = extract_feature(segmentation_map, n_blobs, IM_W, IM_H);
                    tracking.matching(blob_list, n_blobs);
                    delete_blob_list(blob_list, n_blobs);
                    int count = tracking.get_count();
                    printf("Count is %d, global count is %d\n", count, global_count);
                    if (count == global_count)
                    {
                        continue;
                    }
                    sprintf(count_msg_buf, "%d", count);
                    mqtt_send(client, "ovcount", count_msg_buf, 0, 10);
                    memset(count_msg_buf, 0, sizeof(count_msg_buf));
                    global_count = count;
                }
                
            }
            free(msg.msg);
        }
    }
    libvibeModel_Sequential_Free(model);
    free(segmentation_map);
    free(buf);
}


void send_count(void *_)
{
    while(1){
        // Every 5 minutes, send the current count to Node-Red and the IoT Platform
        char count_msg_buf[200];
        memset(count_msg_buf, 0, sizeof(count_msg_buf));
        sprintf(count_msg_buf, "%d", global_count);
        mqtt_send(client, "ovcount", count_msg_buf, 0, 10);
        memset(count_msg_buf, 0, sizeof(count_msg_buf));
        struct timeval now;
        gettimeofday(&now, NULL);
        int64_t timestamp_ms = ((int64_t)now.tv_sec) * 1000 + now.tv_usec / 1000;
        sprintf(count_msg_buf, IOT_MESSAGE_PATTERN, IOT_USERNAME_PUB, IOT_SENSOR, global_count, IOT_DEVICE_ID, timestamp_ms);
        printf("msg: %s\n", count_msg_buf);
        mqtt_send(iot_client, IOT_TOPIC, count_msg_buf, 0, strlen(count_msg_buf));
        vTaskDelay(pdMS_TO_TICKS(300000));
        //300000 for 5 min
    }
}

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(start_wifi());
    initializeSntp();
    obtainTime();
    ESP_ERROR_CHECK(start_mqtt(&client, MYMQTT, NULL, NULL));
    ESP_ERROR_CHECK(start_mqtt(&iot_client, IOT_URI, IOT_USERNAME, IOT_PASSWORD));
    ESP_ERROR_CHECK(spi_master_init());
    ESP_ERROR_CHECK(i2c_master_init());
    global_count = 0;

    // Uncomment to allow debugging and simulation via Node-Red
    // mqtt_listen(client, "ov2640/simul", &q_listen);
    // xTaskCreatePinnedToCore(listen_rgb, "listen_rgb", 8192, NULL, 10, NULL, 1);

    // Uncomment to deploy algorithm
    ESP_ERROR_CHECK(gpio_set_direction(INT_PIN, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_pulldown_en (INT_PIN));
    ESP_ERROR_CHECK(gpio_set_intr_type(INT_PIN, GPIO_INTR_POSEDGE));
    ESP_ERROR_CHECK(gpio_isr_handler_add(INT_PIN, reset_count, NULL));
    OV2640_valid_check();
    myCAM.set_format(JPEG);
    myCAM.InitCAM();
    myCAM.set_JPEG_size(OV2640_160x120);

    myCAM.clear_fifo_flag();
    xTaskCreatePinnedToCore(take_rgb, "take_rgb", 8192, NULL, 10, NULL, 1);
    xTaskCreate(send_count, "send_count", 2048, NULL, 10, NULL);
    xTaskCreate(signalInterruptTask, "signalInterrupt", 2048, NULL, 2, &signalInterruptTaskHandle);

}
