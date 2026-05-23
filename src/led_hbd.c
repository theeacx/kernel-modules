#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define GPIO_LED 529

static struct task_struct *blink_thread;

static void dot(void)
{
    gpio_set_value(GPIO_LED, 1);
    msleep(200);
    gpio_set_value(GPIO_LED, 0);
    msleep(200);
}

static void dash(void)
{
    gpio_set_value(GPIO_LED, 1);
    msleep(600);
    gpio_set_value(GPIO_LED, 0);
    msleep(200);
}

static void letter_gap(void)
{
    msleep(400);
}

static void word_gap(void)
{
    msleep(1000);
}

static int blink_fn(void *data)
{
    while (!kthread_should_stop()) {
        /* H: .... */
        dot(); dot(); dot(); dot();
        letter_gap();
        /* A: .- */
        dot(); dash();
        letter_gap();
        /* P: .--. */
        dot(); dash(); dash(); dot();
        letter_gap();
        /* P: .--. */
        dot(); dash(); dash(); dot();
        letter_gap();
        /* Y: -.-- */
        dash(); dot(); dash(); dash();
        word_gap();
        /* B: -... */
        dash(); dot(); dot(); dot();
        letter_gap();
        /* D: -.. */
        dash(); dot(); dot();
        letter_gap();
        /* A: .- */
        dot(); dash();
        letter_gap();
        /* Y: -.-- */
        dash(); dot(); dash(); dash();
        word_gap();
        word_gap();
    }
    gpio_set_value(GPIO_LED, 0);
    return 0;
}

static int __init led_hbd_init(void)
{
    int ret;

    pr_info("led_hbd: init - Happy Birthday morse mode\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_hbd: gpio_request failed\n");
        return ret;
    }

    ret = gpio_direction_output(GPIO_LED, 0);
    if (ret) {
        pr_err("led_hbd: gpio_direction_output failed\n");
        gpio_free(GPIO_LED);
        return ret;
    }

    blink_thread = kthread_run(blink_fn, NULL, "led_hbd");
    if (IS_ERR(blink_thread)) {
        pr_err("led_hbd: kthread_run failed\n");
        gpio_free(GPIO_LED);
        return PTR_ERR(blink_thread);
    }

    pr_info("led_hbd: Happy Birthday morse started\n");
    return 0;
}

static void __exit led_hbd_exit(void)
{
    pr_info("led_hbd: exit\n");
    if (blink_thread)
        kthread_stop(blink_thread);
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_hbd_init);
module_exit(led_hbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("Happy Birthday morse code LED blinker");
