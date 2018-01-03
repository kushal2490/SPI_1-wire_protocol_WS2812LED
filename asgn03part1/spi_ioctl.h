#include <linux/ioctl.h>

#define IOC_MAGIC 'k'
#define RESET _IOW(IOC_MAGIC, 0, unsigned long) 
 
