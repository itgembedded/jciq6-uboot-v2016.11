/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the JCI Q6 board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __JCIQ6_CONFIG_H
#define __JCIQ6_CONFIG_H

#ifdef CONFIG_SPL
#include "imx6_spl.h"
#undef CONFIG_SPL_EXT_SUPPORT
#endif

#define MACH_TYPE_JCIQ6		5200	
#define CONFIG_MACH_TYPE	MACH_TYPE_JCIQ6
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONSOLE_DEV		"ttymxc0"
#define CONFIG_MMCROOT		"/dev/mmcblk1p2"
#define CONFIG_SATAROOT		"/dev/sda2"

#define VIDEO_ARGS        "${video_args}"
#define VIDEO_ARGS_SCRIPT "run video_args_script; "

/* This include must follow VIDEO_ARGS defs, otherwise re-defined */
#include "jciq6_common.h"

#define CONFIG_SYS_FSL_USDHC_NUM	1

#undef CONFIG_CMD_PCI
#ifdef CONFIG_CMD_PCI
/* #define CONFIG_PCI */
/* #define CONFIG_PCI_PNP */
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_PCIE_IMX
#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(7, 12)
#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(3, 19)
#endif

/* I2C Configs */
/* #define CONFIG_CMD_I2C -> moved to DEFCONFIG */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SPEED		  100000

/* PMIC */
#if 0
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08
#endif

/* USB Configs */
/* #define CONFIG_CMD_USB -> moved to DEFCONFIG */
#ifdef CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
/* #define CONFIG_USB_STORAGE */
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2 /* Enabled USB controller number */
#endif

/*
 * SATA Configs
 * although already included in DEFCONFIG, added define CONFIG_CMD_SATA to force sata command inclusion
 */
#define CONFIG_CMD_SATA
#ifdef CONFIG_CMD_SATA
#define CONFIG_DWC_AHSATA
#define CONFIG_SYS_SATA_MAX_DEVICE	1
#define CONFIG_DWC_AHSATA_PORT_ID	0
#define CONFIG_DWC_AHSATA_BASE_ADDR	SATA_ARB_BASE_ADDR
#define CONFIG_LBA48
#define CONFIG_LIBATA
#endif

#endif                         /* __JCIQ6_CONFIG_H */
