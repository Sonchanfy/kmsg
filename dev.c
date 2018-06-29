#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/memory.h>
#include<linux/delay.h>
#include <linux/sched.h>   
#include <linux/kthread.h> 
#include <linux/err.h> 

/*******************驱动程序**********************/

static dev_t dev;
static struct cdev my_cdev;
static struct task_struct *task;
int count=0;


int kthread_fun(void *data){
	while(1)
	{
		//set_current_state(TASK_UNINTERRUPTIBLE);
        if(kthread_should_stop()) break;
		printk("FlyAudio driver test %d\n",count++);
		//schedule_timeout(HZ/250*25);//100ms
		msleep(100);
	}
	return 0;
}


/*static struct file_operations dev_flops={

	.owner = THIS_MODULE,
};
*/
static int  dev_init(void){
	
	int err;
	printk(KERN_EMERG"INIT\n");
/*	int ret;
	
	alloc_chrdev_region(&dev,0,1,"DATADEV");
	cdev_init(&my_cdev,&dev_flops);
	ret=cdev_add(&my_cdev,dev,1);
	if(ret<0)
	{
		printk(KERN_EMERG"cdev add faile\n");
		return ret;
	}
	
	*/
	task=kthread_create(kthread_fun,NULL,"task");
	 if(IS_ERR(task)){
     printk("Unable to start kernel thread.\n");
      err = PTR_ERR(task);
      task =NULL;
      return err;

    }

	wake_up_process(task);	
	return 0;
	
	
}

static void dev_exit(void){
	
	if(task){
   		 kthread_stop(task);
  	  task = NULL;
    }
	
	printk(KERN_EMERG"REMOVE\n");
/*	cdev_del(&my_cdev);
	unregister_chrdev_region(dev, 1);*/
	


}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
