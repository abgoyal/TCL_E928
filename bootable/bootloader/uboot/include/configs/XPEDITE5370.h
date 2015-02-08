

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_BOOKE		1	/* BOOKE */
#define CONFIG_E500		1	/* BOOKE e500 family */
#define CONFIG_MPC85xx		1	/* MPC8540/60/55/41/48 */
#define CONFIG_MPC8572		1
#define CONFIG_XPEDITE5370	1
#define CONFIG_SYS_BOARD_NAME	"XPedite5370"
#define CONFIG_BOARD_EARLY_INIT_R	/* Call board_pre_init */

#define CONFIG_PCI		1	/* Enable PCI/PCIE */
#define CONFIG_PCI_PNP		1	/* do pci plug-and-play */
#define CONFIG_PCI_SCAN_SHOW	1	/* show pci devices on startup */
#define CONFIG_PCIE1		1	/* PCIE controler 1 */
#define CONFIG_PCIE2		1	/* PCIE controler 2 */
#define CONFIG_FSL_PCI_INIT	1	/* Use common FSL init code */
#define CONFIG_SYS_PCI_64BIT	1	/* enable 64-bit PCI resources */
#define CONFIG_FSL_PCIE_RESET	1	/* need PCIe reset errata */
#define CONFIG_FSL_LAW		1	/* Use common FSL init code */

#define CONFIG_MP
#define CONFIG_BPTR_VIRT_ADDR	0xee000000	/* virt boot page address */
#define CONFIG_MPC8xxx_DISABLE_BPTR		/* Don't leave BPTR enabled */

#define CONFIG_FSL_DDR2
#undef CONFIG_FSL_DDR_INTERACTIVE
#define CONFIG_SPD_EEPROM		/* Use SPD EEPROM for DDR setup */
#define CONFIG_DDR_SPD
#define CONFIG_MEM_INIT_VALUE		0xdeadbeef
#define SPD_EEPROM_ADDRESS1		0x54	/* Both channels use the */
#define SPD_EEPROM_ADDRESS2		0x54	/* same SPD data         */
#define SPD_EEPROM_OFFSET		0x200	/* OFFSET of SPD in EEPROM */
#define CONFIG_NUM_DDR_CONTROLLERS	2
#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define CONFIG_DDR_ECC
#define CONFIG_ECC_INIT_VIA_DDRCONTROLLER
#define CONFIG_SYS_DDR_SDRAM_BASE	0x00000000 /* DDR is system memory*/
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE
#define CONFIG_VERY_BIG_RAM

#ifndef __ASSEMBLY__
extern unsigned long get_board_sys_clk(unsigned long dummy);
extern unsigned long get_board_ddr_clk(unsigned long dummy);
#endif

#define CONFIG_SYS_CLK_FREQ	get_board_sys_clk(0) /* sysclk for MPC85xx */
#define CONFIG_DDR_CLK_FREQ	get_board_ddr_clk(0) /* ddrclk for MPC85xx */

#define CONFIG_L2_CACHE			/* toggle L2 cache */
#define CONFIG_BTB			/* toggle branch predition */
#define CONFIG_ENABLE_36BIT_PHYS	1

#define CONFIG_SYS_CCSRBAR_DEFAULT	0xff700000	/* CCSRBAR Default */
#define CONFIG_SYS_CCSRBAR		0xef000000	/* relocated CCSRBAR */
#define CONFIG_SYS_CCSRBAR_PHYS	CONFIG_SYS_CCSRBAR	/* physical addr of CCSRBAR */
#define CONFIG_SYS_IMMR		CONFIG_SYS_CCSRBAR	/* PQII uses CONFIG_SYS_IMMR */
#define CONFIG_SYS_PCIE1_ADDR		(CONFIG_SYS_CCSRBAR + 0xa000)
#define CONFIG_SYS_PCIE2_ADDR		(CONFIG_SYS_CCSRBAR + 0x9000)

#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x10000000
#define CONFIG_SYS_MEMTEST_END		0x20000000


#define CONFIG_SYS_LBC_LCRR	(LCRR_CLKDIV_8 | LCRR_EADC_3)

#define CONFIG_SYS_NAND_BASE		0xef800000
#define CONFIG_SYS_NAND_BASE2		0xef840000 /* Unused at this time */
#define CONFIG_SYS_NAND_BASE_LIST	{CONFIG_SYS_NAND_BASE, \
					 CONFIG_SYS_NAND_BASE2}
#define CONFIG_SYS_MAX_NAND_DEVICE	2
#define CONFIG_MTD_NAND_VERIFY_WRITE
#define CONFIG_SYS_NAND_QUIET_TEST	/* 2nd NAND flash not always populated */
#define CONFIG_NAND_FSL_ELBC

#define CONFIG_SYS_FLASH_BASE		0xf8000000
#define CONFIG_SYS_FLASH_BASE2		0xf0000000
#define CONFIG_SYS_FLASH_BANKS_LIST	{CONFIG_SYS_FLASH_BASE, CONFIG_SYS_FLASH_BASE2}
#define CONFIG_SYS_MAX_FLASH_BANKS	2		/* number of banks */
#define CONFIG_SYS_MAX_FLASH_SECT	1024		/* sectors per device */
#define CONFIG_SYS_FLASH_ERASE_TOUT	60000		/* Flash Erase Timeout (ms) */
#define CONFIG_SYS_FLASH_WRITE_TOUT	500		/* Flash Write Timeout (ms) */
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE
#define CONFIG_SYS_FLASH_AUTOPROTECT_LIST	{ {0xfff40000, 0xc0000}, \
						  {0xf7f40000, 0xc0000} }
#define CONFIG_SYS_MONITOR_BASE	TEXT_BASE	/* start of monitor */

/* NOR Flash 0 on CS0 */
#define CONFIG_SYS_BR0_PRELIM	(CONFIG_SYS_FLASH_BASE	| \
				 BR_PS_16		| \
				 BR_V)
#define CONFIG_SYS_OR0_PRELIM	(OR_AM_128MB		| \
				 OR_GPCM_CSNT		| \
				 OR_GPCM_XACS		| \
				 OR_GPCM_ACS_DIV2	| \
				 OR_GPCM_SCY_8		| \
				 OR_GPCM_TRLX		| \
				 OR_GPCM_EHTR		| \
				 OR_GPCM_EAD)

/* NOR Flash 1 on CS1 */
#define CONFIG_SYS_BR1_PRELIM	(CONFIG_SYS_FLASH_BASE2	| \
				 BR_PS_16		| \
				 BR_V)
#define CONFIG_SYS_OR1_PRELIM	CONFIG_SYS_OR0_PRELIM

/* NAND flash on CS2 */
#define CONFIG_SYS_BR2_PRELIM	(CONFIG_SYS_NAND_BASE	| \
				 (2<<BR_DECC_SHIFT)	| \
				 BR_PS_8		| \
				 BR_MS_FCM		| \
				 BR_V)

/* NAND flash on CS2 */
#define CONFIG_SYS_OR2_PRELIM	(OR_AM_256KB	| \
				 OR_FCM_PGS	| \
				 OR_FCM_CSCT	| \
				 OR_FCM_CST	| \
				 OR_FCM_CHT	| \
				 OR_FCM_SCY_1	| \
				 OR_FCM_TRLX	| \
				 OR_FCM_EHTR)

/* NAND flash on CS3 */
#define CONFIG_SYS_BR3_PRELIM	(CONFIG_SYS_NAND_BASE2	| \
				 (2<<BR_DECC_SHIFT)	| \
				 BR_PS_8		| \
				 BR_MS_FCM		| \
				 BR_V)
#define CONFIG_SYS_OR3_PRELIM	CONFIG_SYS_OR2_PRELIM

#define CONFIG_SYS_INIT_RAM_LOCK	1
#define CONFIG_SYS_INIT_RAM_ADDR	0xe0000000
#define CONFIG_SYS_INIT_RAM_END		0x00004000

#define CONFIG_SYS_GBL_DATA_SIZE	128	/* num bytes initial data */
#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_INIT_RAM_END - CONFIG_SYS_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_OFFSET	CONFIG_SYS_GBL_DATA_OFFSET

#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)	/* Reserve 512 KB for Mon */
#define CONFIG_SYS_MALLOC_LEN		(1024 * 1024)	/* Reserved for malloc */

#define CONFIG_CONS_INDEX		1
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK		get_bus_freq(0)
#define CONFIG_SYS_NS16550_COM1	(CONFIG_SYS_CCSRBAR+0x4500)
#define CONFIG_SYS_NS16550_COM2	(CONFIG_SYS_CCSRBAR+0x4600)
#define CONFIG_SYS_BAUDRATE_TABLE	\
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 115200}
#define CONFIG_BAUDRATE			115200
#define CONFIG_LOADS_ECHO		1	/* echo on for serial download */
#define CONFIG_SYS_LOADS_BAUD_CHANGE	1	/* allow baudrate change */

#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "

#define CONFIG_OF_LIBFDT		1
#define CONFIG_OF_BOARD_SETUP		1
#define CONFIG_OF_STDOUT_VIA_ALIAS	1

#define CONFIG_FSL_I2C				/* Use FSL common I2C driver */
#define CONFIG_HARD_I2C				/* I2C with hardware support */
#define CONFIG_SYS_I2C_SPEED		400000	/* I2C speed and slave address */
#define CONFIG_SYS_I2C_SLAVE		0x7F
#define CONFIG_SYS_I2C_OFFSET		0x3000
#define CONFIG_SYS_I2C2_OFFSET		0x3100
#define CONFIG_I2C_MULTI_BUS

/* PEX8518 slave I2C interface */
#define CONFIG_SYS_I2C_PEX8518_ADDR	0x70

/* I2C DS1631 temperature sensor */
#define CONFIG_SYS_I2C_DS1621_ADDR	0x48
#define CONFIG_DTT_DS1621
#define CONFIG_DTT_SENSORS		{ 0 }

/* I2C EEPROM - AT24C128B */
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x54
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		2
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	6	/* 64 byte pages */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	10	/* take up to 10 msec */

/* I2C RTC */
#define CONFIG_RTC_M41T11		1
#define CONFIG_SYS_I2C_RTC_ADDR		0x68
#define CONFIG_SYS_M41T11_BASE_YEAR	2000

/* GPIO/EEPROM/SRAM */
#define CONFIG_DS4510
#define CONFIG_SYS_I2C_DS4510_ADDR	0x51

/* GPIO */
#define CONFIG_PCA953X
#define CONFIG_SYS_I2C_PCA953X_ADDR0	0x18
#define CONFIG_SYS_I2C_PCA953X_ADDR1	0x1c
#define CONFIG_SYS_I2C_PCA953X_ADDR2	0x1e
#define CONFIG_SYS_I2C_PCA953X_ADDR3	0x1f
#define CONFIG_SYS_I2C_PCA953X_ADDR	CONFIG_SYS_I2C_PCA953X_ADDR0

/* PCA9557 @ 0x18*/
#define CONFIG_SYS_PCA953X_C0_SER0_EN		0x01 /* PU; UART0 enable (1: enabled) */
#define CONFIG_SYS_PCA953X_C0_SER0_MODE		0x02 /* PU; UART0 serial mode select */
#define CONFIG_SYS_PCA953X_C0_SER1_EN		0x04 /* PU; UART1 enable (1: enabled) */
#define CONFIG_SYS_PCA953X_C0_SER1_MODE		0x08 /* PU; UART1 serial mode select */
#define CONFIG_SYS_PCA953X_C0_FLASH_PASS_CS	0x10 /* PU; Boot flash CS select */
#define CONFIG_SYS_PCA953X_NVM_WP		0x20 /* PU; Set to 0 to enable NVM writing */
#define CONFIG_SYS_PCA953X_C0_VCORE_VID2	0x40 /* VID2 of ISL6262 */
#define CONFIG_SYS_PCA953X_C0_VCORE_VID3	0x80 /* VID3 of ISL6262 */

/* PCA9557 @ 0x1c*/
#define CONFIG_SYS_PCA953X_XMC0_ROOT0		0x01 /* PU; Low if XMC is RC */
#define CONFIG_SYS_PCA953X_XMC0_MVMR0		0x02 /* XMC EEPROM write protect */
#define CONFIG_SYS_PCA953X_XMC0_WAKE		0x04 /* PU; XMC wake */
#define CONFIG_SYS_PCA953X_XMC0_BIST		0x08 /* PU; XMC built in self test */
#define CONFIG_SYS_PCA953X_XMC_PRESENT		0x10 /* PU; Low if XMC module installed */
#define CONFIG_SYS_PCA953X_PMC_PRESENT		0x20 /* PU; Low if PMC module installed */
#define CONFIG_SYS_PCA953X_PMC0_MONARCH		0x40 /* PMC monarch mode enable */
#define CONFIG_SYS_PCA953X_PMC0_EREADY		0x80 /* PU; PMC PCI eready */

/* PCA9557 @ 0x1e*/
#define CONFIG_SYS_PCA953X_P0_GA0		0x01 /* PU; VPX Geographical address */
#define CONFIG_SYS_PCA953X_P0_GA1		0x02 /* PU; VPX Geographical address */
#define CONFIG_SYS_PCA953X_P0_GA2		0x04 /* PU; VPX Geographical address */
#define CONFIG_SYS_PCA953X_P0_GA3		0x08 /* PU; VPX Geographical address */
#define CONFIG_SYS_PCA953X_P0_GA4		0x10 /* PU; VPX Geographical address */
#define CONFIG_SYS_PCA953X_P0_GAP		0x20 /* PU; tied to VPX P0.GAP */
#define CONFIG_SYS_PCA953X_P1_SYSEN		0x80 /* PU; Pulled high; tied to VPX P1.SYSCON */

/* PCA9557 @ 0x1f */
#define CONFIG_SYS_PCA953X_GPIO_VPX0		0x01 /* PU */
#define CONFIG_SYS_PCA953X_GPIO_VPX1		0x02 /* PU */
#define CONFIG_SYS_PCA953X_GPIO_VPX2		0x04 /* PU */
#define CONFIG_SYS_PCA953X_GPIO_VPX3		0x08 /* PU */
#define CONFIG_SYS_PCA953X_VPX_FRU_WRCTL	0x10 /* PD; I2C master source for FRU SEEPROM */

/* PCIE1 - VPX P1 */
#define CONFIG_SYS_PCIE1_MEM_BASE	0x80000000
#define CONFIG_SYS_PCIE1_MEM_PHYS	CONFIG_SYS_PCIE1_MEM_BASE
#define CONFIG_SYS_PCIE1_MEM_SIZE	0x40000000	/* 1G */
#define CONFIG_SYS_PCIE1_IO_BASE	0x00000000
#define CONFIG_SYS_PCIE1_IO_PHYS	0xe8000000
#define CONFIG_SYS_PCIE1_IO_SIZE	0x00800000	/* 8M */

/* PCIE2 - PEX8518 */
#define CONFIG_SYS_PCIE2_MEM_BASE	0xc0000000
#define CONFIG_SYS_PCIE2_MEM_PHYS	CONFIG_SYS_PCIE2_MEM_BASE
#define CONFIG_SYS_PCIE2_MEM_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCIE2_IO_BASE	0x00000000
#define CONFIG_SYS_PCIE2_IO_PHYS	0xe8800000
#define CONFIG_SYS_PCIE2_IO_SIZE	0x00800000	/* 8M */

#define CONFIG_TSEC_ENET		/* tsec ethernet support */
#define CONFIG_PHY_GIGE		1	/* Include GbE speed/duplex detection */
#define CONFIG_NET_MULTI	1
#define CONFIG_TSEC_TBI
#define CONFIG_MII		1	/* MII PHY management */
#define CONFIG_MII_DEFAULT_TSEC	1	/* Allow unregistered phys */
#define CONFIG_ETHPRIME		"eTSEC2"

#define CONFIG_TSEC1		1
#define CONFIG_TSEC1_NAME	"eTSEC1"
#define TSEC1_FLAGS		(TSEC_GIGABIT | TSEC_REDUCED)
#define TSEC1_PHY_ADDR		1
#define TSEC1_PHYIDX		0
#define CONFIG_HAS_ETH0

#define CONFIG_TSEC2		1
#define CONFIG_TSEC2_NAME	"eTSEC2"
#define TSEC2_FLAGS		(TSEC_GIGABIT | TSEC_REDUCED)
#define TSEC2_PHY_ADDR		2
#define TSEC2_PHYIDX		0
#define CONFIG_HAS_ETH1

#include <config_cmd_default.h>

#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_DATE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_DS4510
#define CONFIG_CMD_DS4510_INFO
#define CONFIG_CMD_DTT
#define CONFIG_CMD_EEPROM
#define CONFIG_CMD_ELF
#define CONFIG_CMD_FLASH
#define CONFIG_CMD_I2C
#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MII
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NET
#define CONFIG_CMD_PCA953X
#define CONFIG_CMD_PCA953X_INFO
#define CONFIG_CMD_PCI
#define CONFIG_CMD_PING
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_SNTP

#define CONFIG_SYS_LONGHELP			/* undef to save memory	*/
#define CONFIG_SYS_LOAD_ADDR	0x2000000	/* default load address */
#define CONFIG_SYS_PROMPT	"=> "		/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	256		/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */
#define CONFIG_SYS_MAXARGS	16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size */
#define CONFIG_SYS_HZ		1000		/* decrementer freq: 1ms ticks */
#define CONFIG_CMDLINE_EDITING	1		/* add command line history	*/
#define CONFIG_LOADADDR		0x1000000	/* default location for tftp and bootm */
#define CONFIG_BOOTDELAY	3		/* -1 disables auto-boot */
#define CONFIG_PANIC_HANG			/* do not reset board on panic */
#define CONFIG_PREBOOT				/* enable preboot variable */
#define CONFIG_FIT		1
#define CONFIG_FIT_VERBOSE	1
#define CONFIG_INTEGRITY			/* support booting INTEGRITY OS */

#define CONFIG_SYS_BOOTMAPSZ	(16 << 20)	/* Initial Memory map for Linux*/
#define CONFIG_SYS_BOOTM_LEN	(16 << 20)	/* Increase max gunzip size */

#define BOOTFLAG_COLD		0x01		/* Normal Power-On: Boot from FLASH */
#define BOOTFLAG_WARM		0x02		/* Software reboot */

#define CONFIG_ENV_IS_IN_FLASH	1
#define CONFIG_ENV_SECT_SIZE	0x20000		/* 128k (one sector) for env */
#define CONFIG_ENV_SIZE		0x8000
#define CONFIG_ENV_ADDR		(CONFIG_SYS_MONITOR_BASE - (256 * 1024))

#define CONFIG_UBOOT1_ENV_ADDR	MK_STR(0xfff80000)
#define CONFIG_UBOOT2_ENV_ADDR	MK_STR(0xf7f80000)
#define CONFIG_FDT1_ENV_ADDR	MK_STR(0xfff00000)
#define CONFIG_FDT2_ENV_ADDR	MK_STR(0xf7f00000)
#define CONFIG_OS1_ENV_ADDR	MK_STR(0xfef00000)
#define CONFIG_OS2_ENV_ADDR	MK_STR(0xf6f00000)

#define CONFIG_PROG_UBOOT1						\
	"$download_cmd $loadaddr $ubootfile; "				\
	"if test $? -eq 0; then "					\
		"protect off "CONFIG_UBOOT1_ENV_ADDR" +80000; "		\
		"erase "CONFIG_UBOOT1_ENV_ADDR" +80000; "		\
		"cp.w $loadaddr "CONFIG_UBOOT1_ENV_ADDR" 40000; "	\
		"protect on "CONFIG_UBOOT1_ENV_ADDR" +80000; "		\
		"cmp.b $loadaddr "CONFIG_UBOOT1_ENV_ADDR" 80000; "	\
		"if test $? -ne 0; then "				\
			"echo PROGRAM FAILED; "				\
		"else; "						\
			"echo PROGRAM SUCCEEDED; "			\
		"fi; "							\
	"else; "							\
		"echo DOWNLOAD FAILED; "				\
	"fi;"

#define CONFIG_PROG_UBOOT2						\
	"$download_cmd $loadaddr $ubootfile; "				\
	"if test $? -eq 0; then "					\
		"protect off "CONFIG_UBOOT2_ENV_ADDR" +80000; "		\
		"erase "CONFIG_UBOOT2_ENV_ADDR" +80000; "		\
		"cp.w $loadaddr "CONFIG_UBOOT2_ENV_ADDR" 40000; "	\
		"protect on "CONFIG_UBOOT2_ENV_ADDR" +80000; "		\
		"cmp.b $loadaddr "CONFIG_UBOOT2_ENV_ADDR" 80000; "	\
		"if test $? -ne 0; then "				\
			"echo PROGRAM FAILED; "				\
		"else; "						\
			"echo PROGRAM SUCCEEDED; "			\
		"fi; "							\
	"else; "							\
		"echo DOWNLOAD FAILED; "				\
	"fi;"

#define CONFIG_BOOT_OS_NET						\
	"$download_cmd $osaddr $osfile; "				\
	"if test $? -eq 0; then "					\
		"if test -n $fdtaddr; then "				\
			"$download_cmd $fdtaddr $fdtfile; "		\
			"if test $? -eq 0; then "			\
				"bootm $osaddr - $fdtaddr; "		\
			"else; "					\
				"echo FDT DOWNLOAD FAILED; "		\
			"fi; "						\
		"else; "						\
			"bootm $osaddr; "				\
		"fi; "							\
	"else; "							\
		"echo OS DOWNLOAD FAILED; "				\
	"fi;"

#define CONFIG_PROG_OS1							\
	"$download_cmd $osaddr $osfile; "				\
	"if test $? -eq 0; then "					\
		"erase "CONFIG_OS1_ENV_ADDR" +$filesize; "		\
		"cp.b $osaddr "CONFIG_OS1_ENV_ADDR" $filesize; "	\
		"cmp.b $osaddr "CONFIG_OS1_ENV_ADDR" $filesize; "	\
		"if test $? -ne 0; then "				\
			"echo OS PROGRAM FAILED; "			\
		"else; "						\
			"echo OS PROGRAM SUCCEEDED; "			\
		"fi; "							\
	"else; "							\
		"echo OS DOWNLOAD FAILED; "				\
	"fi;"

#define CONFIG_PROG_OS2							\
	"$download_cmd $osaddr $osfile; "				\
	"if test $? -eq 0; then "					\
		"erase "CONFIG_OS2_ENV_ADDR" +$filesize; "		\
		"cp.b $osaddr "CONFIG_OS2_ENV_ADDR" $filesize; "	\
		"cmp.b $osaddr "CONFIG_OS2_ENV_ADDR" $filesize; "	\
		"if test $? -ne 0; then "				\
			"echo OS PROGRAM FAILED; "			\
		"else; "						\
			"echo OS PROGRAM SUCCEEDED; "			\
		"fi; "							\
	"else; "							\
		"echo OS DOWNLOAD FAILED; "				\
	"fi;"

#define CONFIG_PROG_FDT1						\
	"$download_cmd $fdtaddr $fdtfile; "				\
	"if test $? -eq 0; then "					\
		"erase "CONFIG_FDT1_ENV_ADDR" +$filesize;"		\
		"cp.b $fdtaddr "CONFIG_FDT1_ENV_ADDR" $filesize; "	\
		"cmp.b $fdtaddr "CONFIG_FDT1_ENV_ADDR" $filesize; "	\
		"if test $? -ne 0; then "				\
			"echo FDT PROGRAM FAILED; "			\
		"else; "						\
			"echo FDT PROGRAM SUCCEEDED; "			\
		"fi; "							\
	"else; "							\
		"echo FDT DOWNLOAD FAILED; "				\
	"fi;"

#define CONFIG_PROG_FDT2						\
	"$download_cmd $fdtaddr $fdtfile; "				\
	"if test $? -eq 0; then "					\
		"erase "CONFIG_FDT2_ENV_ADDR" +$filesize;"		\
		"cp.b $fdtaddr "CONFIG_FDT2_ENV_ADDR" $filesize; "	\
		"cmp.b $fdtaddr "CONFIG_FDT2_ENV_ADDR" $filesize; "	\
		"if test $? -ne 0; then "				\
			"echo FDT PROGRAM FAILED; "			\
		"else; "						\
			"echo FDT PROGRAM SUCCEEDED; "			\
		"fi; "							\
	"else; "							\
		"echo FDT DOWNLOAD FAILED; "				\
	"fi;"

#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"autoload=yes\0"						\
	"download_cmd=tftp\0"						\
	"console_args=console=ttyS0,115200\0"				\
	"root_args=root=/dev/nfs rw\0"					\
	"misc_args=ip=on\0"						\
	"set_bootargs=setenv bootargs ${console_args} ${root_args} ${misc_args}\0" \
	"bootfile=/home/user/file\0"					\
	"osfile=/home/user/uImage-XPedite5370\0"			\
	"fdtfile=/home/user/xpedite5370.dtb\0"				\
	"ubootfile=/home/user/u-boot.bin\0"				\
	"fdtaddr=c00000\0"						\
	"osaddr=0x1000000\0"						\
	"loadaddr=0x1000000\0"						\
	"prog_uboot1="CONFIG_PROG_UBOOT1"\0"				\
	"prog_uboot2="CONFIG_PROG_UBOOT2"\0"				\
	"prog_os1="CONFIG_PROG_OS1"\0"					\
	"prog_os2="CONFIG_PROG_OS2"\0"					\
	"prog_fdt1="CONFIG_PROG_FDT1"\0"				\
	"prog_fdt2="CONFIG_PROG_FDT2"\0"				\
	"bootcmd_net=run set_bootargs; "CONFIG_BOOT_OS_NET"\0"		\
	"bootcmd_flash1=run set_bootargs; "				\
		"bootm "CONFIG_OS1_ENV_ADDR" - "CONFIG_FDT1_ENV_ADDR"\0"\
	"bootcmd_flash2=run set_bootargs; "				\
		"bootm "CONFIG_OS2_ENV_ADDR" - "CONFIG_FDT2_ENV_ADDR"\0"\
	"bootcmd=run bootcmd_flash1\0"
#endif	/* __CONFIG_H */