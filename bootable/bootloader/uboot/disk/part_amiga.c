
#include <common.h>
#include <command.h>
#include <ide.h>
#include "part_amiga.h"

#if defined(CONFIG_CMD_IDE) || \
    defined(CONFIG_CMD_MG_DISK) || \
    defined(CONFIG_CMD_SCSI) || \
    defined(CONFIG_CMD_USB) || \
    defined(CONFIG_MMC) || \
    defined(CONFIG_SYSTEMACE)

#undef AMIGA_DEBUG

#ifdef AMIGA_DEBUG
#define PRINTF(fmt, args...) printf(fmt ,##args)
#else
#define PRINTF(fmt, args...)
#endif

struct block_header
{
    u32 id;
    u32 summed_longs;
    s32 chk_sum;
};

static unsigned char block_buffer[DEFAULT_SECTOR_SIZE];
static struct rigid_disk_block rdb = {0};
static struct bootcode_block bootcode = {0};

static void bcpl_strcpy(char *to, char *from)
{
    int len = *from++;

    while (len)
    {
	*to++ = *from++;
	len--;
    }
    *to = 0;
}

static void bstr_print(char *string)
{
    int len = *string++;
    char buffer[256];
    int i;

    i = 0;
    while (len)
    {
	buffer[i++] = *string++;
	len--;
    }

    buffer[i] = 0;
    printf("%-10s", buffer);
}

int sum_block(struct block_header *header)
{
    s32 *block = (s32 *)header;
    u32 i;
    s32 sum = 0;

    for (i = 0; i < header->summed_longs; i++)
	sum += *block++;

    return (sum != 0);
}

static void print_disk_type(u32 disk_type)
{
    char buffer[6];
    buffer[0] = (disk_type & 0xFF000000)>>24;
    buffer[1] = (disk_type & 0x00FF0000)>>16;
    buffer[2] = (disk_type & 0x0000FF00)>>8;
    buffer[3] = '\\';
    buffer[4] = (disk_type & 0x000000FF) + '0';
    buffer[5] = 0;
    printf("%s", buffer);
}

static void print_part_info(struct partition_block *p)
{
    struct amiga_part_geometry *g;

    g = (struct amiga_part_geometry *)&(p->environment);

    bstr_print(p->drive_name);
    printf("%6d\t%6d\t",
	   g->low_cyl * g->block_per_track * g->surfaces ,
	   (g->high_cyl - g->low_cyl + 1) * g->block_per_track * g->surfaces - 1);
    print_disk_type(g->dos_type);
    printf("\t%5d\n", g->boot_priority);
}

struct rigid_disk_block *get_rdisk(block_dev_desc_t *dev_desc)
{
    int i;
    int limit;
    char *s;

    s = getenv("amiga_scanlimit");
    if (s)
	limit = simple_strtoul(s, NULL, 10);
    else
	limit = AMIGA_BLOCK_LIMIT;

    for (i=0; i<limit; i++)
    {
	ulong res = dev_desc->block_read(dev_desc->dev, i, 1,
					 (ulong *)block_buffer);
	if (res == 1)
	{
	    struct rigid_disk_block *trdb = (struct rigid_disk_block *)block_buffer;
	    if (trdb->id == AMIGA_ID_RDISK)
	    {
		PRINTF("Rigid disk block suspect at %d, checking checksum\n",i);
		if (sum_block((struct block_header *)block_buffer) == 0)
		{
		    PRINTF("FOUND\n");
		    memcpy(&rdb, trdb, sizeof(struct rigid_disk_block));
		    return (struct rigid_disk_block *)&rdb;
		}
	    }
	}
    }
    PRINTF("Done scanning, no RDB found\n");
    return NULL;
}


struct bootcode_block *get_bootcode(block_dev_desc_t *dev_desc)
{
    int i;
    int limit;
    char *s;

    s = getenv("amiga_scanlimit");
    if (s)
	limit = simple_strtoul(s, NULL, 10);
    else
	limit = AMIGA_BLOCK_LIMIT;

    PRINTF("Scanning for BOOT from 0 to %d\n", limit);

    for (i = 0; i < limit; i++)
    {
	ulong res = dev_desc->block_read(dev_desc->dev, i, 1, (ulong *)block_buffer);
	if (res == 1)
	{
	    struct bootcode_block *boot = (struct bootcode_block *)block_buffer;
	    if (boot->id == AMIGA_ID_BOOT)
	    {
		PRINTF("BOOT block at %d, checking checksum\n", i);
		if (sum_block((struct block_header *)block_buffer) == 0)
		{
		    PRINTF("Found valid bootcode block\n");
		    memcpy(&bootcode, boot, sizeof(struct bootcode_block));
		    return &bootcode;
		}
	    }
	}
    }

    PRINTF("No boot code found on disk\n");
    return 0;
}

int test_part_amiga(block_dev_desc_t *dev_desc)
{
    struct rigid_disk_block *rdb;
    struct bootcode_block *bootcode;

    PRINTF("test_part_amiga: Testing for an Amiga RDB partition\n");

    rdb = get_rdisk(dev_desc);
    if (rdb)
    {
	bootcode = get_bootcode(dev_desc);
	if (bootcode)
	    PRINTF("test_part_amiga: bootable Amiga disk\n");
	else
	    PRINTF("test_part_amiga: non-bootable Amiga disk\n");

	return 0;
    }
    else
    {
	PRINTF("test_part_amiga: no RDB found\n");
	return -1;
    }

}

static struct partition_block *find_partition(block_dev_desc_t *dev_desc, int partnum)
{
    struct rigid_disk_block *rdb;
    struct partition_block *p;
    u32 block;

    PRINTF("Trying to find partition block %d\n", partnum);
    rdb = get_rdisk(dev_desc);
    if (!rdb)
    {
	PRINTF("find_partition: no rdb found\n");
	return NULL;
    }

    PRINTF("find_partition: Scanning partition list\n");

    block = rdb->partition_list;
    PRINTF("find_partition: partition list at 0x%x\n", block);

    while (block != 0xFFFFFFFF)
    {
	ulong res = dev_desc->block_read(dev_desc->dev, block, 1,
					 (ulong *)block_buffer);
	if (res == 1)
	{
	    p = (struct partition_block *)block_buffer;
	    if (p->id == AMIGA_ID_PART)
	    {
		PRINTF("PART block suspect at 0x%x, checking checksum\n",block);
		if (sum_block((struct block_header *)p) == 0)
		{
		    if (partnum == 0) break;
		    else
		    {
			partnum--;
			block = p->next;
		    }
		}
	    } else block = 0xFFFFFFFF;
	} else block = 0xFFFFFFFF;
    }

    if (block == 0xFFFFFFFF)
    {
	PRINTF("PART block not found\n");
	return NULL;
    }

    return (struct partition_block *)block_buffer;
}

int get_partition_info_amiga (block_dev_desc_t *dev_desc, int part, disk_partition_t *info)
{
    struct partition_block *p = find_partition(dev_desc, part-1);
    struct amiga_part_geometry *g;
    u32 disk_type;

    if (!p) return -1;

    g = (struct amiga_part_geometry *)&(p->environment);
    info->start = g->low_cyl  * g->block_per_track * g->surfaces;
    info->size  = (g->high_cyl - g->low_cyl + 1) * g->block_per_track * g->surfaces - 1;
    info->blksz = rdb.block_bytes;
    bcpl_strcpy(info->name, p->drive_name);


    disk_type = g->dos_type;

    info->type[0] = (disk_type & 0xFF000000)>>24;
    info->type[1] = (disk_type & 0x00FF0000)>>16;
    info->type[2] = (disk_type & 0x0000FF00)>>8;
    info->type[3] = '\\';
    info->type[4] = (disk_type & 0x000000FF) + '0';
    info->type[5] = 0;

    return 0;
}

void print_part_amiga (block_dev_desc_t *dev_desc)
{
    struct rigid_disk_block *rdb;
    struct bootcode_block *boot;
    struct partition_block *p;
    u32 block;
    int i = 1;

    rdb = get_rdisk(dev_desc);
    if (!rdb)
    {
	PRINTF("print_part_amiga: no rdb found\n");
	return;
    }

    PRINTF("print_part_amiga: Scanning partition list\n");

    block = rdb->partition_list;
    PRINTF("print_part_amiga: partition list at 0x%x\n", block);

    printf("Summary:  DiskBlockSize: %d\n"
	   "          Cylinders    : %d\n"
	   "          Sectors/Track: %d\n"
	   "          Heads        : %d\n\n",
	   rdb->block_bytes, rdb->cylinders, rdb->sectors,
	   rdb->heads);

    printf("                 First   Num. \n"
	   "Nr.  Part. Name  Block   Block  Type        Boot Priority\n");

    while (block != 0xFFFFFFFF)
    {
	ulong res;

	PRINTF("Trying to load block #0x%X\n", block);

	res = dev_desc->block_read(dev_desc->dev, block, 1,
				   (ulong *)block_buffer);
	if (res == 1)
	{
	    p = (struct partition_block *)block_buffer;
	    if (p->id == AMIGA_ID_PART)
	    {
		PRINTF("PART block suspect at 0x%x, checking checksum\n",block);
		if (sum_block((struct block_header *)p) == 0)
		{
		    printf("%-4d ", i); i++;
		    print_part_info(p);
		    block = p->next;
		}
	    } else block = 0xFFFFFFFF;
	} else block = 0xFFFFFFFF;
    }

    boot = get_bootcode(dev_desc);
    if (boot)
    {
	printf("Disk is bootable\n");
    }
}

#endif