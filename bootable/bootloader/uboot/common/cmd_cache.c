

#include <common.h>
#include <command.h>

static int on_off (const char *);

int do_icache ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	switch (argc) {
	case 2:			/* on / off	*/
		switch (on_off(argv[1])) {
#if 0	/* prevented by varargs handling; FALLTROUGH is harmless, too */
		default: cmd_usage(cmdtp);
			return;
#endif
		case 0:	icache_disable();
			break;
		case 1:	icache_enable ();
			break;
		}
		/* FALL TROUGH */
	case 1:			/* get status */
		printf ("Instruction Cache is %s\n",
			icache_status() ? "ON" : "OFF");
		return 0;
	default:
		cmd_usage(cmdtp);
		return 1;
	}
	return 0;
}

int do_dcache ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	switch (argc) {
	case 2:			/* on / off	*/
		switch (on_off(argv[1])) {
#if 0	/* prevented by varargs handling; FALLTROUGH is harmless, too */
		default: cmd_usage(cmdtp);
			return;
#endif
		case 0:	dcache_disable();
			break;
		case 1:	dcache_enable ();
			break;
		}
		/* FALL TROUGH */
	case 1:			/* get status */
		printf ("Data (writethrough) Cache is %s\n",
			dcache_status() ? "ON" : "OFF");
		return 0;
	default:
		cmd_usage(cmdtp);
		return 1;
	}
	return 0;

}

static int on_off (const char *s)
{
	if (strcmp(s, "on") == 0) {
		return (1);
	} else if (strcmp(s, "off") == 0) {
		return (0);
	}
	return (-1);
}


U_BOOT_CMD(
	icache,   2,   1,     do_icache,
	"enable or disable instruction cache",
	"[on, off]\n"
	"    - enable or disable instruction cache"
);

U_BOOT_CMD(
	dcache,   2,   1,     do_dcache,
	"enable or disable data cache",
	"[on, off]\n"
	"    - enable or disable data (writethrough) cache"
);