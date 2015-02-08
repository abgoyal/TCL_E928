

#include <common.h>
#include <asm/arch/hardware.h>

DECLARE_GLOBAL_DATA_PTR;

void serial_setbrg (void)
{
	unsigned short divisor = 0;

	switch (gd->baudrate) {
	case   1200:	divisor = 3072;	break;
	case   9600:	divisor =  384;	break;
	case  19200:	divisor =  192;	break;
	case  38400:	divisor =   96;	break;
	case  57600:	divisor =   64;	break;
	case 115200:	divisor =   32;	break;
	default:	hang ();	break;
	}

	/* init serial UART0 */
	PUT8(U0LCR, 0);
	PUT8(U0IER, 0);
	PUT8(U0LCR, 0x80);	/* DLAB=1 */
	PUT8(U0DLL, (unsigned char)(divisor & 0x00FF));
	PUT8(U0DLM, (unsigned char)(divisor >> 8));
	PUT8(U0LCR, 0x03);	/* 8N1, DLAB=0  */
	PUT8(U0FCR, 1);		/* Enable RX and TX FIFOs */
}

int serial_init (void)
{
	unsigned long pinsel0;

	serial_setbrg ();

	pinsel0 = GET32(PINSEL0);
	pinsel0 &= ~(0x00000003);
	pinsel0 |= 5;
	PUT32(PINSEL0, pinsel0);

	return (0);
}

void serial_putc (const char c)
{
	if (c == '\n')
	{
		while((GET8(U0LSR) & (1<<5)) == 0); /* Wait for empty U0THR */
		PUT8(U0THR, '\r');
	}

	while((GET8(U0LSR) & (1<<5)) == 0); /* Wait for empty U0THR */
	PUT8(U0THR, c);
}

int serial_getc (void)
{
	while((GET8(U0LSR) & 1) == 0);
	return GET8(U0RBR);
}

void
serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

/* Test if there is a byte to read */
int serial_tstc (void)
{
	return (GET8(U0LSR) & 1);
}