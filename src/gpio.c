#include "gpio.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>


int gpio_export(int pin)
{	
	int fd, len;
	char buffer[128];

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0) {
		perror("Failed open export.");
		return -1;
	}

	len = snprintf(buffer, sizeof(buffer), "%d", pin);
	if (write(fd, buffer, len) < 0) {
		close(fd);
		perror("Falied to export gpio.");
		return -1;
	}

	close(fd);
	usleep(200000);
	return 0;
}

int gpio_unexport(int pin)
{
	int fd, len;
	char buffer[128];

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (fd < 0) {
		perror("Failed open unexport.");
		return -1;
	}

	len = snprintf(buffer, sizeof(buffer), "%d", pin);
	write(fd, buffer, len);

	close(fd);
	return 0;
}

int gpio_read(int pin)
{
	int fd;
	char path[128];
	char value[1];

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror("Failed to open gpio_value for read.");
		return -1;
	}

	if (read(fd, &value, 1) < 0) {
		perror("Failed to read value.");
		return -1;
	}
	
	close (fd);
	return atoi(value);
}

struct pollfd open_read_poll(int pin)
{
	struct pollfd fds;
	int fd;
	char path[128];
	char value[1];
	
	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror("Failed to open gpio_value for read.");
	}

	fds.fd = fd;
	fds.events = POLLPRI;

	return fds;
}

int gpio_write(int pin, int value)
{
	int fd, ret;
	char path[128];
	
	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror("Failed to open gpio_value for write.");
		return -1;
	}

	switch(value) {
	case GPIO_LOW:
		ret = write(fd, "0", 1);
		break;
	case GPIO_HIGH:
		ret = write(fd, "1", 1);
		break;
	}

	if(ret < 0) {
		perror("Failed to write value.");
		return -1;
	}

	close(fd);
	return 0;
}

int gpio_direction(int pin, int direction)
{
	int fd, ret;
	char path[128];
	
	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		perror("Failed to open gpio_direction for write.");
		return -1;
	}

	switch(direction) {
	case GPIO_IN:
		ret = write(fd, "in", sizeof("in"));
		break;
	case GPIO_OUT:
		ret = write(fd, "out", sizeof("out"));
		break;
	}

	if(ret < 0) {
		perror("Failed to write direction.");
		return -1;
	}

	close(fd);
	return 0;
}

int gpio_edge(int pin, int edge)
{
	int fd, ret;
	char path[128];
	
	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/edge", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		perror("Failed to open gpio_edge for write.");
		return -1;
	}

	switch(edge) {
	case GPIO_NONE:
		ret = write(fd, "none", sizeof("none"));
		break;
	case GPIO_RISING:
		ret = write(fd, "rising", sizeof("rising"));
		break;
	case GPIO_FALLING:
		ret = write(fd, "falling", sizeof("falling"));
		break;
	case GPIO_BOTH:
		ret = write(fd, "both", sizeof("both"));
		break;	
	}

	if(ret < 0) {
		perror("Failed to write edge.");
		return -1;
	}

	close(fd);
	return 0;
}


