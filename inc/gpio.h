#ifndef GPIO_H
#define GPIO_H

#include <sys/poll.h>

enum gpio_direction {
	GPIO_IN,
	GPIO_OUT
};

enum gpio_value {
	GPIO_LOW,
	GPIO_HIGH
};

enum gpio_edge {
	GPIO_NONE,
	GPIO_RISING,
	GPIO_FALLING,
	GPIO_BOTH
};

int gpio_export(int pin);
int gpio_unexport(int pin);
int gpio_direction(int pin, int direction);
int gpio_edge(int pin, int edge);
int gpio_read(int pin);
struct pollfd open_read_poll(int pin);
int gpio_write(int pin, int value);

#endif
