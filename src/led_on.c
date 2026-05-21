#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define GPIO_LED 529

static int __init led_on_init(void)
{
    int ret;

    pr_info("led_on: init\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_on: gpio_request failed\n");
        return ret;
    }

    ret = gpio_direction_output(GPIO_LED, 1);
    if (ret) {
        pr_err("led_on: gpio_direction_output failed\n");
        gpio_free(GPIO_LED);
        return ret;
    }

    gpio_set_value(GPIO_LED, 1);
    pr_info("led_on: LED ON\n");
    return 0;
}

static void __exit led_on_exit(void)
{
    pr_info("led_on: exit\n");
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_on_init);
module_exit(led_on_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("Raspberry Pi LED always ON kernel module"); 
 
 
 
 
 
