#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "statusbar.h"
#include "util.h"

enum meminfo_item_id {
	MI_MemTotal,
	MI_MemFree,
	MI_Buffers,
	MI_Cached,

	_MEMINFO_ITEM_COUNT
};

struct meminfo {
	unsigned long items[(size_t)_MEMINFO_ITEM_COUNT];
};

#define meminfo_item(MI, ID) \
	((MI).items[(size_t)(ID)])

static void read_meminfo(struct meminfo *mi)
{
	char buffer[256];
	const size_t len = read_file("/proc/meminfo", buffer, sizeof buffer);
	buffer[len == sizeof buffer ? sizeof buffer - 1 : len] = '\0';

	char *p = buffer;
	memset(mi, 0, sizeof *mi);
	for (size_t i = (size_t)_MEMINFO_ITEM_COUNT; i > 0; ) {
		const char c0 = *p;
		if (c0 == '\0')
			break;

		const char *const name = p;
		p = strchr(p, ':');
		if (!p)
			continue;
		*p++ = '\0';

		enum meminfo_item_id item_id;
		if (c0 == 'M') {
			if (!strcmp(name, "MemTotal"))
				item_id = MI_MemTotal;
			else if (!strcmp(name, "MemFree"))
				item_id = MI_MemFree;
			else
				item_id = _MEMINFO_ITEM_COUNT;
		} else if (c0 == 'B') {
			if (!strcmp(name, "Buffers"))
				item_id = MI_Buffers;
			else
				item_id = _MEMINFO_ITEM_COUNT;
		} else if (c0 == 'C') {
			if (!strcmp(name, "Cached"))
				item_id = MI_Cached;
			else
				item_id = _MEMINFO_ITEM_COUNT;
		} else {
			item_id = _MEMINFO_ITEM_COUNT;
		}

		if (item_id < _MEMINFO_ITEM_COUNT) {
			meminfo_item(*mi, item_id) = strtoul(p, &p, 10);
			i--;
		}

		while (*p++ != '\n');
	}
}

static unsigned int calc_mem_per(const struct meminfo *mi)
{
	const unsigned long mem_total = meminfo_item(*mi, MI_MemTotal);
	/* free(1): used memory is calculated as total - free - buffers - cache. */
	const unsigned long mem_used  = mem_total - meminfo_item(*mi, MI_MemFree)
		- meminfo_item(*mi, MI_Buffers) - meminfo_item(*mi, MI_Cached);
	return  (unsigned int)((double)mem_used / (double)mem_total * 100);
}

static const char *const icon_ramp_arr[] = MOD_MEMORY_ICON_RAMP_ARR;

void mod_memory(BarModuleContext *ctx)
{
	struct meminfo mi;
	read_meminfo(&mi);

	const unsigned int mem_percent = calc_mem_per(&mi);
	const char *const mem_icon = ramp_select(
		icon_ramp_arr, array_length(icon_ramp_arr), mem_percent, 100);
	module_string(ctx, MOD_MEMORY_FORMAT);
}
