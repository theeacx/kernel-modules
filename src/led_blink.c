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

static int __init led_blink_init(void)
{
    int ret;

    pr_info("led_blink: init - blinking every second\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_blink: gpio_request failed\n");
        return ret;
    }

    gpio_direction_output(GPIO_LED, 0);

    timer_setup(&blink_timer, blink_callback, 0);
    mod_timer(&blink_timer, jiffies + HZ);

    pr_info("led_blink: started\n");
    return 0;
}

static void __exit led_blink_exit(void)
{
    pr_info("led_blink: exit\n");
    del_timer_sync(&blink_timer);
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_blink_init);
module_exit(led_blink_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("LED blinks once per second");
