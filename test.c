/*
 * To conduct various tests
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
	int fd;
	char option;
	fd=open("/dev/etx_device", O_RDWR);
	if (fd<0)
	{
		printf("Some error occured\n");
		return 0;
	}
	while (1)
	{
		printf("PLease enter option \n");
		printf("Write...1 \n ");
		printf("Read...2\n");
		printf("Exit....3 \n");
		scanf("%c",&option);
		printf("YOur option is %d \n", option);
		switch(option)
		{
			case '1':
				printf("Enter data to be written to memory region");
				scanf(" %[^t\n]s", write_buf);
				printf(" \n Writing data \n");
				write(fd, write_buf, strlen(write_buf)+1);
				break;
			case '2':
				printf("Read Data \n");
				read(fd, read_buf, 1024);
				printf("Done! \n \n");	
				printf("Data=%s \n\n", read_buf);
				break;
			case 3:
				close(fd);
				exit(1);
				break;
			default:
				printf("Enter Valid Option= %c\n", option);
				break;
		}
	}
	close(fd);
}










