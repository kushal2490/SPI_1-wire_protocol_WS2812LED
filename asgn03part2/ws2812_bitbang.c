////////////////////////////////////////////////////////////////////////////////////
//CSE 530 Assignment 3 Part 2
//Authors: Team 27 (Aakanksha Budhiraja, Kushal Anil Parmar)
//Driver for WS2812 LED strip using "bit-banging" approach
//////////////////////////////////////////////////////////////////////////////////

#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/irqflags.h>

#include <linux/irq.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/mfd/intel_qrk_gip_pdata.h>

#define DRIVER_NAME 		"WS2812"
#define DEVICE_NAME 		"WS2812"
#define DEVICE_CLASS	 	"ws2812"
#define DEVICES 1

#define	VENDOR_ID			0x8086
#define DEVICE_ID			0x0934

/* PCI BAR for register base address */
#define GIP_I2C_BAR			0
#define GIP_GPIO_BAR		1
#define PORTA_DATA			0x00	/* Data */

static void __iomem *reg_base;
static dev_t device_no;
static struct cdev cdev_info;
static struct class *class_data;
u32 val_data = 0;
u32 val_data_dummy = 0;
unsigned long flags = 0;
unsigned int MAJOR_NUMBER = 00, MINOR_NUMBER = 0;
uint64_t arr[24];
unsigned char ledstring[3*16];
unsigned char rotate_string[16];
struct pci_dev *pdev;

/* time stamp counter to retrieve the CPU cycles */
uint64_t tsc(void)
{
	uint32_t a, d;
	asm volatile("rdtsc" : "=a" (a), "=d" (d));
	return (( (uint64_t)a)|( (uint64_t)d)<<32 );
}

/* ws2812 device write function to display the led pattern received from user */
static ssize_t ws2812_write(struct file *filp, const char *buf, size_t counter, loff_t *ppos)
{
	int ret=0;
	int COUNT = 0;
	unsigned offset = 0x04;
	void __iomem *reg_data;
	void __iomem *reg_data_dummy;
	struct pci_dev *pdev;
	resource_size_t start = 0, len = 0;

	//Copy bitbang pattern from user to buffer
	copy_from_user((void *)&rotate_string, (void * __user)buf, sizeof(rotate_string));

	flags = 0;
	val_data = 0;
	pdev = pci_get_device(VENDOR_ID, DEVICE_ID, NULL);

	/* Determine the address of the GPIO area */
	start = pci_resource_start(pdev, GIP_GPIO_BAR);
	len = pci_resource_len(pdev, GIP_GPIO_BAR);
	reg_base = ioremap_nocache(start, len);

	//disable interrupts
	local_irq_disable();

	/*****************************Reset*******************************/
	reg_data = reg_base + PORTA_DATA;
	reg_data_dummy = reg_base + PORTA_DATA;
	val_data = ioread32(reg_data_dummy);
	iowrite32(val_data & ~BIT(offset % 32), reg_data);
	ndelay(2500);		//low for atleast 50us to reset the LED strip
	/*****************************************************************/

	// Display the LED pattern for 16 LEDs
	for(COUNT=0;COUNT<16;COUNT++)
	{

	if(rotate_string[COUNT] == 'B')
	{
	/**********************************************LED BLUE*****************************************************/

	////////////////////////BYTE  = 00000000////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 2
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	////////////////////////////////////////////////////////////////////////////////
	

	////////////////////////BYTE  = 00000000////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 2
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////BYTE = 11111111 ////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250); 
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 2
		// reg_data = reg_base + PORTA_DATA;
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	/////////////////////////////////////////////////////////////////////////////////
	/**********************************************LED 1 END*****************************************************/
}

if(rotate_string[COUNT] == 'G')  //Send 255,0,0
{
	/**********************************************LED GREEN*****************************************************/

	/////////////////////////BYTE = 11111111 ////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250); 
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 2
		// reg_data = reg_base + PORTA_DATA;
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////BYTE  = 00000000////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 2
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////BYTE  = 00000000////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 2
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	////////////////////////////////////////////////////////////////////////////////////

	/*************************LED GREEN END********************************************/
}

if(rotate_string[COUNT] == 'R')	//Send 0,255,0
{
	/**********************************************LED RED***********************************************************/

	////////////////////////////BYTE  = 00000000////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 2
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////BYTE = 11111111 ////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250); 
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 2
		// reg_data = reg_base + PORTA_DATA;
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
				
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		ndelay(250);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////BYTE  = 00000000////////////////////////////////////////		
		//bit 0
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 1
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 2
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 3
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 4
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 5
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
		
		//bit 6
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);

		//bit 7
		val_data = ioread32(reg_data);
		iowrite32(val_data | BIT(offset % 32), reg_data);
		iowrite32(val_data & ~BIT(offset % 32), reg_data);
	////////////////////////////////////////////////////////////////////////////////////
	/*******************************LED RED END****************************************/
}
}
	//enable interrupts
	local_irq_enable();
	return ret;
}

/* ws2812 device open function */
static int ws2812_open(struct inode *inode, struct file *filp)
{
	return 0;
}

/* ws2812 device release function */
static int ws2812_release(struct inode *inode, struct file *filp)
{
	int ret = 0;
	printk(KERN_INFO "ws2812 driver is released\n");
	return ret;
}

static struct file_operations ws2812_fops = {
  .owner   			= THIS_MODULE,		/* Owner */
  .write   			= ws2812_write,		/* Write method */
  .open    			= ws2812_open,		/* Open method */
  .release 			= ws2812_release,	/* Release method */
};

/* ws2812 device initialisation function */
static int __init ws2812_init(void)
{
	int ret = 1;

	ret=alloc_chrdev_region(&device_no,MINOR_NUMBER,DEVICES,DEVICE_NAME);

	if(!ret)
	{
		MAJOR_NUMBER=MAJOR(device_no);
	}
	else
	{
		goto RETURN_ERROR; //ERROR cant register
	}


	if((class_data=class_create(THIS_MODULE,DEVICE_CLASS))==NULL)
	{
		goto UNREGISTER;
	}

	if(device_create(class_data,NULL,device_no,NULL,DEVICE_NAME)==NULL)
	{
		goto CLASS_DESTROY;
	}

	cdev_init(&cdev_info,&ws2812_fops);

	if(cdev_add(&cdev_info,device_no,DEVICES)<0)
	{
		goto DEVICE_DESTROY;
	}
	MAJOR_NUMBER=MAJOR(device_no);

	//configure gpio
	gpio_request(28, "BB_SHIFT");
	gpio_request(45, "BB_MUX");
	gpio_set_value_cansleep(45, 0);
	gpio_request(12, "BitBang");
	gpio_direction_output(28, 0);
	gpio_direction_output(12, 0);

	printk(KERN_ALERT "WS2812 driver initialized with major:%d\n", MAJOR_NUMBER);;
	return 0;

	DEVICE_DESTROY: device_destroy(class_data,device_no);
	CLASS_DESTROY: class_destroy(class_data);
	UNREGISTER: unregister_chrdev_region(device_no,DEVICES);
	RETURN_ERROR: return -1;
}

static void __exit ws2812_exit(void)
{
	//free gpio
	gpio_free(12);
	gpio_free(45);
	gpio_free(28);
	//unregister and destroy
	iounmap(reg_base);
	cdev_del(&cdev_info);
	device_destroy(class_data,device_no);
	class_destroy(class_data);
	unregister_chrdev_region(device_no,DEVICES);
	printk("ws2812 driver removed\n");
}

MODULE_AUTHOR("Team 27");
MODULE_DESCRIPTION("WS2812 Driver");
MODULE_LICENSE("GPL");

module_init(ws2812_init);
module_exit(ws2812_exit);