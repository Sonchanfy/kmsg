#include<sys/klog.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>

/***********应用程序****************/
#define BUFSIZE    128
#define FILESIZE   10240
#define SYSIZE     128
int main()
{
	char buf[BUFSIZE];
	char sy[SYSIZE];
	int size,i,j,n;
	int move=0;
	int fd_log,fd_dev,fd_lseek;
	int flag=0;
	int sy_space=0;
	
	fd_log=open("./kel.log",O_RDWR|O_CREAT,0777);//打开或者创建log文件
	fd_lseek=open("./lseek.txt",O_RDWR|O_CREAT,0777);//打开或者创建存放文件偏移量的文件

	
	size=klogctl(10,buf,sizeof(buf));//返回内核环缓冲区大小
	printf("the size of ring buffer : %d\n",size);
	
	while(1)
	{
			while(move<FILESIZE)//文件偏移量小于文件大小时，进行写入
			{
				klogctl(2,buf,sizeof(buf));//读取内核日志的内容
			
				printf("%s\n",buf);
				
				if((FILESIZE-move)>=strlen(buf))//文件有足够空间存放内核日志buf的数据时
				{
					lseek(fd_lseek,0,SEEK_SET);
					read(fd_lseek,&move,sizeof(move));//读取文件偏移量
				
					lseek(fd_log,move,SEEK_SET);//把log文件的文件指针偏移到上次写入的位置
					n=write(fd_log,buf,strlen(buf));//写入数据
				
					move+=n;//更新本次写入的文件偏移量
					
					lseek(fd_lseek,0,SEEK_SET);
					write(fd_lseek,&move,sizeof(move));//保存本次写入的文件偏移量
				
					memset(buf,0,sizeof(buf));
				}
				else
				{
					flag=1;
					break;
				}
			}
			
		if(flag==1)//表示log文件空间不足或已满
		{
			flag=0;
			sy_space=FILESIZE-move;//算出log文件剩余的空间大小

			for(i=0;i<sy_space;i++)
			{
				sy[i]=buf[i];//把读取到的内核日志的内容复制到另外一个数组，大小为log文件剩余的大小
			}
			n=write(fd_log,sy,strlen(sy));//写入，log文件空间已满
			move=0;//文件偏移量更新为0

			memset(sy,0,sizeof(sy));
			for(j=0;j<strlen(buf)-sy_space;j++,i++)
			{
				sy[j]=buf[i];//把buf剩余的数据复制到新数组
			}
			lseek(fd_log,0,SEEK_SET);//把文件指针指向log文件的文件头
			n=write(fd_log,sy,strlen(sy));//把buf剩余的数据从log文件头覆盖写入

			move+=n;//更新文件偏移量
			lseek(fd_lseek,0,SEEK_SET);//覆盖上次的偏移量
			write(fd_lseek,&move,sizeof(move));//写入保存本次写入结束的位置
		
			memset(sy,0,sizeof(sy));
			memset(buf,0,sizeof(buf));
		}
		
	}	
	return 0;
}

