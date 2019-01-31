/**!
 * @file gpio_controller.hpp
 * @author Rodrigo Peixoto (rodrigopex@edge.ufal.br)
 * @brief
 * @version 0.1
 * @date 2019-01-28
 *
 */

#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <zephyr.h>

#define MAX_INPUT_PINS 4

#include "pin_interface.hpp"


class GPIOController
{
   public:
    void init(PinInput *pin_input)
    {
        if (pin_input && m_device == nullptr) {
            m_device = pin_input->device();
            gpio_add_callback(m_device, &m_gpio_cb);
        }
        this->add_callback(pin_input);
    }
    int add_callback(PinInput *pin_input)
    {
        m_pin_mask = m_pin_mask | BIT(pin_input->pin());
        gpio_init_callback(&m_gpio_cb, &GPIOController::changed, m_pin_mask);
        printk("Error 2: %d\n", gpio_pin_enable_callback(m_device, pin_input->pin()));
        s8_t available = -1;
        bool exists    = false;
        for (u8_t i = 0; i < MAX_INPUT_PINS; i++) {
            if (m_pins[i] == nullptr) {
                available = i;
                break;
            } else if (m_pins[i] == pin_input) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            if (available >= 0) {
                m_pins[available] = pin_input;
                printk("Pin added at %d\n", available);
            } else {
                printk("No more room for this item\n");
            }
        }
        return 0;
    }
    static void changed(struct device *dev, struct gpio_callback *cb, u32_t pins)
    {
        GPIOController *c = GPIOController::instance();
        for (u8_t i = 0; c->m_pins[i] && (i < MAX_INPUT_PINS); i++) {
            if (BIT(c->m_pins[i]->pin()) & pins) {
                c->m_pins[i]->callback();
            }
        }
    }
    static GPIOController *instance()
    {
        return &m_instance;
    }

   private:
    GPIOController() : m_device(nullptr)
    {
        m_pin_mask = 0;
        for (u8_t i = 0; i < MAX_INPUT_PINS; i++) {
            m_pins[i] = nullptr;
        }
    }
    struct gpio_callback m_gpio_cb;
    struct device *m_device;
    PinInput *m_pins[MAX_INPUT_PINS];
    static GPIOController m_instance;
    u32_t m_pin_mask;
};