


#include <common.h>
#include <command.h>

#ifdef CONFIG_USE_IRQ
DECLARE_GLOBAL_DATA_PTR;
#endif

/* read co-processor 15, register #1 (control register) */
static unsigned long read_p15_c1 (void)
{
	unsigned long value;

	__asm__ __volatile__(
		"mrc	p15, 0, %0, c1, c0, 0   @ read control reg\n"
		: "=r" (value)
		:
		: "memory");

	return value;
}

/* write to co-processor 15, register #1 (control register) */
static void write_p15_c1 (unsigned long value)
{

	__asm__ __volatile__(
		"mcr	p15, 0, %0, c1, c0, 0   @ write it back\n"
		:
		: "r" (value)
		: "memory");

	read_p15_c1 ();
}

static void cp_delay (void)
{
	volatile int i;

	/* copro seems to need some delay between reading and writing */
	for (i = 0; i < 100; i++);
}

/* See also ARM926EJ-S Technical Reference Manual */
#define C1_MMU		(1<<0)		/* mmu off/on */
#define C1_ALIGN	(1<<1)		/* alignment faults off/on */
#define C1_DC		(1<<2)		/* dcache off/on */

#define C1_BIG_ENDIAN	(1<<7)		/* big endian off/on */
#define C1_SYS_PROT	(1<<8)		/* system protection */
#define C1_ROM_PROT	(1<<9)		/* ROM protection */
#define C1_IC		(1<<12)		/* icache off/on */
#define C1_HIGH_VECTORS	(1<<13)		/* location of vectors: low/high addresses */


int cpu_init (void)
{
	/*
	 * setup up stacks if necessary
	 */
	UBOOT_TRACER;	 
#ifdef CONFIG_USE_IRQ
	IRQ_STACK_START = _armboot_start - CFG_MALLOC_LEN - CFG_GBL_DATA_SIZE - 4;
	FIQ_STACK_START = IRQ_STACK_START - CONFIG_STACKSIZE_IRQ;
#endif
	return 0;
}

void cpu_clean_env (void)
{
	unsigned long i;

	disable_interrupts ();

	// (1) make sure the cache is off (turn off I/D-cache)
	asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));
	i &= ~(C1_DC | C1_IC);
	asm ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));

	// (2) make sure the cache is clean (flush I/D-cache)
	i = 0;
	asm ("mcr p15, 0, %0, c7, c7, 0": :"r" (i));
}

int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	disable_interrupts ();
	reset_cpu (0);
	/*NOTREACHED*/
	return (0);
}

void icache_enable (void)
{
	ulong reg;

	reg = read_p15_c1 ();		/* get control reg. */
	cp_delay ();
	write_p15_c1 (reg | C1_IC);
}

void icache_disable (void)
{
	ulong reg;

	reg = read_p15_c1 ();
	cp_delay ();
	write_p15_c1 (reg & ~C1_IC);
}

int icache_status (void)
{
	return (read_p15_c1 () & C1_IC) != 0;
}