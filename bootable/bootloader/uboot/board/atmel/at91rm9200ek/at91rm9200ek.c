

#include <common.h>
#include <exports.h>
#include <netdev.h>
#include <asm/arch/AT91RM9200.h>
#include <asm/io.h>
#if defined(CONFIG_DRIVER_ETHER)
#include <at91rm9200_net.h>
#include <dm9161.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */

int board_init (void)
{
	/* Enable Ctrlc */
	console_init_f ();

	/*
	 * Correct IRDA resistor problem
	 * Set PA23_TXD in Output
	 */
	writel(AT91C_PA23_TXD2, ((AT91PS_PIO) AT91C_BASE_PIOA)->PIO_OER);

	/*
	 * memory and cpu-speed are setup before relocation
	 * so we do _nothing_ here
	 */

	/* arch number of AT91RM9200EK-Board */
	gd->bd->bi_arch_number = MACH_TYPE_AT91RM9200EK;
	/* adress of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	return 0;
}

int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_SIZE;
	return 0;
}

#if defined(CONFIG_DRIVER_ETHER) && defined(CONFIG_CMD_NET)
void at91rm9200_GetPhyInterface(AT91PS_PhyOps p_phyops)
{
	p_phyops->Init = dm9161_InitPhy;
	p_phyops->IsPhyConnected = dm9161_IsPhyConnected;
	p_phyops->GetLinkSpeed = dm9161_GetLinkSpeed;
	p_phyops->AutoNegotiate = dm9161_AutoNegotiate;
}
#endif

#ifdef CONFIG_DRIVER_AT91EMAC
int board_eth_init(bd_t *bis)
{
	int rc = 0;
	rc = at91emac_register(bis, 0);
	return rc;
}
#endif