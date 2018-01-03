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

#include "spi_ioctl.h"

#define SPI_DEVICE_NAME "/dev/ws2812"

unsigned char patternArr[16][3] = {{255, 0, 0 },
									{190, 0, 0},
									{127, 0, 0},
									{63, 0, 0 },
									{0, 255, 0},
									{0, 190, 0},
									{0, 127, 0},
									{0, 64, 0},
									{0, 0, 255},
									{0, 0, 190},
									{0, 0, 127},
									{0, 0, 64},
									{127, 127, 0},
									{64, 64, 0},
									{0, 127, 127},
									{0, 64, 64}};

unsigned char rainbowArr[16][3] = {{130, 238, 238},
									{0, 75, 130},
									{0, 0, 255},
									{255, 0, 0},
									{255, 255, 0},
									{165, 255, 0},
									{0, 255, 0},
									{0, 255, 0},
									{165, 255, 0},
									{255, 255, 0},
									{255, 0, 0},
									{0, 0, 255},
									{0, 75, 130},
									{130, 238, 238},
									{0, 75, 130},
									{0, 0, 255}};

unsigned char colorful[16][3] = {{100,0,0},
								{139,34,34},
								{205,50,50},
								{252,124,0},
								{205,154,50},
								{0, 128, 0},
								{20,220,60},
								{99,255,71},
								{128,240,128},
								{128,250,114},
								{79, 47, 79},
								{139,0,139},
								{255,0,255},
								{206,0,209},
								{224,64,208},
								{255,127,212}};

unsigned char zeroes[16][3] = {	{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								};

// unsigned char mypattern[16][3] = {};

unsigned char sendLEDpattern[16][3];

unsigned char temppattern[1][3] = {{0, 0, 0}};

unsigned char pattern[384];

/* WRITE call to pass the pattern sequence display to the SPI Driver */
int spi_led_write(int fd)
{
	int retValue=0;

	retValue = write(fd, sendLEDpattern, sizeof(sendLEDpattern));
	if(retValue < 0)
	{
		printf("SPI led ring write error\n");
	}

	return retValue;
}

int main()
{
	int ret, fd;
	int i, j;
	int COUNT = 0;
	int num;

	printf("Enter number of LEDs to light up\n");
	scanf("%d", &num);

	for(i=0;i<16;i++)
	{
		for(j=0;j<3;j++)
		{
			if(i<num)
			{
				sendLEDpattern[i][j] = rainbowArr[i][j];
			}
			else{
				sendLEDpattern[i][j] = zeroes[i][j];
			}
		}
	}


	fd = open(SPI_DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("Cannot open device file fd_spi.\n");
		return 0;
	}

	ret = ioctl(fd, RESET, NULL);
	if(ret<0)
	{
		printf("Error in ioctl command: %d\n", ret);
	}

	ret = spi_led_write(fd);
	if(ret < 0)
	{
		printf("Error in writing sequence to LED");
	}

	while(1)
	{
		for(i=0;i<16;i++)
		{
			for(j=0;j<3;j++)
			{
				if(i==0)
				{
					temppattern[i][j] = sendLEDpattern[i][j];
				}
				if(i!=15)
				{
					sendLEDpattern[i][j] = sendLEDpattern[i+1][j];
				}
				else{
					sendLEDpattern[i][j] = temppattern[0][j];
				}
			}
		}
		usleep(250000);
		ret = spi_led_write(fd);
		if(ret < 0)
		{
			printf("Error in writing sequence to LED");
		}

		COUNT++;
	}

	close(fd);

	return 0;
}
