/**
 * @file libgpiod.h
 * 
 * @author Eugen Wiens
 * 
 * @brief GPIO library header for handling GPIO lines.
 *        his file contains functions to initialize and control GPIO 
 *        lines using the libgpiod library.
 * 
 */
#pragma once

#include <gpiod.h>


/**
 * @typedef gpiod_line_s
 * @brief Typedef for the GPIO line structure.
 */
typedef struct gpiod_line gpiod_line_s;

/**
 * @brief Initializes a GPIO line.
 *
 * This function initializes a GPIO line specified by the chip name and line number.
 *
 * @param chip_name The name of the GPIO chip.
 * @param line_number The number of the GPIO line to initialize.
 * @return A pointer to the initialized gpiod_line_s structure.
 */
gpiod_line_s* init_gpio(const char* chip_name, unsigned int line_number);

/**
 * @brief Sets the value of a GPIO line.
 *
 * This function sets the value of the specified GPIO line.
 *
 * @param line A pointer to the gpiod_line_s structure representing the GPIO line.
 * @param value The value to set the GPIO line to (e.g., 0 or 1).
 */
void set_gpio(gpiod_line_s* line, unsigned int value);