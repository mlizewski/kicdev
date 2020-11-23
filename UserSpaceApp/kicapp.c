/*  Keyboard Interrupt Counter (KIC) - Test App - kicapp
*   
*   Copyright (C) 2020, Marcin Liżewski
*
*   Released under the GNU General Public License, version 2
*   included herein by reference.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/rtc.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>  

#include "../DeviceDriver/ioctl_cmd_def.h"

#define DEF_DEV_FILE	"/dev/kicdev0"
#define NEW_MSG			"Info!!\n"

int main(int argc, char **argv)
{
    int fd, opt, err;
    int irq_count = 0;
	time_t reset_time;

	/* You can change dev file */
	const char *fkicdev = DEF_DEV_FILE;
	switch (argc) {
	case 2:
		fkicdev = argv[1];
	case 1:
		break;
	default:
		fprintf(stderr, "usage:  kicapp [kicdev]\n");
		return 1;
	}
	printf("KIC App test: %s \n",fkicdev);
	
	/* Connect to dev file */
	fd = open(fkicdev, O_RDONLY);
	if(fd < 0)
	{
		printf("Error in file open... \n");
		return -1;
	}

	/* Test menu */
	while(1)
	{
		printf("\n1.Get irq count\n2.Reset irq counter\n3.Get time of last reset\n4.Exit\nChoose Operation:");
		scanf("%d", &opt);
        printf(">>");
		if(opt == 4)
			break;

		if(opt < 1 || opt > 3)
		{
			printf("\n  Invalid Option... \n");
			continue;
		}

		switch(opt)
		{
		case 1:
		    /* Get irq count */
		    err = ioctl(fd, KIC_GET_IRQ_COUNT, &irq_count);
		    printf("\n  Irq count: %d \n",irq_count);
		    break;
		case 2:
		    /* Reset irq counter */
		    err = ioctl(fd, KIC_RESET_IRQ_COUNT);
		    printf("\n  Reset done\n");
		    break;
		case 3:
		    /* Get time of last reset */
		    err = ioctl(fd, KIC_GET_RESET_TIME, &reset_time);
            struct tm *ptm = localtime(&reset_time);
            if (ptm == NULL) {
                puts("The localtime() function failed");
                return -1;
            }
            printf("\n  Time of last reset: %d-%d-%d, %02d:%02d:%02d.\n",
                            ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900,
                            ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
		    break;
		}
		printf("  IOCTL retval = %d\n", err);
	}

	close(fd);
	return 0;
}
