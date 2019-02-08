/*
 * Copyright (c) 2015-2016 Wind River Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <device.h>
#include <errno.h>
#include <gpio.h>
#include <logging/log.h>
#include <misc/printk.h>
#include <nvs/nvs.h>
#include <shell/shell.h>
#include <shell/shell_fprintf.h>
#include <string.h>
#include <watchdog.h>
#include <zephyr.h>

#include "zeta.hpp"

LOG_MODULE_REGISTER(MAIN);

#define LED_PORT LED0_GPIO_CONTROLLER
#define LED0 LED0_GPIO_PIN

#define BUTTON0_PIN SW0_GPIO_PIN
#define BUTTON_GPIO_CONTROLLER SW0_GPIO_CONTROLLER

#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 8
#define NVS_STORAGE_OFFSET FLASH_AREA_STORAGE_OFFSET

#ifdef CONFIG_MCUMGR_CMD_IMG_MGMT
#include "img_mgmt/img_mgmt.h"
#endif

typedef enum { TC_MODE, TC_PERIOD } property_e;

struct device *wdt_dev;

zt::GPIOController gpioController(BUTTON_GPIO_CONTROLLER,
                                  [](struct device *dev, struct gpio_callback *cb,
                                     u32_t pins) {
                                      gpioController.notifyObservers(dev, pins);
                                  });


int main(void)
{
#ifdef CONFIG_MCUMGR_CMD_IMG_MGMT
    img_mgmt_register_group();
#endif

    static struct nvs_fs fs = {0};
    fs.sector_size          = NVS_SECTOR_SIZE;
    fs.sector_count         = NVS_SECTOR_COUNT;
    fs.offset               = NVS_STORAGE_OFFSET;

    int error = nvs_init(&fs, DT_FLASH_DEV_NAME);
    if (error) {
        LOG_ERR("Could not load %s, error code %d!\n", DT_FLASH_DEV_NAME, error);
    } else {
        u8_t data = 0;
        int rc    = nvs_read(&fs, TC_MODE, &data, 1);
        if (rc == 1) {
            LOG_DBG("Data recovered: %02Xh\n", data);
        } else {
            data = 0xFA;
            rc   = nvs_write(&fs, TC_MODE, &data, 1);
            if (rc == 1) {
                LOG_DBG("Data persisted: %02Xh\n", data);
            } else {
                LOG_ERR("Could not persist the data %02Xh on flash", data);
            }
        }
    }
    zt::DigitalOutput led0(ZT_ALLOC_BYTE(), LED_PORT, LED0);
    zt::DigitalInput button0(ZT_ALLOC_BYTES(1), BUTTON_GPIO_CONTROLLER, BUTTON0_PIN);
    button0.connect(&led0, 't');
    gpioController.add_observer(&button0);

    wdt_dev = device_get_binding(CONFIG_WDT_0_NAME);
    LOG_WRN("Starting a new mainloop right now!");
    while (1) {
        k_sleep(K_SECONDS(15));
        wdt_reload(wdt_dev);  // feed/reload watchdog
    }

    return 0;
}
