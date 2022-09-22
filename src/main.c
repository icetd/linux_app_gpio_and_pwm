#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <glob.h>

#include "gpio.h"
#include "pwm.h"

int get_file_count(char *path)
{
	int total, err;
	glob_t globres;
	
	err = glob(path, 0, NULL, &globres);
	if (err) {
		return 0;
	}

	for(int i = 1; i <= globres.gl_pathc; ++i) {
		total = i;
	}
	
	globfree(&globres);
	return total;
}

void set_serial_angle(int angle)
{
	int duty = angle * 11111 + 500000;
	set_pwm_duty(0, duty);
	start_pwm(0);
}

int main()
{
	gpio_unexport(38);
	destroy_pwm(0);

	if (gpio_export(38) < 0)
		goto Exit_GPIO;
	if (gpio_direction(38, GPIO_IN) < 0)
		goto Exit_GPIO;
	if (gpio_edge(38, GPIO_BOTH) < 0)
		goto Exit_GPIO;
	
	if (init_pwm(0) < 0)
		goto Exit_PWM;
	if (set_pwm_period(0, 3000000) < 0)
		goto Exit_PWM;
	if (set_pwm_duty(0, 0) < 0)
		goto Exit_PWM;

	int ret, status, num;
	struct pollfd fds;
	char buff[10];
	fds = open_read_poll(38);

	char cmd[1024];
	char* cmd1="rosbag record -O livox_record";
	char cmd2[1024];
	char* cmd3=".bag /livox/imu /livox/lidar  > /dev/null 2>&1 &";

	while(1)
	{
		ret = read(fds.fd, buff, 10);
		if(ret < 0)
			perror("read gpio failed");
		
		ret = poll(&fds, 1, 0);
		if(ret < 0)
			perror("poll failed");

		if(fds.revents & POLLPRI)
		{
			ret = lseek(fds.fd, 0, SEEK_SET);
			if(ret < 0)
				perror("lseek failed");
			status = atoi(buff);
			
			switch (status) {
			case 0:
				chdir("/home/USB");
				num = get_file_count("/home/USB/*.bag");
				memset(cmd, 0, sizeof(cmd));
				sprintf(cmd2, "%d", num);
				strcat(cmd, cmd1);
				strcat(cmd, cmd2);
				strcat(cmd, cmd3);
				system("roslaunch livox_ros_driver livox_lidar_msg.launch > /dev/null 2>&1 &");			
				system(cmd);
				printf("SWITCH ON\n");
				
				sleep(5);
				set_serial_angle(90);

				break;
			case 1:
				system("killall record");
				system("killall livox_ros_driver_node");
				printf("SWITCH OFF\n");

				sleep(5);
				set_serial_angle(0);
				break;
			}
		}
		
		usleep(20);
	}

	close(fds.fd);

Exit_PWM:
	destroy_pwm(0);

Exit_GPIO:
	gpio_unexport(38);
	return 0;
}

