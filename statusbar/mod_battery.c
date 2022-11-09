#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "statusbar.h"
#include "util.h"

struct battery_status {
	unsigned int percentage;
	bool plugged;
	bool charging;
};

static int battery_status(struct battery_status *bs)
{
	long long temp_ll;
	if (!(temp_ll = read_file_as_ll(MOD_BATTERY_FILE_FULL, 10)))
		return -1;
	assert(temp_ll > 0);
	const double bat_full = (double)temp_ll;
	if (!(temp_ll = read_file_as_ll(MOD_BATTERY_FILE_NOW, 10)))
		return -1;
	assert(temp_ll > 0);
	bs->percentage = (double)temp_ll * 100 / bat_full;

	char status_name[32];
	const size_t status_name_len =
		read_file(MOD_BATTERY_FILE_STATUS, status_name, sizeof status_name);
	assert(status_name_len < sizeof status_name);
	if (status_name_len && status_name[status_name_len - 1] == '\n')
		status_name[status_name_len - 1] = '\0';
	else
		status_name[status_name_len] = '\0';
	if (!strcmp(status_name, "Charging"))
		bs->plugged = true, bs->charging = true;
	else if (!strcmp(status_name, "Not charging"))
		bs->plugged = true, bs->charging = false;
	else
		bs->plugged = false, bs->charging = false;

	return 0;
}

static const char icon_plugged[] = MOD_BATTERY_ICON_PLUGGED;
static const char icon_charging[] = MOD_BATTERY_ICON_CHARGING;
static const char *const icon_ramp_arr[] = MOD_BATTERY_ICON_RAMP_ARR;

void mod_battery(BarModuleContext *ctx)
{
	struct battery_status bs;
	if (battery_status(&bs)) {
		module_string(ctx, NULL, "-?-");
		return;
	}

	const unsigned int percentage = bs.percentage;
	const char *const status_icon =
		bs.charging ? icon_charging :
		bs.plugged  ? icon_plugged  :
		ramp_select(icon_ramp_arr, array_length(icon_ramp_arr), percentage);
	module_string(ctx, MOD_BATTERY_FORMAT);

	unsigned int new_interval;
	if (bs.plugged || bs.percentage >= 20)
		new_interval = 90;
	else
		new_interval = 20;
	module_interval(ctx, new_interval);
}
