#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define GPIO_LED 529

static struct task_struct *blink_thread;

static int blink_fn(void *data)
{
    while (!kthread_should_stop()) {
        gpio_set_value(GPIO_LED, 1);
        msleep(1000);
        gpio_set_value(GPIO_LED, 0);
        msleep(1000);
    }
    gpio_set_value(GPIO_LED, 0);
    return 0;
}

static int __init led_blink_init(void)
{
    int ret;

    pr_info("led_blink: init - 1s on/off mode\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_blink: gpio_request failed\n");
        return ret;
    }

    ret = gpio_direction_output(GPIO_LED, 0);
    if (ret) {
        pr_err("led_blink: gpio_direction_output failed\n");
        gpio_free(GPIO_LED);
        return ret;
    }

    blink_thread = kthread_run(blink_fn, NULL, "led_blink");
    if (IS_ERR(blink_thread)) {
        pr_err("led_blink: kthread_run failed\n");
        gpio_free(GPIO_LED);
        return PTR_ERR(blink_thread);
    }

    pr_info("led_blink: blinking started (1s interval)\n");
    return 0;
}

static void __exit led_blink_exit(void)
{
    pr_info("led_blink: exit\n");
    if (blink_thread)
        kthread_stop(blink_thread);
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_blink_init);
module_exit(led_blink_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("Raspberry Pi LED blink 1s on/off kernel module");