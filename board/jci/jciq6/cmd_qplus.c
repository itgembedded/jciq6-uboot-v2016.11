/*
 * Copyright 2017 JCI Frick
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/arch/sys_proto.h>

// To be used with U-Boot "if" command, fxn should return 0 on "success", other on "failure"

// Determine if current processor is PLUS variety
static int do_qplus(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret;

	if(is_mx6dqp())
		ret = 0;	// Yes, it's an i.MX6 PLUS
	else
		ret = 1;	// No, just normal i.MX6

	return ret;
}

U_BOOT_CMD(
	qplus,	1,	1,	do_qplus,
	"Checks if CPU is i.MX6 Plus type",
	"Usage: qplus\n"
	"       Checks if CPU is i.MX6 Plus type\n"
	"       Returns 0 (Success) if Plus, else 1 (Failure)\n"
);

