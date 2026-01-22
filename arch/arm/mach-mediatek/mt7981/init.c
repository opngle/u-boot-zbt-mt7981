// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022 MediaTek Inc.
 * Author: Sam Shih <sam.shih@mediatek.com>
 */

#include <fdtdec.h>
#include <init.h>
#include <asm/armv8/mmu.h>
#include <asm/system.h>
#include <asm/global_data.h>
#include <linux/sizes.h>

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	int ret;

	ret = fdtdec_setup_mem_size_base();
	if (ret)
		printf("fdt setup mem-base-size failed %d\n",ret);

	gd->ram_size = get_ram_size((void *)gd->ram_base, SZ_1G);
	if (gd->ram_size ==0)
		gd->ram_size =256UL * SZ_1M; //fallback
	printf("Detected Dram size:%lu Mib\n",gd->ram_size / SZ_1M);
	return 0;
}

void reset_cpu(void)
{
	psci_system_reset();
}

static struct mm_region mt7981_mem_map[] = {
	{
		/* DDR */
		.virt = 0x40000000UL,
		.phys = 0x40000000UL,
		.size = 0x80000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_OUTER_SHARE,
	}, {
		.virt = 0x00000000UL,
		.phys = 0x00000000UL,
		.size = 0x40000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		0,
	}
};

struct mm_region *mem_map = mt7981_mem_map;
