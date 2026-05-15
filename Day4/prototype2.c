#include <gpiod.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#define CHIP "gpiochip0"

#define PIN_100MS 17
#define PIN_1S    27
#define PIN_1MS   22

struct gpiod_chip *chip;
struct gpiod_line *l100, *l1s, *l1ms;

// Zeit addieren (nanosekunden-sicher)
void add_time(struct timespec *t, long ns) {
    t->tv_nsec += ns;
    while (t->tv_nsec >= 1000000000) {
        t->tv_nsec -= 1000000000;
        t->tv_sec++;
    }
}

// Jitter messen
long diff_ns(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * 1000000000L +
           (b.tv_nsec - a.tv_nsec);
}

// ---------------- THREAD 1 (100ms) ----------------
void* thread_100ms(void* arg) {

    struct timespec next, now;

    clock_gettime(CLOCK_MONOTONIC, &next);

    while (1) {

        gpiod_line_set_value(l100, 1);
        gpiod_line_set_value(l100, 0);

        add_time(&next, 100000000); // 100 ms

        clock_gettime(CLOCK_MONOTONIC, &now);
        long jitter = diff_ns(next, now);

        printf("[100ms] jitter: %ld ns\n", jitter);

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    }
}

// ---------------- THREAD 2 (1s) ----------------
void* thread_1s(void* arg) {

    struct timespec next, now;

    clock_gettime(CLOCK_MONOTONIC, &next);

    while (1) {

        gpiod_line_set_value(l1s, 1);
        gpiod_line_set_value(l1s, 0);

        add_time(&next, 1000000000); // 1 s

        clock_gettime(CLOCK_MONOTONIC, &now);
        long jitter = diff_ns(next, now);

        printf("[1s] jitter: %ld ns\n", jitter);

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    }
}

// ---------------- THREAD 3 (1ms) ----------------
void* thread_1ms(void* arg) {

    struct timespec next, now;

    clock_gettime(CLOCK_MONOTONIC, &next);

    while (1) {

        gpiod_line_set_value(l1ms, 1);
        gpiod_line_set_value(l1ms, 0);

        add_time(&next, 1000000); // 1 ms

        clock_gettime(CLOCK_MONOTONIC, &now);
        long jitter = diff_ns(next, now);

        printf("[1ms] jitter: %ld ns\n", jitter);

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    }
}

// ---------------- MAIN ----------------
int main() {

    pthread_t t1, t2, t3;

    chip = gpiod_chip_open_by_name(CHIP);

    l100 = gpiod_chip_get_line(chip, PIN_100MS);
    l1s  = gpiod_chip_get_line(chip, PIN_1S);
    l1ms = gpiod_chip_get_line(chip, PIN_1MS);

    gpiod_line_request_output(l100, "t100", 0);
    gpiod_line_request_output(l1s,  "t1s", 0);
    gpiod_line_request_output(l1ms, "t1ms", 0);

    pthread_create(&t1, NULL, thread_100ms, NULL);
    pthread_create(&t2, NULL, thread_1s, NULL);
    pthread_create(&t3, NULL, thread_1ms, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    gpiod_chip_close(chip);

    return 0;
}