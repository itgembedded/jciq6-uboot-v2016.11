/*
 * Copyright 2017 JCI Frick
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/gpio.h>

// To be used with U-Boot "if" command, fxn should return 0 on "success", other on "failure"
// changing to support str params ("1024", "640", "HDMI", "hdmi")
static int do_j20(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int val640, val1024, ret;
	
	if(argc < 2) {
		printf("j20 needs an argument (1024, 640, HDMI, hdmi)\n");
		return 0;	
	}

	// read both J20 jumper positions (note: 0 = installed, 1 = open)
	val1024 = gpio_get_value(IMX_GPIO_NR(5, 21));	// check J20_1024 position 
	val640 = gpio_get_value(IMX_GPIO_NR(5, 22));	// check J20_640 position 
	
	// check which setting was selected
	if(strcmp(argv[1], "1024") == 0) {
		if(val1024)
			ret = 1;	// J20_1024 jumper NOT installed
		else
			ret = 0;	// J20_1024 jumper installed (i.e. Success)		
	}
	else if(strcmp(argv[1], "640") == 0) {
		if(val640)
			ret = 1;	// J20_640 jumper NOT installed
		else
			ret = 0;	// J20_640 jumper installed (i.e. Success)			
	}
	else if(strcasecmp(argv[1], "hdmi") == 0) {
		if(val1024 && val640)
			ret = 0;	// J20 jumper nothing installed = HDMI (i.e. Success)
		else
			ret = 1;	// J20 jumper an LCD selected	
	}
	else {
		printf("%s is an invalid J20 parameter (use 1024, 640, hdmi, HDMI)\n", argv[1]);
		ret = 1;	// failure		
	}

	return ret;
}

U_BOOT_CMD(
	j20,	2,	1,	do_j20,
	"Checks position of J20 Jumper",
	"Usage: j20 [1024 | 640 | hdmi | HDMI]\n"
	"       Checks specified Q6 PCB jumper location\n"
	"       Returns 0 (Success) if installed, else 1 (Failure)\n"
);


