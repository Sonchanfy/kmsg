#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/klog.h>
/*********调用printk打印日志***********/
int main()
{
	int fd_dev;
	int val;
	char buf[39];
	int i;
	fd_dev=open("/dev/mydev",O_RDWR);
	if(fd_dev<0)
	{
		perror("open dev fail\n");
		return -1;
	}
	while(1)
	{
		read(fd_dev,buf,sizeof(buf));
		printf("%s\n",buf);
		//sleep(1);
	}
	return 0;
}
