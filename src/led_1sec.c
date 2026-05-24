#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/timer.h>

#define GPIO_LED 529

static struct timer_list blink_timer;
static int led_state = 0;

static void blink_callback(struct timer_list *t)
{
    led_state = !led_state;
    gpio_set_value(GPIO_LED, led_state);
    mod_timer(&blink_timer, jiffies + HZ);
}

static int __init led_hbd_init(void)
{
    int ret;

    pr_info("led_hbd: init - blink every 1s\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_hbd: gpio_request failed\n");
        return ret;
    }

    gpio_direction_output(GPIO_LED, 1);
    led_state = 1;

    timer_setup(&blink_timer, blink_callback, 0);
    mod_timer(&blink_timer, jiffies + HZ);

    pr_info("led_hbd: blinking started\n");
    return 0;
}

static void __exit led_hbd_exit(void)
{
    pr_info("led_hbd: exit\n");
    del_timer_sync(&blink_timer);
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_hbd_init);
module_exit(led_hbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("LED blink every second");