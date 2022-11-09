#include <sys/sysinfo.h>

#include "config.h"
#include "statusbar.h"
#include "util.h"

static unsigned int calc_ram_per(const struct sysinfo *si)
{
	unsigned long ram_used = si->totalram - si->freeram;
	ram_used -= si->bufferram;
	return (double)ram_used / (double)si->totalram * 100.0;
}

static unsigned int calc_cpu_per(const struct sysinfo *si)
{
	static unsigned int nproc = 0;
	if (!nproc) {
		const int n = get_nprocs();
		nproc = n > 0 ? (unsigned int)n : 1;
	}
	const unsigned int res = ((si->loads[0] * 100) >> SI_LOAD_SHIFT) / nproc;
	return res <= 100 ? res : 100;
}

static const char *const icon_ram_ramp_arr[] = MOD_SYSINFO_ICON_RAM_RAMP_ARR;
static const char *const icon_cpu_ramp_arr[] = MOD_SYSINFO_ICON_CPU_RAMP_ARR;

void mod_sysinfo(BarModuleContext *ctx)
{
	struct sysinfo si;
	if (sysinfo(&si) != 0) {
		module_string(ctx, NULL, "-?-");
		return;
	}

	const unsigned int ram_percent = calc_ram_per(&si);
	const unsigned int cpu_percent = calc_cpu_per(&si);
	const char *const ram_icon = ramp_select(
		icon_ram_ramp_arr, array_length(icon_ram_ramp_arr), ram_percent, 100);
	const char *const cpu_icon = ramp_select(
		icon_cpu_ramp_arr, array_length(icon_cpu_ramp_arr), cpu_percent, 100);
	module_string(ctx, MOD_SYSINFO_FORMAT);
}
