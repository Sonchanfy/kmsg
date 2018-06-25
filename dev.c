#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/memory.h>

static dev_t dev;
static struct cdev my_cdev;
char j='0';
static ssize_t dev_read(struct file *file, char __user *buf,size_t count,loff_t *ppos){
	
	char data[38]="read-------ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char kdata[39];
	memset(kdata,0,sizeof(kdata));
	if(j>'9')
	{
		j='0';
	}

	sprintf(kdata,"%c%s",j++,data);
	copy_to_user(buf,kdata,sizeof(kdata));
	printk(KERN_EMERG"%s",kdata);
	memset(kdata,0,sizeof(kdata));

	
	return sizeof(kdata); 

}

static struct file_operations dev_flops={

	.owner = THIS_MODULE,
	.read  = dev_read,
};

static int  dev_init(void){
	int ret;
	printk(KERN_EMERG"INIT\n");
	alloc_chrdev_region(&dev,0,1,"DATADEV");
	cdev_init(&my_cdev,&dev_flops);
	ret=cdev_add(&my_cdev,dev,1);
	if(ret<0)
	{
		printk(KERN_EMERG"cdev add faile\n");
		return ret;
	}
	return 0;
	
	
}

static void dev_exit(void){

	printk(KERN_EMERG"REMOVE\n");
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev, 1);

}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
