#include "roeo.h"

#define CONFIG_VIRTUAL_GPIO 1

#ifdef CONFIG_VIRTUAL_GPIO
#include <stdio.h>
static roeo_dev_gpio_t gpio_dev;

struct virtual_gpio_reg_t
{
  __IO uint32_t MODER;       /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;      /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;     /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;       /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;         /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;         /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint32_t BSRR;        /*!< GPIO port bit set/reset  register,     Address offset: 0x18      */
  __IO uint32_t LCKR;        /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];      /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
  __IO uint32_t BRR;         /*!< GPIO Bit Reset register,               Address offset: 0x28      */
};

static struct virtual_gpio_reg_t virtual_gpio_reg;


static int drv_gpio_configure(roeo_dev_gpio_t *gpio, int position, uint32_t mode)
{
    (void)gpio;

    if (!(mode & 0x01)){
        if (mode & 0x02) // Output Mode
        {
            virtual_gpio_reg.MODER &= ~(0x03 << (position * 2));
            virtual_gpio_reg.MODER |= (0x01 << (position * 2));
        }
        else            // Input Mode
        {
            virtual_gpio_reg.MODER &= ~(0x03 << (position * 2));
        }
        if (mode & 0x04) // Output Push-Pull
        {
            virtual_gpio_reg.OTYPER &= ~(1 << position);
        }
        else              // Output Open-Drain
        {
            virtual_gpio_reg.OTYPER |= (1 << position);
        }

        if ((mode & 0x18) == 0) // PULL NO
        {
            virtual_gpio_reg.PUPDR &= ~(0x03 << (position * 2));
            virtual_gpio_reg.PUPDR |= (0x00 << (position * 2));
        }
        else if (((mode & 0x18) >> 3) == 1) // PULL UP
        {
            virtual_gpio_reg.PUPDR &= ~(0x03 << (position * 2));
            virtual_gpio_reg.PUPDR |= (0x01 << (position * 2));
        }
        else if (((mode & 0x18) >> 3) == 2) // PULL DWON
        {
            virtual_gpio_reg.PUPDR &= ~(0x03 << (position * 2));
            virtual_gpio_reg.PUPDR |= (0x02 << (position * 2));
        }
    }

    
    return ROEO_STATUS_OK; 
}


static int drv_gpio_read(roeo_dev_gpio_t *gpio, int position, uint8_t *value)
{
    (void)gpio;
    (void)position;
    (void)value;
    (void)virtual_gpio_reg;

    *value = (virtual_gpio_reg.IDR & (1 << position)) ? 1 : 0;
    
    return ROEO_STATUS_OK; 
}


static int drv_gpio_write(roeo_dev_gpio_t *gpio, int position, uint8_t value)
{
    (void)gpio;

    if (value)
    {
        virtual_gpio_reg.BSRR |= (1 << position);
        virtual_gpio_reg.ODR |= (1 << position);
        virtual_gpio_reg.IDR = (1 << position); 
    }
    else 
    {
        virtual_gpio_reg.BRR |= (1 << position);
        virtual_gpio_reg.ODR &= ~(1 << position);
        virtual_gpio_reg.IDR &= ~(1 << position); 
    }
    
    return ROEO_STATUS_OK;
}

static roeo_drv_gpio_ops_t gpio_drv_ops =
{
    .configure = drv_gpio_configure,
    .read = drv_gpio_read,
    .write = drv_gpio_write,
};


static roeo_drv_t gpio_drv =
{
    .ops = &gpio_drv_ops,
    .data = ROEO_NULL
};

static void virtual_gpio_init(void)
{
    virtual_gpio_reg.MODER = 0xFFFFFFFF; // All pins at Analog mode
    virtual_gpio_reg.OTYPER = 0x0; // All pins at Output push-pull
    virtual_gpio_reg.OSPEEDR = 0xFFFFFFFF; // All pins at high speed
    virtual_gpio_reg.PUPDR = 0x0; // No pull-up, pull-down
    virtual_gpio_reg.LCKR = 0x0; // No lock
    virtual_gpio_reg.AFR[0] = 0x0; // No alternate function
    virtual_gpio_reg.AFR[1] = 0x0; // No alternate function
    roeo_dev_gpio_register(&gpio_dev, "gpio", &gpio_drv);
}

ROEO_BSP_CALL_REGISTER(virtual_gpio, virtual_gpio_init)

#endif