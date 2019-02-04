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
#include <gpio.h>
#include <misc/printk.h>
#include <zephyr.h>

#include "gpio_controller.hpp"

#define LED_PORT LED0_GPIO_CONTROLLER
#define LED0 LED0_GPIO_PIN
#define LED1 LED1_GPIO_PIN

#define BUTTON0_PIN SW0_GPIO_PIN
#define BUTTON1_PIN SW1_GPIO_PIN
#define BUTTON2_PIN SW2_GPIO_PIN
#define BUTTON3_PIN SW3_GPIO_PIN
#define BUTTON_GPIO_CONTROLLER SW0_GPIO_CONTROLLER

int main(void)
{
    GPIOController::instance()->init(BUTTON_GPIO_CONTROLLER);
    zeta::DigitalOutput led0(ZT_ALLOC_BYTE(), LED_PORT, LED0);
    zeta::DigitalOutput led1(ZT_ALLOC_BYTE(), LED_PORT, LED1);
    led0.connect(&led1, 'w');
    zeta::DigitalInput button0(ZT_ALLOC_BYTES(1), BUTTON_GPIO_CONTROLLER, BUTTON0_PIN);
    button0.connect(&led0, 't');
    GPIOController::instance()->add_callback(&button0);
    zeta::DigitalInput button1(ZT_ALLOC_BYTE(), BUTTON_GPIO_CONTROLLER, BUTTON1_PIN);
    button1.connect(&led0, 't');
    GPIOController::instance()->add_callback(&button1);

    printk("Hello!\n");
    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
