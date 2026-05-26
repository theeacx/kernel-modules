#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/timer.h>

#define GPIO_LED 529

/* Timing (in jiffies, HZ = 1 second) */
#define DOT_MS      200
#define DASH_MS     600
#define INTRA_MS    200   /* gap between dots/dashes within a letter */
#define LETTER_MS   600   /* gap between letters  (3 * DOT) */
#define WORD_MS     1400  /* gap after full SOS before repeat (7 * DOT) */

#define MS_TO_JIFFIES(ms) (HZ * (ms) / 1000)

/*
 * Morse SOS: S = ...   O = ---   S = ...
 * Encoded as durations: positive = LED ON, negative = LED OFF (gap)
 * The sequence ends with 0 as sentinel.
 */
static int morse_sos[] = {
    /* S: . . . */
     DOT_MS, -INTRA_MS,  DOT_MS, -INTRA_MS,  DOT_MS, -LETTER_MS,
    /* O: - - - */
    DASH_MS, -INTRA_MS, DASH_MS, -INTRA_MS, DASH_MS, -LETTER_MS,
    /* S: . . . */
     DOT_MS, -INTRA_MS,  DOT_MS, -INTRA_MS,  DOT_MS, -WORD_MS,
    /* sentinel */
    0
};

static struct timer_list morse_timer;
static int step = 0;

static void morse_callback(struct timer_list *t)
{
    int val = morse_sos[step];

    if (val == 0) {
        /* loop back to start */
        step = 0;
        val = morse_sos[step];
    }

    if (val > 0) {
        gpio_set_value(GPIO_LED, 1);
        mod_timer(&morse_timer, jiffies + MS_TO_JIFFIES(val));
    } else {
        gpio_set_value(GPIO_LED, 0);
        mod_timer(&morse_timer, jiffies + MS_TO_JIFFIES(-val));
    }

    step++;
}

static int __init led_sos_init(void)
{
    int ret;

    pr_info("led_sos: init - blinking SOS in Morse code\n");

    ret = gpio_request(GPIO_LED, "led_gpio");
    if (ret) {
        pr_err("led_sos: gpio_request failed\n");
        return ret;
    }

    gpio_direction_output(GPIO_LED, 0);

    timer_setup(&morse_timer, morse_callback, 0);
    step = 0;
    mod_timer(&morse_timer, jiffies + MS_TO_JIFFIES(100));

    pr_info("led_sos: started\n");
    return 0;
}

static void __exit led_sos_exit(void)
{
    pr_info("led_sos: exit\n");
    del_timer_sync(&morse_timer);
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
}

module_init(led_sos_init);
module_exit(led_sos_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thea");
MODULE_DESCRIPTION("LED blinks SOS in Morse code");