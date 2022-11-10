#include <sys/sysinfo.h>

#include "config.h"
#include "statusbar.h"
#include "util.h"

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

static const char *const icon_ramp_arr[] = MOD_CPU_ICON_RAMP_ARR;

void mod_cpu(BarModuleContext *ctx)
{
	struct sysinfo si;
	if (sysinfo(&si) != 0) {
		module_string(ctx, NULL, "-?-");
		return;
	}

	const unsigned int cpu_percent = calc_cpu_per(&si);
	const char *const cpu_icon = ramp_select(
		icon_ramp_arr, array_length(icon_ramp_arr), cpu_percent, 100);
	module_string(ctx, MOD_CPU_FORMAT);
}
