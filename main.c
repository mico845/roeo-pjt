#include <stdio.h>
#include "roeo.h"

int main() {
    roeo_bsp_init();

    int fd = roeo_dev_open("gpio");
    if (fd < 0) {
        printf("Failed to open GPIO device: %d\n", fd);
        return -1;
    }
    uint32_t gpio_info[] = {1, 1};
    roeo_dev_ioctl(fd, ROEO_CMD_GPIO_SET_NUMBER, &gpio_info, sizeof(uint32_t) * 2);
    uint32_t mode = ROEO_DEV_GPIO_MODE_OUTPUT;
    roeo_dev_ioctl(fd, ROEO_CMD_GPIO_SET_MODE, &mode, sizeof(uint32_t));

    uint8_t value = 1;
    roeo_dev_write(fd, &value, sizeof(value));
    
    return 0;
}