

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/io.h>

phys_size_t initdram(int board_type)
{
	/* Sdram is setup by assembler code */
	/* If memory could be changed, we should return the true value here */
	return MEM_SIZE*1024*1024;
}

int checkboard(void)
{
	u32 proc_id;
	u32 config1;

	proc_id = read_c0_prid();
	printf("Board: Qemu -M mips CPU: ");
	switch (proc_id) {
	case 0x00018000:
		printf("4Kc");
		break;
	case 0x00018400:
		printf("4KEcR1");
		break;
	case 0x00019000:
		printf("4KEc");
		break;
	case 0x00019300:
		config1 = read_c0_config1();
		if (config1 & 1)
			printf("24Kf");
		else
			printf("24Kc");
		break;
	case 0x00019500:
		printf("34Kf");
		break;
	case 0x00000400:
		printf("R4000");
		break;
	case 0x00018100:
		config1 = read_c0_config1();
		if (config1 & 1)
			printf("5Kf");
		else
			printf("5Kc");
		break;
	case 0x000182a0:
		printf("20Kc");
		break;

	default:
		printf("unknown");
	}
	printf(" proc_id=0x%x\n", proc_id);

	return 0;
}

int misc_init_r(void)
{
	set_io_port_base(0);
	return 0;
}