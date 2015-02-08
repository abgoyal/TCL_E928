

#ifndef __CONFIG_BF533_EZKIT_H__
#define __CONFIG_BF533_EZKIT_H__

#include <asm/config-pre.h>


#define CONFIG_BFIN_CPU             bf533-0.3
#define CONFIG_BFIN_BOOT_MODE       BFIN_BOOT_BYPASS


/* CONFIG_CLKIN_HZ is any value in Hz					*/
#define CONFIG_CLKIN_HZ			27000000
/* CLKIN_HALF controls the DF bit in PLL_CTL      0 = CLKIN		*/
/*                                                1 = CLKIN / 2		*/
#define CONFIG_CLKIN_HALF		0
/* PLL_BYPASS controls the BYPASS bit in PLL_CTL  0 = do not bypass	*/
/*                                                1 = bypass PLL	*/
#define CONFIG_PLL_BYPASS		0
/* VCO_MULT controls the MSEL (multiplier) bits in PLL_CTL		*/
/* Values can range from 0-63 (where 0 means 64)			*/
#define CONFIG_VCO_MULT			22
/* CCLK_DIV controls the core clock divider				*/
/* Values can be 1, 2, 4, or 8 ONLY					*/
#define CONFIG_CCLK_DIV			1
/* SCLK_DIV controls the system clock divider				*/
/* Values can range from 1-15						*/
#define CONFIG_SCLK_DIV			5


#define CONFIG_MEM_SIZE		32
/* Early EZKITs had 32megs, but later have 64megs */
#if (CONFIG_MEM_SIZE == 64)
# define CONFIG_MEM_ADD_WDTH	10
#else
# define CONFIG_MEM_ADD_WDTH	9
#endif

#define CONFIG_EBIU_SDRRC_VAL	0x398
#define CONFIG_EBIU_SDGCTL_VAL	0x91118d

#define CONFIG_EBIU_AMGCTL_VAL	0xFF
#define CONFIG_EBIU_AMBCTL0_VAL	0x7BB07BB0
#define CONFIG_EBIU_AMBCTL1_VAL	0xFFC27BB0

#define CONFIG_SYS_MONITOR_LEN	(256 * 1024)
#define CONFIG_SYS_MALLOC_LEN	(128 * 1024)


#define ADI_CMDS_NETWORK	1
#define CONFIG_NET_MULTI
#define CONFIG_SMC91111	1
#define CONFIG_SMC91111_BASE	0x20310300
#define SMC91111_EEPROM_INIT() \
	do { \
		bfin_write_FIO_DIR(bfin_read_FIO_DIR() | PF1 | PF0); \
		bfin_write_FIO_FLAG_C(PF1); \
		bfin_write_FIO_FLAG_S(PF0); \
		SSYNC(); \
	} while (0)
#define CONFIG_HOSTNAME		bf533-ezkit
/* Uncomment next line to use fixed MAC address */
/* #define CONFIG_ETHADDR	02:80:ad:20:31:e8 */


#define CONFIG_SYS_FLASH_BASE		0x20000000
#define CONFIG_SYS_MAX_FLASH_BANKS	3
#define CONFIG_SYS_MAX_FLASH_SECT	40
#define CONFIG_ENV_IS_IN_FLASH
#define CONFIG_ENV_ADDR		0x20030000
#define CONFIG_ENV_SECT_SIZE	0x10000
#define FLASH_TOT_SECT		40


#define CONFIG_SOFT_I2C
#ifdef CONFIG_SOFT_I2C
#define PF_SCL PF0
#define PF_SDA PF1
#define I2C_INIT \
	do { \
		*pFIO_DIR |= PF_SCL; \
		SSYNC(); \
	} while (0)
#define I2C_ACTIVE \
	do { \
		*pFIO_DIR |= PF_SDA; \
		*pFIO_INEN &= ~PF_SDA; \
		SSYNC(); \
	} while (0)
#define I2C_TRISTATE \
	do { \
		*pFIO_DIR &= ~PF_SDA; \
		*pFIO_INEN |= PF_SDA; \
		SSYNC(); \
	} while (0)
#define I2C_READ ((*pFIO_FLAG_D & PF_SDA) != 0)
#define I2C_SDA(bit) \
	do { \
		if (bit) \
			*pFIO_FLAG_S = PF_SDA; \
		else \
			*pFIO_FLAG_C = PF_SDA; \
		SSYNC(); \
	} while (0)
#define I2C_SCL(bit) \
	do { \
		if (bit) \
			*pFIO_FLAG_S = PF_SCL; \
		else \
			*pFIO_FLAG_C = PF_SCL; \
		SSYNC(); \
	} while (0)
#define I2C_DELAY		udelay(5)	/* 1/4 I2C clock duration */

#define CONFIG_SYS_I2C_SPEED		50000
#define CONFIG_SYS_I2C_SLAVE		0
#endif


#define CONFIG_MISC_INIT_R
#define CONFIG_RTC_BFIN
#define CONFIG_UART_CONSOLE	0


#include <configs/bfin_adi_common.h>

#endif