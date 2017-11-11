/* I2C Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "fonts.h"
#include "ssd1306.h"
#include "nvs_flash.h"
#include "logo.h"
#include "init.h"


#define TAG "main"
/**
 * TEST CODE BRIEF
 *
 * This example will show you how to use I2C SSD1306 display module using the i2c bus:
 *
 * - Use one I2C port(master mode) to write to I2C port on one SSD1306 OLED display.
 *
 * Pin assignment:
 *
 * - master:
 *    GPIO21 is assigned as the data signal of i2c master port
 *    GPIO22 is assigned as the clock signal of i2c master port
 *
 * Connection:
 *
 * - connect GPIO21 with SDA
 * - connect GPIO22 with SCL
 * - no need to add external pull-up resistors, driver will enable internal pull-up resistors.
 *
 * Test items:
 *
 * - write to the display, if connected.
 * 
 */

#define I2C_EXAMPLE_MASTER_SCL_IO    GPIO_NUM_22    /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO    GPIO_NUM_21    /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ    1000000     /*!< I2C master clock frequency */


void i2c_test(void * ignore)
{

#if 0
    SSD1306_GotoXY(40, 4);
    SSD1306_Puts("ESP32", &Font_7x10, SSD1306_COLOR_WHITE);
    
    /* Go to location X = 20, Y = 25 */
    SSD1306_GotoXY(8, 25);
    SSD1306_Puts("I2C SSD1306 OLED", &Font_7x10, SSD1306_COLOR_WHITE);
    
    /* Go to location X = 15, Y = 45 */
    SSD1306_GotoXY(20, 45);
    SSD1306_Puts("Vergil Cola", &Font_7x10, SSD1306_COLOR_WHITE);
#endif

    SSD1306_DrawXbm(8, 10, Wifi_Logo.XbmWidth, Wifi_Logo.XbmHeight, (const char *) Wifi_Logo.data, SSD1306_COLOR_WHITE );
    /* Update screen, send changes to LCD */
    SSD1306_UpdateScreen();

    while (true) {
        /* Invert pixels */
        //SSD1306_ToggleInvert();
        
        /* Update screen */
        //SSD1306_UpdateScreen();
        
        /* Make a little delay */
        vTaskDelay(50);
    }

}

/**
 * @brief i2c master initialization
 */
static void i2c_example_master_init()
{
    i2c_port_t i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t) I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t) I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, I2C_MODE_MASTER,
                       0,
                       0, 0);
}

void app_main()
{
    i2c_example_master_init();
    SSD1306_Init();
    ESP_LOGI(TAG, "starting app_main()");
    ESP_LOGI(TAG, "RAM left: %u", esp_get_free_heap_size());

    init_hardware();

    start_wifi();
    start_web_radio();

    xTaskCreate(i2c_test, "i2c_test", 1024, NULL, 10, NULL);
}
