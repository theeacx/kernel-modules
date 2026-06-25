#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define GPIO_LED 529

static int __init led_on_init(void)
{
    int ret;

    pr_info("led_on: init - turning LED on\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_on: gpio_request failed\n");
        return ret;
    }

    gpio_direction_output(GPIO_LED, 1);

    pr_info("led_on: LED is on\n");
    return 0;
}

static void __exit led_on_exit(void)
{
    pr_info("led_on: exit - turning LED off\n");
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_on_init);
module_exit(led_on_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("LED stays on continuously");
