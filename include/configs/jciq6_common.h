/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the JCI Q6 board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __JCIQ6_COMMON_CONFIG_H
#define __JCIQ6_COMMON_CONFIG_H

#include "mx6_common.h"

#define CONFIG_IMX_THERMAL

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(10 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_MXC_UART

/* MMC Configs */
#define CONFIG_SYS_FSL_ESDHC_ADDR      0
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_FAT_WRITE
#define CONFIG_DOS_PARTITION

/* moved to DEFCONFIG
#define CONFIG_CMD_FAT
#define CONFIG_CMD_MMC
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
*/
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RGMII
#define CONFIG_ETHPRIME			"FEC"
#define CONFIG_FEC_MXC_PHYADDR		3

#define CONFIG_PHYLIB
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9031

/* #define CONFIG_CMD_SF -> DEFCONFIG */
#ifdef CONFIG_CMD_SF
#define CONFIG_SPI_FLASH_SST
#define CONFIG_SPI_FLASH_SST26
#define CONFIG_MXC_SPI
#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED		20000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#endif

/* Command definition */
#define CONFIG_CMD_BMODE

#ifdef CONFIG_SUPPORT_EMMC_BOOT
#define EMMC_ENV \
	"emmcdev=2\0" \
	"update_emmc_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if ${get_cmd} ${update_sd_firmware_filename}; then " \
			"if mmc dev ${emmcdev} 1; then "	\
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0"
#else
#define EMMC_ENV ""
#endif

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#ifndef VIDEO_ARGS
#define VIDEO_ARGS ""
#endif

#ifndef VIDEO_ARGS_SCRIPT
#define VIDEO_ARGS_SCRIPT ""
#endif


/* Env settings */
/* If adding new CONFIG options/vars, add them to /git/scripts/cnofig_whitelist.txt for validity tests */
#define CONFIG_ENV_DEFAULT_UBT_FILE		"u-boot.imx"
#define CONFIG_ENV_DEFAULT_IMG_FILE		"zImage"
#define CONFIG_ENV_DEFAULT_FDT_FILE		"imx6q-jciq6.dtb"
#define CONFIG_ENV_DEFAULT_FDT_FILE_640		"imx6q-jciq6-640.dtb"
#define CONFIG_ENV_DEFAULT_FDT_FILE_QP		"imx6qp-jciq6.dtb"
#define CONFIG_ENV_DEFAULT_FDT_FILE_QP_640	"imx6qp-jciq6-640.dtb"
#define CONFIG_ENV_DEFAULT_SCR_FILE		"boot-jciq6.scr"

#define CONFIG_ENV_DEFAULT_ETH_ADDR         "00:0D:15:00:D1:75"
#define CONFIG_ENV_DEFAULT_CLIENT_IP        "192.168.0.150"
#define CONFIG_ENV_DEFAULT_SERVER_IP        "192.168.0.1"
#define CONFIG_ENV_DEFAULT_NETMASK          "255.255.255.0"
#define CONFIG_ENV_DEFAULT_NFSROOT          "/nfs/imx6"
#define CONFIG_ENV_DEFAULT_TFTP_DIR         "imx6"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"tftp_dir=" CONFIG_ENV_DEFAULT_TFTP_DIR "\0" \
	"script="   CONFIG_ENV_DEFAULT_SCR_FILE "\0" \
	"image="    CONFIG_ENV_DEFAULT_IMG_FILE "\0" \
	"fdt_file=\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"nfsroot=" CONFIG_ENV_DEFAULT_NFSROOT "\0" \
	"console=" CONSOLE_DEV "\0" \
	"dfuspi=dfu 0 sf 0:0:10000000:0\0" \
	"dfu_alt_info_spl=spl raw 0x400\0" \
	"dfu_alt_info_img=u-boot raw 0x10000\0" \
	"dfu_alt_info=spl raw 0x400\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"mmcdev=0\0" \
	"mmcpart=1\0" \
	"satadev=0\0" \
	"satapart=1\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"sataroot=" CONFIG_SATAROOT " rootwait rw\0" \
	"spidev=" __stringify(CONFIG_ENV_SPI_BUS) "\0" \
	"spics=" __stringify(CONFIG_ENV_SPI_CS) "\0" \
	"set_ethernet=" \
		"if test ${ethaddr}; then; else " \
			"setenv ethaddr  " CONFIG_ENV_DEFAULT_ETH_ADDR  "; " \
		"fi; " \
		"if test ${ipaddr}; then; else " \
			"setenv ipaddr   " CONFIG_ENV_DEFAULT_CLIENT_IP "; " \
		"fi; " \
		"if test -n ${serverip}; then; else " \
			"setenv serverip " CONFIG_ENV_DEFAULT_SERVER_IP "; " \
		"fi; " \
		"if test ${netmask}; then; else " \
			"setenv netmask  " CONFIG_ENV_DEFAULT_NETMASK   "; " \
		"fi\0" \
	"update_set_filename=" \
		"if test ${upd_uboot}; then; else " \
			"setenv upd_uboot " CONFIG_ENV_DEFAULT_UBT_FILE  "; " \
		"fi; " \
		"if test ${upd_kernel}; then; else " \
			"setenv upd_kernel " CONFIG_ENV_DEFAULT_IMG_FILE "; " \
		"fi; " \
		"if test ${upd_fdt}; then; else " \
			"setenv upd_fdt    " CONFIG_ENV_DEFAULT_FDT_FILE "; " \
		"fi; " \
		"if test ${upd_script}; then; else " \
			"setenv upd_script " CONFIG_ENV_DEFAULT_SCR_FILE "; " \
		"fi\0" \
	"update_uboot=" \
		"run set_ethernet; " \
		"run update_set_filename; " \
		"if mmc dev ${mmcdev}; then "	\
			"if tftp ${tftp_dir}/${upd_uboot}; then " \
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; " \
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0" \
	"update_kernel=" \
		"run set_ethernet; " \
		"run update_set_filename; " \
		"if mmc dev ${mmcdev}; then "	\
			"if tftp ${tftp_dir}/${upd_kernel}; then " \
				"fatwrite mmc ${mmcdev}:${mmcpart} " \
				"${loadaddr} ${image} ${filesize}; " \
			"fi; "	\
		"fi\0" \
	"update_fdt=" \
		"run set_ethernet; " \
		"run update_set_filename; " \
		"if mmc dev ${mmcdev}; then "	\
			"if tftp ${tftp_dir}/${upd_fdt}; then " \
				"fatwrite mmc ${mmcdev}:${mmcpart} " \
				"${loadaddr} ${fdt_file} ${filesize}; " \
			"fi; "	\
		"fi\0" \
	"update_spi_uboot=" \
		"run set_ethernet; " \
		"run update_set_filename; " \
		"mw.b ${loadaddr} 0xFF 0x80000; " \
		"tftp ${tftp_dir}/${upd_uboot}; " \
		"sf probe;sf erase 0x0 0x80000; " \
		"sf write ${loadaddr} 0x400 0x80000 " \
		"\0" \
	"update_script=" \
		"run set_ethernet; " \
		"run update_set_filename; " \
		"if mmc dev ${mmcdev}; then "	\
			"if tftp ${tftp_dir}/${upd_script}; then " \
				"fatwrite mmc ${mmcdev}:${mmcpart} " \
				"${loadaddr} ${script} ${filesize}; " \
			"fi; "	\
		"fi\0" \
	"update_sd_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if mmc dev ${mmcdev}; then "	\
			"if ${get_cmd} ${update_sd_firmware_filename}; then " \
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0" \
	EMMC_ENV	  \
	"video_args_hdmi=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=hdmi,1280x720M@60,if=RGB24\0" \
	"video_args_lvds=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=ldb,LDB-XGA,if=RGB888\0" \
	"fb=0\0" \
	"video_interfaces=\0" \
	"video_args_script=" \
		"for v in ${video_interfaces}; do " \
			"run video_args_${v}; " \
			"setexpr fb $fb + 1; " \
		"done\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=${mmcroot} " \
		VIDEO_ARGS "\0" \
	"sataargs=setenv bootargs console=${console},${baudrate} " \
		"root=${sataroot} " \
		VIDEO_ARGS "\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"sataloadimage=fatload sata ${satadev}:${satapart} ${loadaddr} ${image}\0" \
	"sataloadfdt=fatload sata ${satadev}:${satapart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=" \
		"run check_j20; " \
		VIDEO_ARGS_SCRIPT \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"bootmmc=echo Booting from MMC ...; " \
		"mmc dev ${mmcdev};" \
		"if mmc rescan; then " \
			"if run loadbootscript; then " \
				"run bootscript; " \
			"else " \
				"if run loadimage; then " \
					"run mmcboot; " \
				"else run bootsata; " \
				"fi; " \
			"fi; " \
		"else run bootsata; fi;\0" \
	"bootsata=echo Booting from SATA ...; " \
		"run check_j20; " \
		VIDEO_ARGS_SCRIPT \
		"run sataargs; " \
		"if sata init; then " \
			"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
				"if run sataloadimage; then " \
					"if run sataloadfdt; then " \
						"bootz ${loadaddr} - ${fdt_addr}; " \
					"else " \
						"if test ${boot_fdt} = try; then " \
							"bootz; " \
						"else " \
							"echo WARN: Cannot load the DT; " \
						"fi; " \
					"fi; " \
				"else echo WARN: Cannot load the image; " \
				"fi; " \
			"else " \
				"bootz; " \
			"fi; " \
		"else run netboot; fi;\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp rootwait rw " \
		VIDEO_ARGS "\0" \
	"netboot=echo Booting from net ...; " \
		"run check_j20; " \
		VIDEO_ARGS_SCRIPT \
		"run set_ethernet; " \
		"run update_set_filename; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${loadaddr} ${tftp_dir}/${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${tftp_dir}/${upd_fdt}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"bootnet=run netboot;\0" \
	"check_j20=" \
		"if test -z \"$video_interfaces\"; then " \
			"setenv video_interfaces lvds; " \
			"if j20 hdmi; then " \
				"setenv video_interfaces hdmi; " \
			"fi; " \
		"fi; " \
		"if test -z \"$fdt_file\"; then " \
			"if j20 640; then " \
				"if qplus; then " \
					"setenv fdt_file " CONFIG_ENV_DEFAULT_FDT_FILE_QP_640 "; " \
				"else " \
					"setenv fdt_file " CONFIG_ENV_DEFAULT_FDT_FILE_640 "; " \
				"fi; " \
			"else " \
				"if qplus; then " \
					"setenv fdt_file " CONFIG_ENV_DEFAULT_FDT_FILE_QP "; " \
				"else " \
					"setenv fdt_file " CONFIG_ENV_DEFAULT_FDT_FILE "; " \
				"fi; " \
			"fi; " \
		"fi;\0 " \
		

#define CONFIG_BOOTCOMMAND \
	"run bootmmc;"

#define CONFIG_ARP_TIMEOUT     200UL

#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

#define CONFIG_STACKSIZE               (128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS           1
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* Environment organization */
#define CONFIG_ENV_SIZE			(8 * 1024)

#define CONFIG_ENV_IS_IN_MMC

#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(12 * 64 * 1024)
#define CONFIG_SYS_MMC_ENV_DEV		0
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET		(768 * 1024)
#define CONFIG_ENV_SECT_SIZE		(8 * 1024)
#define CONFIG_ENV_SPI_BUS		CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS		CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE		CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
#endif

/* Framebuffer - arch/arm/imx-common/video.c needs displays, display_count */
#if 0
/* #define CONFIG_VIDEO */
#define CONFIG_VIDEO_IPUV3
/* moved to defconfig in Morty
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
*/
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IPUV3_CLK 264 000000
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP
#endif

#ifndef CONFIG_SPL
#define CONFIG_USBD_HS
#define CONFIG_USB_FUNCTION_MASS_STORAGE

/* moved to DEFCONFIG
#define CONFIG_CI_UDC
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_DOWNLOAD
#define CONFIG_USB_GADGET_VBUS_DRAW	2

#define CONFIG_G_DNL_VENDOR_NUM		0x0525
#define CONFIG_G_DNL_PRODUCT_NUM	0xa4a5
#define CONFIG_G_DNL_MANUFACTURER	"FSL"

#define CONFIG_CMD_DFU
*/

#define CONFIG_USB_FUNCTION_FASTBOOT
#define CONFIG_CMD_FASTBOOT
#define CONFIG_ANDROID_BOOT_IMAGE
#define CONFIG_FASTBOOT_BUF_ADDR   CONFIG_SYS_LOAD_ADDR
#define CONFIG_FASTBOOT_BUF_SIZE   0x07000000

/* USB Device Firmware Update support */
/* moved to defconfig
#define CONFIG_DFU_SF
#define CONFIG_DFU_MMC
*/
/* #define CONFIG_USB_FUNCTION_DFU */
#endif

#endif                         /* __JCIQ6_COMMON_CONFIG_H */
