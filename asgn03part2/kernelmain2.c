////////////////////////////////////////////////////////////////
//CSE 530 Assignment 3 Part2 
//Authors: Team 27 (Aakanksha Budhiraja, Kushal Anil Parmar)
//User Program to test WS2812 driver that uses gpio bitbanging
//to light the WS2812 led strip
////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <time.h>
#include <poll.h>
#include <pthread.h>
#include <linux/spi/spidev.h>


#define SPI_DEVICE_NAME "/dev/WS2812"

unsigned char bitbang_pattern[16] = {'R','R','R','B','B','B','G','G','G','R','R','R','B','B','B','B'};		//pattern to display on the WS2812 LED strip

/* WRITE call to pass the pattern sequence display to the SPI Driver */
int spi_led_write(int fd)
{
	int retValue=0;
	retValue = write(fd, bitbang_pattern, sizeof(bitbang_pattern));
	if(retValue < 0)
	{
		printf("SPI led ring write error %d\n", retValue);
	}
	return retValue;
}

int main()
{
	int ret, fd, j;
	int rotate = 0;
	unsigned char temp_string = NULL;

	fd = open(SPI_DEVICE_NAME, O_RDWR);		//open spi device
	if(fd < 0)
	{
		printf("Cannot open device file fd_spi.\n");
		return 0;
	}

	//rotate 10 times
	while(rotate<10)						
	{
		ret = spi_led_write(fd);
		if(ret < 0)
		{
			printf("Error in writing sequence to LED");
		}

		/************ROTATE COLOR ARRAY***************/
		temp_string = bitbang_pattern[0];
		for(j=0;j<15;j++)
		{	
			bitbang_pattern[j] = bitbang_pattern[j+1];

		}
		bitbang_pattern[15] = temp_string;
		/********************************************/

		rotate++;
		sleep(1);		//delay between different patterns
	}
	close(fd);		//close spi device
	return 0;
}
