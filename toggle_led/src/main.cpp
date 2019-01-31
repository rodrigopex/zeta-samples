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

class Button0 : public PinInput
{
   public:
    Button0(u8_t *data, PinOutput *led)
        : PinInput(data, BUTTON_GPIO_CONTROLLER, BUTTON0_PIN), m_led(led)
    {
    }
    int notify()
    {
        PinInput::notify();
        printk("Button0 pressed!");
        m_led->toggle();
        return 0;
    }

   private:
    PinOutput *m_led;
};

int main(void)
{
    PinOutput led0((u8_t *) alloca(1), LED_PORT, LED);
    Button0 b0((u8_t *) alloca(1), &led0);
    GPIOController::instance()->init(&b0);
    PinInput button1((u8_t *) alloca(1), BUTTON_GPIO_CONTROLLER, BUTTON1_PIN);
    GPIOController::instance()->init(&button1);

    printk("Hello!\n");
    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
