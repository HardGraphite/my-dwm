#include <assert.h>
#include <limits.h>

#include "config.h"
#include "statusbar.h"
#include "util.h"

static unsigned int get_temperature(void)
{
	const long long val = read_file_as_ll(MOD_THERMAL_FILE_TEMP, 10);
	assert(val >= 0 && val <= UINT_MAX);
	return MOD_THERMAL_TEMP_CONV(((unsigned int)val));
}

static const char *const icon_ramp_arr[] = MOD_THERMAL_ICON_RAMP_ARR;

void mod_thermal(BarModuleContext *ctx)
{
	static unsigned int last_temp = 0;

	const unsigned int temperature = get_temperature();
	const char *const thermal_icon =
		ramp_select(icon_ramp_arr, array_length(icon_ramp_arr), temperature, 80);
	module_string(ctx, MOD_THERMAL_FORMAT);

	const unsigned int temp_diff =
		last_temp < temperature ? temperature - last_temp : last_temp - temperature;
	module_interval(ctx, temp_diff < 3 ? 60 : 10);
}
