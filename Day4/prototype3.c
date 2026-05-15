#include <gpiod.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

// GPIO chip + pins
#define CHIPNAME "gpiochip0"
#define PIN_T1 17
#define PIN_T2 27
#define PIN_T3 22

struct gpiod_chip *chip;
struct gpiod_line *t1, *t2, *t3;

// helper: nanosleep wrapper
void sleep_ns(long sec, long nsec) {
    struct timespec req;
    req.tv_sec = sec;
    req.tv_nsec = nsec;
    nanosleep(&req, NULL);
}

// GPIO pulse
void pulse(struct gpiod_line *line, long high_ns) {
    gpiod_line_set_value(line, 1);
    sleep_ns(0, high_ns);
    gpiod_line_set_value(line, 0);
}

void* thread_100ms(void* arg) {
    struct timespec cycle = {0, 100000000L}; // 100 ms

    while (1) {
        pulse(t1, 1000000L);  // 1 ms pulse
        nanosleep(&cycle, NULL);
    }
}

void* thread_1s(void* arg) {
    struct timespec cycle = {1, 0}; // 1 second

    while (1) {
        pulse(t2, 1000000L);
        nanosleep(&cycle, NULL);
    }
}

void* thread_1ms(void* arg) {
    struct timespec cycle = {0, 1000000L}; // 1 ms

    while (1) {
        pulse(t3, 100000L); // 0.1 ms pulse
        nanosleep(&cycle, NULL);
    }
}

int main() {

    pthread_t th1, th2, th3;

    // open GPIO chip
    chip = gpiod_chip_open_by_name(CHIPNAME);

    // get lines
    t1 = gpiod_chip_get_line(chip, PIN_T1);
    t2 = gpiod_chip_get_line(chip, PIN_T2);
    t3 = gpiod_chip_get_line(chip, PIN_T3);

    // request output
    gpiod_line_request_output(t1, "t1", 0);
    gpiod_line_request_output(t2, "t2", 0);
    gpiod_line_request_output(t3, "t3", 0);

    // start threads
    pthread_create(&th1, NULL, thread_100ms, NULL);
    pthread_create(&th2, NULL, thread_1s, NULL);
    pthread_create(&th3, NULL, thread_1ms, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    gpiod_chip_close(chip);

    return 0;
}