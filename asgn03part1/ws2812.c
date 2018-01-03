#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/kthread.h>
#include <linux/miscdevice.h>
#include "spi_ioctl.h"

#define DRIVER_NAME 		"ws2812"
#define DEVICE_NAME 		"ws2812"
#define DEVICE_CLASS_NAME 	"ws2812"

#define MAJOR_NUMBER    153     /* assigned */

static int c=0;

struct spidev_data {
	struct spi_device *spi;
};

static struct spidev_data *ws2812p;

static struct spi_device *spi_ws2812_device;
static struct miscdevice ws2812_misc_dev;
static struct class *ws2812_data_class;

/* WS2812 MODULE PARAMETERS */
static uint ws2812_spi_bus = 1;

static struct spi_message m;
static unsigned char xfer_tx[384]={0};

static struct spi_transfer t = {
			.tx_buf = &xfer_tx[0],
			.rx_buf = 0,
			.len = 384,
			.cs_change = 1,
			.bits_per_word = 8,
			.speed_hz = 4000000,
			 };

static ssize_t ws2812_transfer(void)
{
	int ret = 0;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	ret = spi_async(ws2812p->spi, &m);
	
	udelay(100);
	c++;
	return ret;
}

static ssize_t ws2812_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	int ret;
	int m=0, n=0, k=0, c=0;
	unsigned char led_ring_sequence[16][3];

	printk("WRITE called\n");

	ret = copy_from_user((void *)&led_ring_sequence, (void * __user)buf, sizeof(led_ring_sequence));

	k=0;
	for(m=0;m<16;m++)
	{
		for(n=0;n<3;n++)
		{
			c=0;
			while(c<8)
			{
				if((led_ring_sequence[m][n]&0x1) == 1)
					xfer_tx[k] = 0xE0;
				else if(((led_ring_sequence[m][n]&0x1) == 0))
					xfer_tx[k] = 0x80;
				
				led_ring_sequence[m][n] >>= 1;
				c++; k++;
			}
		}
	}


	ws2812_transfer();

	return ret;
}

static long ws2812_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch(cmd)
	{
		case RESET:
		{
			t.tx_buf = &xfer_tx[0];
			t.rx_buf = 0;
			t.len = 384;
			t.cs_change = 1;
			t.bits_per_word = 8;
			t.speed_hz = 4000000;
		}

	}

	return ret;
}

static int ws2812_open(struct inode *inode, struct file *filp)
{

	return 0;
}

static int ws2812_release(struct inode *inode, struct file *filp)
{
	int ret = 0;

	gpio_free(44);
	gpio_free(72);
	gpio_free(24);
	gpio_free(42);

	printk(KERN_INFO "ws2812 driver is released\n");
	return ret;
}

static struct file_operations ws2812_fops = {
  .owner   			= THIS_MODULE,
  .write   			= ws2812_write,
  .open    			= ws2812_open,
  .release 			= ws2812_release,
  .unlocked_ioctl   = ws2812_ioctl,
};

static int ws2812_probe(struct spi_device *spi)
{
	int ret = 0;
	struct device *dev;
	// struct spidev_data *ws2812p;

	printk(KERN_INFO "found device : %s on spi_bus:%d  chip_select:%d\n", spi->modalias, spi->master->bus_num, spi->chip_select);

	ws2812_misc_dev.minor = MISC_DYNAMIC_MINOR;
	ws2812_misc_dev.fops = &ws2812_fops;
	ws2812_misc_dev.name = DEVICE_NAME;
	misc_register(&ws2812_misc_dev);


	ws2812p = kzalloc(sizeof(*ws2812p), GFP_KERNEL);
	if(!ws2812p)
	{
		return -ENOMEM;
	}

	ws2812p->spi = spi;

	dev = device_create(ws2812_data_class, &spi->dev, MKDEV(MAJOR_NUMBER, 1), ws2812p, DEVICE_NAME);
	if(dev == NULL)
	{
		printk("ws2812 device creation failed\n");
		kfree(ws2812p);
		return -1;
	}

	return ret;
}

static int ws2812_remove(struct spi_device *spi)
{
	int ret = 0;

	ws2812p->spi = NULL;

	device_destroy(ws2812_data_class, MKDEV(MAJOR_NUMBER, 1));
	misc_deregister(&ws2812_misc_dev);
	kfree(ws2812p);
	printk("ws2812 device removed\n");

	return ret;
}

struct spi_board_info spi_ws2812_device_info = {
	.modalias = "ws2812",
	.max_speed_hz = 4000000,
	.bus_num = 1,     
	.chip_select = 1,
	.mode = 0,
};

static const struct spi_device_id ws2812_id[] = {
	{"ws2812", 0},
	{}
};
MODULE_DEVICE_TABLE(spi, ws2812_id);

static struct spi_driver ws2812_driver = {
	.driver = {
		.name	= "ws2812",
		.owner	= THIS_MODULE,
	},
	.probe		= ws2812_probe,
	.remove		= ws2812_remove,
	.id_table	= ws2812_id,
};


static int __init ws2812_init(void)
{
	int ret;

	struct spi_master *master;

	master = spi_busnum_to_master(spi_ws2812_device_info.bus_num);
	if( !master )
	{
		printk(KERN_ALERT "Error in adding master\n");
		return -ENODEV;
	}

	spi_ws2812_device = spi_new_device( master, &spi_ws2812_device_info );
	if( !spi_ws2812_device )
	{
		printk(KERN_ALERT "Error in adding device\n");
		return -ENODEV;
	}

	spi_ws2812_device->bits_per_word = 8;

	ret = spi_setup(spi_ws2812_device);
	if(ret)
		spi_unregister_device(spi_ws2812_device);
	else
		printk(KERN_INFO "Registered the WS2812 device on SPI bus %u\n", ws2812_spi_bus);


	/* initialize the spi driver for ws2812 */
#if 0
	/* Request static allocation of a device major number */
	ret = register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &ws2812_fops);
	if(ret < 0)
	{
		printk("WS2812 Device Registration Failed\n");
		return -1;
	}
	
	/* Populate sysfs entries */
	ws2812_data_class = class_create(THIS_MODULE, DEVICE_CLASS_NAME);
	if(ws2812_data_class == NULL)
	{
		printk("Class Creation Failed\n");
		unregister_chrdev(MAJOR_NUMBER, ws2812_driver.driver.name);
		return -1;
	}
#endif
	ret = spi_register_driver(&ws2812_driver);
	if(ret<0)
	{
		printk(KERN_ALERT "WS2812 SPI driver register failed\n");
		// class_destroy(ws2812_data_class);
		// unregister_chrdev(MAJOR_NUMBER, ws2812_driver.driver.name);
		return -1;
	}
	printk(KERN_INFO "WS2812 SPI driver registered\n");

#if 1
	/* Free the GPIOs for correct export later on */
	gpio_free(44);
	gpio_free(72);
	gpio_free(24);
	gpio_free(42);


	/* Request the required the GPIOs */
	gpio_request_one(44, GPIOF_DIR_OUT, "MOSI_MUX1");
	gpio_request_one(72, GPIOF_OUT_INIT_LOW, "MOSI_MUX2");
	gpio_request_one(24, GPIOF_DIR_OUT, "MOSI_SHIFT");


	/* Initiliase GPIO values */
	gpio_set_value_cansleep(44, 1);
	gpio_set_value_cansleep(72, 0);
	gpio_set_value_cansleep(24, 0);


#endif
#if 0
	/* Free the GPIOs for correct export later on */
	gpio_free(44);
	gpio_free(72);
	gpio_free(46);
	gpio_free(24);
	gpio_free(42);
	gpio_free(30);
	gpio_free(15);


	gpio_request(44, "MUX1");
	gpio_request(72, "MUX20");
	gpio_request(46, "MUX21");

	gpio_request(24, "SHIFT_MOSI");
	gpio_request(42, "SHIFY_SS");
	gpio_request(30, "SHIFT_SCK");
	gpio_request(15, "SS");

	gpio_direction_output(30, 0);
	gpio_direction_output(24, 0);
	gpio_direction_output(42, 0);
	// gpio_direction_output(72, 0);
	gpio_direction_output(15, 0);



	/* Request the required the GPIOs */
	// gpio_request_one(44, GPIOF_DIR_OUT, "MOSI_MUX1");
	// gpio_request_one(72, GPIOF_OUT_INIT_LOW, "MOSI_MUX2");
	// gpio_request_one(46, GPIOF_DIR_OUT, "SPI_SCK");
	// gpio_request_one(24, GPIOF_DIR_OUT, "MOSI_SHIFT");
	// gpio_request_one(42, GPIOF_DIR_OUT, "SS_SHIFT");
	// gpio_request_one(30, GPIOF_DIR_OUT, "SCK_SHIFT");
	// gpio_request_one(15, GPIOF_DIR_OUT, "SS_PIN");

	/* Initiliase GPIO values */
	gpio_set_value_cansleep(44, 1);
	// gpio_set_value_cansleep(72, 0);
	gpio_set_value_cansleep(46, 1);

	// gpio_set_value_cansleep(24, 0);
	// gpio_set_value_cansleep(42, 0);
	// gpio_set_value_cansleep(30, 0);
#endif
	printk("ws2812 driver initialized\n");

	return ret;
}

static void __exit ws2812_exit(void)
{
	spi_unregister_driver(&ws2812_driver);
	spi_unregister_device(spi_ws2812_device);
	printk("ws2812 driver exited\n");
}


MODULE_AUTHOR("Team 27");
MODULE_DESCRIPTION("WS2812 Driver");
MODULE_LICENSE("GPL");

module_init(ws2812_init);
module_exit(ws2812_exit);