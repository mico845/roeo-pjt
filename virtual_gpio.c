#include "device/peripheral/roeo_dev_gpio.h"
#include "roeo.h"
#include <string.h>


#define CONFIG_VIRTUAL_GPIO 1

#ifdef CONFIG_VIRTUAL_GPIO
#include <stdio.h>
static roeo_dev_gpio_t gpio_dev;

#define VIRTUAL_GPIO_PIN_MAX 32

struct virtual_gpio_reg
{
    uint8_t port;
    uint32_t mode;
    uint32_t output[4];
};

static struct virtual_gpio_reg gpio_reg;

static int drv_gpio_init(roeo_dev_gpio_t *gpio)
{
    (void)gpio;
    printf("[init gpio]: gpio init\r\n");
    gpio_reg.mode = 0;
    gpio_reg.port = 0;
    for (int i = 0; i < 4; i++) {
        gpio_reg.output[i] = 0; 
    }
    return ROEO_STATUS_OK;
}

static int drv_gpio_deinit(roeo_dev_gpio_t *gpio)
{
    (void)gpio;
    printf("[deinit gpio]: gpio deinit\r\n");
    gpio_reg.mode = 0;
    gpio_reg.port = 0;
    for (int i = 0; i < 4; i++) {
        gpio_reg.output[i] = 0; 
    }
    return ROEO_STATUS_OK;
}


static int drv_gpio_configure(roeo_dev_gpio_t *gpio, uint32_t mode)
{
    (void)gpio;
    switch (mode) {
        case ROEO_DEV_GPIO_MODE_NONE: 
        case ROEO_DEV_GPIO_MODE_OUTPUT:
        case ROEO_DEV_GPIO_MODE_OUTPUT_OD:
        case ROEO_DEV_GPIO_MODE_INPUT:
        case ROEO_DEV_GPIO_MODE_INPUT_DOWN:
        case ROEO_DEV_GPIO_MODE_INPUT_UP:
            gpio_reg.mode = mode;
            printf("[configure gpio]: gpio mode set to %d\r\n", mode);
            return ROEO_STATUS_OK;
        default:
            return ROEO_STATUS_INVAL;
    }
}


static int drv_gpio_read(roeo_dev_gpio_t *gpio, uint8_t *value)
{
    *value = (gpio_reg.output[gpio->port] >> gpio->pins) & 0x01;
    printf("[read gpio]: gpio port %d, pin %d, value %d\r\n", gpio->port, gpio->pins, *value);
    return ROEO_STATUS_OK; 
}


static int drv_gpio_write(roeo_dev_gpio_t *gpio, uint8_t value)
{
    gpio_reg.output[gpio->port] = ((value && 0x01) << gpio->pins);
    printf("[write gpio]: gpio port %d, pin %d, value %d\r\n", gpio->port, gpio->pins, value);
    return ROEO_STATUS_OK;
}

static roeo_drv_gpio_ops_t gpio_drv_ops =
{
    .init = drv_gpio_init,
    .deinit = drv_gpio_deinit,
    .configure = drv_gpio_configure,
    .read = drv_gpio_read,
    .write = drv_gpio_write,
};

static roeo_drv_t gpio_drv =
{
    &gpio_drv_ops,
    ROEO_NULL
};

static void virtual_gpio_init(void)
{
    printf("register virtual gpio\r\n");
    roeo_dev_gpio_register(&gpio_dev, "gpio", &gpio_drv);
}

ROEO_BSP_CALL_REGISTER(virtual_gpio, virtual_gpio_init)

#endif