# roeo-pjt

基于roeo库开发的工程

| 层级         | 描述     |
|-------------|---------|
| app         | 应用层   |
| device      | 设备层   |
| bsp         | 硬件驱动层|

# 使用说明

根据业务需求编写应用

```c
#include <stdio.h>
#include "roeo.h"

int main() {

    roeo_bsp_init(); // 注册ROEO全部底层驱动


    int fd = roeo_dev_open("gpio"); // 打开GPIO设备，并分配描述符
    if (fd < 0) {
        printf("Failed to open GPIO device: %d\n", fd);
        return -1;
    }
    roeo_dev_ioctl(fd, ROEO_CMD_GPIO_SET_NUMBER, &(int){1}, sizeof(int)); // 配置GPIO设备的引脚号
    roeo_dev_ioctl(fd, ROEO_CMD_GPIO_SET_MODE, &(uint32_t){ROEO_DEV_GPIO_MODE_OUTPUT_PP}, sizeof(uint32_t)); // 配置GPIO设备的模式
    roeo_dev_write(fd, &(uint8_t){1}, sizeof(uint8_t)); // 向GPIO设备写1，输出GPIO高电平
    
    return 0;
}
```


