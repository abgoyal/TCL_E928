


/* supports both the 16650 duart and the MPSC */

#include <common.h>
#include <command.h>
#include "../include/memory.h"
#include "serial.h"

#ifdef CONFIG_DB64360
#include "../db64360/mpsc.h"
#endif

#ifdef CONFIG_DB64460
#include "../db64460/mpsc.h"
#endif

#include "ns16550.h"

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_MPSC


int serial_init (void)
{
#if (defined CONFIG_SYS_INIT_CHAN1) || (defined CONFIG_SYS_INIT_CHAN2)
	int clock_divisor = 230400 / gd->baudrate;
#endif

	mpsc_init (gd->baudrate);

	/* init the DUART chans so that KGDB in the kernel can use them */
#ifdef CONFIG_SYS_INIT_CHAN1
	NS16550_reinit (COM_PORTS[0], clock_divisor);
#endif
#ifdef CONFIG_SYS_INIT_CHAN2
	NS16550_reinit (COM_PORTS[1], clock_divisor);
#endif
	return (0);
}

void serial_putc (const char c)
{
	if (c == '\n')
		mpsc_putchar ('\r');

	mpsc_putchar (c);
}

int serial_getc (void)
{
	return mpsc_getchar ();
}

int serial_tstc (void)
{
	return mpsc_test_char ();
}

void serial_setbrg (void)
{
	galbrg_set_baudrate (CONFIG_MPSC_PORT, gd->baudrate);
}

#else  /* ! CONFIG_MPSC */

int serial_init (void)
{
	int clock_divisor = 230400 / gd->baudrate;

#ifdef CONFIG_SYS_INIT_CHAN1
	(void) NS16550_init (0, clock_divisor);
#endif
#ifdef CONFIG_SYS_INIT_CHAN2
	(void) NS16550_init (1, clock_divisor);
#endif
	return (0);
}

void serial_putc (const char c)
{
	if (c == '\n')
		NS16550_putc (COM_PORTS[CONFIG_SYS_DUART_CHAN], '\r');

	NS16550_putc (COM_PORTS[CONFIG_SYS_DUART_CHAN], c);
}

int serial_getc (void)
{
	return NS16550_getc (COM_PORTS[CONFIG_SYS_DUART_CHAN]);
}

int serial_tstc (void)
{
	return NS16550_tstc (COM_PORTS[CONFIG_SYS_DUART_CHAN]);
}

void serial_setbrg (void)
{
	int clock_divisor = 230400 / gd->baudrate;

#ifdef CONFIG_SYS_INIT_CHAN1
	NS16550_reinit (COM_PORTS[0], clock_divisor);
#endif
#ifdef CONFIG_SYS_INIT_CHAN2
	NS16550_reinit (COM_PORTS[1], clock_divisor);
#endif
}

#endif /* CONFIG_MPSC */

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

#if defined(CONFIG_CMD_KGDB)
void kgdb_serial_init (void)
{
}

void putDebugChar (int c)
{
	serial_putc (c);
}

void putDebugStr (const char *str)
{
	serial_puts (str);
}

int getDebugChar (void)
{
	return serial_getc ();
}

void kgdb_interruptible (int yes)
{
	return;
}
#endif