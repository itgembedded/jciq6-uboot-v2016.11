/*
 * (C) Copyright 2009-2013 ADVANSEE
 * Benoît Thébaudeau <benoit.thebaudeau@advansee.com>
 *
 * Based on the mpc512x iim code:
 * Copyright 2008 Silicon Turnkey Express, Inc.
 * Martha Marx <mmarx@silicontkx.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <console.h>
#include <fuse.h>
#include <linux/errno.h>

#define MAC_BASE_HI 0x00000010
#define MAC_BASE_LO 0x8D092000
#define MAC_OFF_LIMIT 0x12000 
#define BOOT_CFG1 0x08000030	
#define BOOT_CFG2 0x00000010 

static int strtou32(const char *str, unsigned int base, u32 *result)
{
	char *ep;

	*result = simple_strtoul(str, &ep, base);
	if (ep == str || *ep != '\0')
		return -EINVAL;

	return 0;
}

static int confirm_prog(void)
{
	puts("Warning: Programming fuses is an irreversible operation!\n"
			"         This may brick your system.\n"
			"         Use this command only if you are sure of "
					"what you are doing!\n"
			"\nReally perform this fuse programming? <y/N>\n");

	if (confirm_yesno())
		return 1;

	puts("Fuse programming aborted\n");
	return 0;
}

static int do_fuse(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	const char *op = argc >= 2 ? argv[1] : NULL;
	int confirmed = argc >= 3 && !strcmp(argv[2], "-y");
	u32 bank, word, cnt, val, offset, mac, val2, val3, val4;
	int ret, i, check_error;

	argc -= 2 + confirmed;
	argv += 2 + confirmed;

	if(strcmp(op, "q6")) {	/* no arg checking for new "q6" op */
		if (argc < 2 || strtou32(argv[0], 0, &bank) || strtou32(argv[1], 0, &word))
			return CMD_RET_USAGE;
	}

	if (!strcmp(op, "read")) {
		if (argc == 2)
			cnt = 1;
		else if (argc != 3 || strtou32(argv[2], 0, &cnt))
			return CMD_RET_USAGE;

		printf("Reading bank %u:\n", bank);
		for (i = 0; i < cnt; i++, word++) {
			if (!(i % 4))
				printf("\nWord 0x%.8x:", word);

			ret = fuse_read(bank, word, &val);
			if (ret)
				goto err;

			printf(" %.8x", val);
		}
		putc('\n');
	} else if (!strcmp(op, "sense")) {
		if (argc == 2)
			cnt = 1;
		else if (argc != 3 || strtou32(argv[2], 0, &cnt))
			return CMD_RET_USAGE;

		printf("Sensing bank %u:\n", bank);
		for (i = 0; i < cnt; i++, word++) {
			if (!(i % 4))
				printf("\nWord 0x%.8x:", word);

			ret = fuse_sense(bank, word, &val);
			if (ret)
				goto err;

			printf(" %.8x", val);
		}
		putc('\n');
	} else if (!strcmp(op, "prog")) {
		if (argc < 3)
			return CMD_RET_USAGE;

		for (i = 2; i < argc; i++, word++) {
			if (strtou32(argv[i], 16, &val))
				return CMD_RET_USAGE;

			printf("Programming bank %u word 0x%.8x to 0x%.8x...\n",
					bank, word, val);
			if (!confirmed && !confirm_prog())
				return CMD_RET_FAILURE;
			ret = fuse_prog(bank, word, val);
			if (ret)
				goto err;
		}
	} else if (!strcmp(op, "override")) {
		if (argc < 3)
			return CMD_RET_USAGE;

		for (i = 2; i < argc; i++, word++) {
			if (strtou32(argv[i], 16, &val))
				return CMD_RET_USAGE;

			printf("Overriding bank %u word 0x%.8x with "
					"0x%.8x...\n", bank, word, val);
			ret = fuse_override(bank, word, val);
			if (ret)
				goto err;
		}
	} else if (!strcmp(op, "q6")) {
		/* determine if valid MAC offset provided */
		offset = 0;
		if(argc) {
			if(strtou32(argv[0], 0, &offset))
				offset = 0;
			else if(offset >= MAC_OFF_LIMIT)
				offset = 0;
			if(offset == 0) {
				printf("Invalid MAC offset provided: 0 < %d < %d (0x%.8x)\n", 
					offset, MAC_OFF_LIMIT, MAC_OFF_LIMIT);
				goto err;
			}
		}

		if(offset) { /* Program fuses */
			/* First confirm that affected fuses are still blank */
			ret = fuse_sense(0, 5, &val);	/* BOOT_CFG */
			ret += fuse_sense(0, 6, &val2);	/* BT_FUSE_SEL */
			ret += fuse_sense(4, 2, &val3);	/* MAC_ADDR_LO */
			ret += fuse_sense(4, 3, &val4);	/* MAC_ADDR_HI */
			if(ret) {
				puts("ERROR sensing fuses.  Aborted!\n");
				goto err;
			}

			/* Abort if some fuses are already programmed */
			if((val !=0 ) || (val2 !=0 ) || (val3 !=0 ) || (val4 !=0 )) {
				puts("ERROR.  At least one of the fuses is already blown.  Aborted!\n");
				printf("Bank 0-5: 0x%08x\n", val);
				printf("Bank 0-6: 0x%08x\n", val2);
				printf("Bank 4-3: 0x%08x\n", val3);
				printf("Bank 4-2: 0x%08x\n", val4);
				goto err;
			}
		
			printf("Setting PCB S/N to %d\n", offset);
			puts("Confirm you want to program i.MX6 fuses now: <y/N>");
			if (confirm_yesno()) {
				mac = MAC_BASE_LO + offset;
				puts("Programming fuses now...");
				if(fuse_prog(0, 5, BOOT_CFG1))
					puts("\nERROR programming 0-5 BOOT_CFG1. Aborted!\n");
				else {
					if(fuse_prog(0, 6, BOOT_CFG2))
						puts("\nERROR programming 0-6 BOOT_CFG2. Aborted!\n");
					else {
						if(fuse_prog(4, 3, MAC_BASE_HI))
							puts("\nERROR programming 4-3 MAC_ADDR_HI. Aborted!\n");
						else {
							if(fuse_prog(4, 2, mac))
								puts("\nERROR programming 4-2 MAC_ADDR_LO. Aborted!\n");
							else {
								puts("Completed\nChecking fuses...");
								check_error = 0;
								if(fuse_sense(0, 5, &val)) {
									puts("\nERROR sensing 0-5 BOOT_CFG1\n");
									check_error++;
								}
								else if(val != BOOT_CFG1) {
									printf("\nERROR validating 0-5 BOOT_CFG1 (0x%08x)\n", val);
									check_error++;
								}
								if(fuse_sense(0, 6, &val2)) {
									puts("\nERROR sensing 0-6 BOOT_CFG2\n");
									check_error++;
								}
								else if(val2 != BOOT_CFG2) {
									printf("\nERROR validating 0-6 BOOT_CFG2 (0x%08x)\n", val2);
									check_error++;
								}
								if(fuse_sense(4, 3, &val3)) {
									puts("\nERROR sensing 4-3 MAC_ADDR_HI\n");
									check_error++;
								}
								else if(val3 != MAC_BASE_HI) {
									printf("\nERROR validating 4-3 MAC_ADDR_HI (0x%08x)\n", val3);
									check_error++;
								}
								if(fuse_sense(4, 2, &val4)) {
									puts("\nERROR sensing 4-2 MAC_ADDR_LO\n");
									check_error++;
								}
								else if(val4 != mac) {
									printf("\nERROR validating 4-2 MAC_ADDR_LO (0x%08x)\n", val4);
									check_error++;
								}
								if(check_error) 
									puts("WARNING: Validation Failed!\n");
								else {
									puts("Completed Successfully\n");
									printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", 
									(val3 & 0xff00) >> 8, (val3 & 0xff),				
									(val4 & 0xff000000) >> 24, (val4 & 0xff0000) >> 16, 
									(val4 & 0xff00) >> 8, (val4 & 0xff));
								}				
								printf("Bank 0-5: 0x%08x\n", val);
								printf("Bank 0-6: 0x%08x\n", val2);
								printf("Bank 4-3: 0x%08x\n", val3);
								printf("Bank 4-2: 0x%08x\n", val4);
							}
						}
					}
				}
			}
			else
				puts("Operation Aborted!\n");
		}
		else {	  /* Report Q6 fuse settings */
			ret = fuse_sense(0, 5, &val);	/* BOOT_CFG */
			ret += fuse_sense(0, 6, &val2);	/* BT_FUSE_SEL */
			if(ret) 
				printf("Error reading BOOT_CFG fuses\n");
			else {
				if((val == BOOT_CFG1) && (val2 & BOOT_CFG2))
					printf("Correct Q6 BOOT_CFG (from SPI)\n");
				else if(!(val2 & BOOT_CFG2))
					printf("WARNING: BOOT_CFG not selected from eFuses\n");
				else 
					printf("WARNING: BOOT_CFG not set to SPI\n");
				printf("Bank 0-5: 0x%08x\n", val);
				printf("Bank 0-6: 0x%08x\n", val2);
			}

			ret = fuse_sense(4, 2, &val);	/* MAC_ADDR_LO */
			ret += fuse_sense(4, 3, &val2);	/* MAC_ADDR_HI */
			if(ret) 
				printf("Error reading MAC_ADDR fuses\n");
			else {
				if(val == 0) 
					printf("WARNING: MAC_ADDR Fuses not set yet\n");
				else if((val2 != MAC_BASE_HI) || (val > (MAC_BASE_LO + MAC_OFF_LIMIT)) || (val < MAC_BASE_LO)) {
					printf("WARNING: MAC_ADDR outside valid JCI range\n");
					printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", 
					(val2 & 0xff00) >> 8, (val2 & 0xff),				
					(val & 0xff000000) >> 24, (val & 0xff0000) >> 16, 
					(val & 0xff00) >> 8, (val & 0xff));		
				}		
				else {
					printf("PCB S/N: %d\n", val - MAC_BASE_LO);
					printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", 
					(val2 & 0xff00) >> 8, (val2 & 0xff),				
					(val & 0xff000000) >> 24, (val & 0xff0000) >> 16, 
					(val & 0xff00) >> 8, (val & 0xff));				
				}
				printf("Bank 4-2: 0x%08x\n", val);
				printf("Bank 4-3: 0x%08x\n", val2);
			}
		}
	
	} else {
		return CMD_RET_USAGE;
	}

	return 0;

err:
	puts("ERROR\n");
	return CMD_RET_FAILURE;
}

U_BOOT_CMD(
	fuse, CONFIG_SYS_MAXARGS, 0, do_fuse,
	"Fuse sub-system",
	     "read <bank> <word> [<cnt>] \n"
	"     read 1 or 'cnt' fuse words, starting at 'word'\n"
	"fuse sense <bank> <word> [<cnt>] \n"
	"     sense 1 or 'cnt' fuse words, starting at 'word'\n"
	"fuse prog [-y] <bank> <word> <hexval> [<hexval>...] \n"
	"     program 1 or several fuse words, starting at 'word' (PERMANENT)\n"
	"fuse override <bank> <word> <hexval> [<hexval>...] \n"
	"     override 1 or several fuse words, starting at 'word'\n"
	"fuse q6 [<MAC ADDR offset>] \n"
	"     factory program Q6 board if optional offset provided\n"
	"     otherwise reports settings: boot from SPI, MAC ADDR"
/*
	"Fuse sub-system",
	     "read <bank> <word> [<cnt>] - read 1 or 'cnt' fuse words,\n"
	"    starting at 'word'\n"
	"fuse sense <bank> <word> [<cnt>] - sense 1 or 'cnt' fuse words,\n"
	"    starting at 'word'\n"
	"fuse prog [-y] <bank> <word> <hexval> [<hexval>...] - program 1 or\n"
	"    several fuse words, starting at 'word' (PERMANENT)\n"
	"fuse override <bank> <word> <hexval> [<hexval>...] - override 1 or\n"
	"    several fuse words, starting at 'word'"
*/
);
