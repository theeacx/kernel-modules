#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define GPIO_LED 529

static int __init led_hbd_init(void)
{
    int ret;

    pr_info("led_hbd: init - LED ON mode\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_hbd: gpio_request failed\n");
        return ret;
    }

    ret = gpio_direction_output(GPIO_LED, 1);
    if (ret) {
        pr_err("led_hbd: gpio_direction_output failed\n");
        gpio_free(GPIO_LED);
        return ret;
    }

    pr_info("led_hbd: LED is ON\n");
    return 0;
}

static void __exit led_hbd_exit(void)
{
    pr_info("led_hbd: exit - turning LED off\n");
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_hbd_init);
module_exit(led_hbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("Simple LED always-on module");