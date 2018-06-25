#include<sys/klog.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>

/***********应用程序****************/
#define BUFSIZE    1024
#define FILESIZE   1024000
#define SYSIZE     1024
int main()
{
	char buf[BUFSIZE];
	char sy[SYSIZE];
	int size,num,i,j,n;
	int move=0,len=0;
	int fd_log,fd_dev,fd_lseek;
	int flag=0;
	int sy_space=0;
	
	fd_log=open("./kel.log",O_RDWR|O_CREAT,0777);
	fd_lseek=open("./lseek.txt",O_RDWR|O_CREAT,0777);
	fd_dev=open("/dev/mydev",O_RDWR);
	if(fd_dev<0)
	{
		perror("open dev fail\n");
		return -1;
	}
	
	size=klogctl(10,buf,sizeof(buf));//返回内核环缓冲区大小
	printf("the size of ring buffer : %d\n",size);
	num=FILESIZE;
	while(1)
	{
			while(move<FILESIZE)
			{
				klogctl(2,buf,sizeof(buf));
			
				printf("%s\n",buf);
				
				if((FILESIZE-move)>=strlen(buf))
				{
					lseek(fd_lseek,0,SEEK_SET);
					read(fd_lseek,&move,sizeof(move));
				
					lseek(fd_log,move,SEEK_SET);
					n=write(fd_log,buf,strlen(buf));
				
					move+=n;
					
					lseek(fd_lseek,0,SEEK_SET);
					write(fd_lseek,&move,sizeof(move));
				
					memset(buf,0,sizeof(buf));
				}
				else
				{
					flag=1;
					break;
				}
			}
			
		if(flag==1)
		{
			printf("full\n");
			flag=0;
			sy_space=FILESIZE-move;

			for(i=0;i<sy_space;i++)
			{
				sy[i]=buf[i];
			}
			n=write(fd_log,sy,strlen(sy));
			move=0;

			memset(sy,0,sizeof(sy));
			for(j=0;j<strlen(buf)-sy_space;j++,i++)
			{
				sy[j]=buf[i];
			}
			lseek(fd_log,0,SEEK_SET);
			n=write(fd_log,sy,strlen(sy));

			move+=n;
			lseek(fd_lseek,0,SEEK_SET);
			write(fd_lseek,&move,sizeof(move));
		
			memset(sy,0,sizeof(sy));
			memset(buf,0,sizeof(buf));
		}
		
	}	
	return 0;
}

