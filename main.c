#include <stdint.h>
#include <stdio.h>
#include "device/peripheral/roeo_dev_gpio.h"
#include "device/roeo_device.h"
#include "roeo.h"

int main() {
    roeo_bsp_init();
    uint8_t value = 0;
    int fd = roeo_dev_open("gpio");

    if (fd < 0) {
        printf("Failed to open GPIO device: %d\n", fd);
        return -1;
    }
    roeo_dev_ioctl(fd, ROEO_CMD_GPIO_SET_NUMBER, &(int){1}, sizeof(int));
    roeo_dev_ioctl(fd, ROEO_CMD_GPIO_SET_MODE, &(uint32_t){ROEO_DEV_GPIO_MODE_OUTPUT_PP}, sizeof(uint32_t));
    roeo_dev_write(fd, &(uint8_t){1}, sizeof(uint8_t));
    
    roeo_dev_read(fd, &value, sizeof(uint8_t));
    printf("GPIO value: %d\n", value);

    roeo_dev_write(fd, &(uint8_t){0}, sizeof(uint8_t));
    roeo_dev_read(fd, &value, sizeof(uint8_t));
    printf("GPIO value: %d\n", value);
    
    return 0;
}