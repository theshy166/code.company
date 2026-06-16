#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void dump_data_print(const char *string, unsigned char *data, unsigned int length, unsigned char align)
{
	unsigned int i;

	/* Print the string */
	printf("%s\n",string);

	if (NULL != data)
	{
		for(i=0; i<length; i++)
		{
			printf("%02x ", data[i]);
			if(0 == ((i+1) & (align-1)))
			{
				printf("\n");
			}
		}
		printf("\n");
	}
}


int main(int argc, char const *argv[])
{
	int fd = -1;
	int retval = 0;
	int length = 0;
	unsigned char buff[1024];

	fd = open("/dev/hw_random", O_RDONLY);
	if(fd == -1)
	{
		printf("ERROR: open hw_random failed!\n");
		return -1;
	}

	if( argc > 1 )
	{
		length = atoi( argv[1] );
		printf("manual test mode, the random byte length is specified as %d \n", length);

		retval = read(fd, buff, length);
		if(retval == -1)
		{
			printf("ERROR: read trng failed!\n");
			return -2;
		}
		dump_data_print("random out:",buff, retval, 16);
	}
	else
	{
		printf("random number auto test begin:\n");
		while(length<1024)
		{
			length += 16;
			retval = read(fd, buff, length);
			if(retval == -1)
			{
				printf("ERROR: read trng failed!\n");
				return -2;
			}
			dump_data_print("random out:",buff, retval, 16);
		}
	}
	printf("random number test finished!\n");

	close(fd);
	return 0;
}
