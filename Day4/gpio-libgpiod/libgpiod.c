/*
 * libgpiod.c
 * 
 * author: 	Eugen Wiens
 * 
 * This file contains functions to initialize and control GPIO lines using the libgpiod library.
 * 
 */

#include "libgpiod.h"

#include <stdio.h>
#include <unistd.h>


gpiod_line_s* init_gpio(const char* chip_name, unsigned int line_number)
{
    if (chip_name == NULL) {
        fprintf(stderr, "Chip name is NULL\n");
        return NULL;
    }

    printf("Initializing GPIO\n");
    struct gpiod_chip *chip = gpiod_chip_open_by_name(chip_name);
    
    if (chip == NULL) {
        fprintf(stderr, "Could not open chip %s\n", chip_name);
        return NULL;
    }
    
    gpiod_line_s *linePin = gpiod_chip_get_line(chip, line_number);
    gpiod_line_request_output(linePin, "example_consumer", 0);

    return linePin;
}

void set_gpio(gpiod_line_s* line, unsigned int value)
{
    if (line == NULL) {
        fprintf(stderr, "Line is NULL\n");
        return;
    }
    
    gpiod_line_set_value(line, value);
}
