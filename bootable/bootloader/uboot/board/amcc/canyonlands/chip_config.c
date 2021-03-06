

#include <common.h>
#include <asm/ppc4xx_config.h>

struct ppc4xx_config ppc4xx_config_val[] = {
	{
		"600-nor", "NOR  CPU: 600 PLB: 200 OPB: 100 EBC: 100",
		{
			0x86, 0x80, 0xce, 0x1f, 0x79, 0x80, 0x00, 0xa0,
			0x40, 0x08, 0x23, 0x50, 0x0d, 0x05, 0x00, 0x00
		}
	},
	{
		"800-nor", "NOR  CPU: 800 PLB: 200 OPB: 100 EBC: 100",
		{
			0x86, 0x80, 0xba, 0x14, 0x99, 0x80, 0x00, 0xa0,
			0x40, 0x08, 0x23, 0x50, 0x0d, 0x05, 0x00, 0x00
		}
	},
	{
		"1000-nor", "NOR  CPU:1000 PLB: 200 OPB: 100 EBC: 100",
		{
			0x86, 0x82, 0x96, 0x19, 0xb9, 0x80, 0x00, 0xa0,
			0x40, 0x08, 0x23, 0x50, 0x0d, 0x05, 0x00, 0x00
		}
	},
	{
		"1066-nor", "NOR  CPU:1066 PLB: 266 OPB:  88 EBC:  88",
		{
			0x86, 0x80, 0xb3, 0x01, 0x9d, 0x80, 0x00, 0xa0,
			0x40, 0x08, 0x23, 0x50, 0x0d, 0x05, 0x00, 0x00
		}
	},
#if !defined(CONFIG_ARCHES)
	{
		"600-nand", "NAND CPU: 600 PLB: 200 OPB: 100 EBC: 100",
		{
			0x86, 0x80, 0xce, 0x1f, 0x79, 0x90, 0x01, 0xa0,
			0xa0, 0xe8, 0x23, 0x58, 0x0d, 0x05, 0x00, 0x00
		}
	},
	{
		"800-nand", "NAND CPU: 800 PLB: 200 OPB: 100 EBC: 100",
		{
			0x86, 0x80, 0xba, 0x14, 0x99, 0x90, 0x01, 0xa0,
			0xa0, 0xe8, 0x23, 0x58, 0x0d, 0x05, 0x00, 0x00
		}
	},
	{
		"1000-nand", "NAND CPU:1000 PLB: 200 OPB: 100 EBC: 100",
		{
			0x86, 0x82, 0x96, 0x19, 0xb9, 0x90, 0x01, 0xa0,
			0xa0, 0xe8, 0x23, 0x58, 0x0d, 0x05, 0x00, 0x00
		}
	},
	{
		"1066-nand", "NAND CPU:1066 PLB: 266 OPB:  88 EBC:  88",
		{
			0x86, 0x80, 0xb3, 0x01, 0x9d, 0x90, 0x01, 0xa0,
			0xa0, 0xe8, 0x23, 0x58, 0x0d, 0x05, 0x00, 0x00
		}
	},
#endif
};

int ppc4xx_config_count = ARRAY_SIZE(ppc4xx_config_val);
