#include <gpiod.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define CHIPNAME "gpiochip0"

// GPIO lines
#define PIN_T1 77   // 100 ms
#define PIN_T2 78   // 1 s
#define PIN_T3 81   // 1 ms

struct gpiod_chip *chip;
struct gpiod_line *t1, *t2, *t3;

// helper: toggle GPIO
void pulse(struct gpiod_line *line, int high_us) {
    gpiod_line_set_value(line, 1);
    usleep(high_us);
    gpiod_line_set_value(line, 0);
}

void* thread_100ms(void* arg) {
    while (1) {
        pulse(t1, 1000);     // short pulse for visibility
        usleep(100000);      // 100 ms cycle
    }
}

void* thread_1s(void* arg) {
    while (1) {
        pulse(t2, 1000);
        sleep(1);
    }
}

void* thread_1ms(void* arg) {
    while (1) {
        pulse(t3, 100);
        usleep(1000);       // 1 ms cycle
    }
}

int main() {

    pthread_t th1, th2, th3;

    // 1. Open GPIO chip
    chip = gpiod_chip_open_by_name(CHIPNAME);

    // 2. Get lines
    t1 = gpiod_chip_get_line(chip, PIN_T1);
    t2 = gpiod_chip_get_line(chip, PIN_T2);
    t3 = gpiod_chip_get_line(chip, PIN_T3);

    // 3. Request output mode
    gpiod_line_request_output(t1, "t1", 0);
    gpiod_line_request_output(t2, "t2", 0);
    gpiod_line_request_output(t3, "t3", 0);

    // 4. Start threads
    pthread_create(&th1, NULL, thread_100ms, NULL);
    pthread_create(&th2, NULL, thread_1s, NULL);
    pthread_create(&th3, NULL, thread_1ms, NULL);

    // 5. Wait forever
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    // cleanup (never reached)
    gpiod_chip_close(chip);

    return 0;
}