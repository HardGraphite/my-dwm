#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "statusbar.h"
#include "util.h"

struct battery_status {
	unsigned int percentage;
	bool plugged;
	bool charging;
};

#define BAT_FILE_FULL       "/sys/class/power_supply/BAT0/energy_full"
#define BAT_FILE_NOW        "/sys/class/power_supply/BAT0/energy_now"
#define BAT_FILE_STATUS     "/sys/class/power_supply/BAT0/status"

static int battery_status(struct battery_status *bs)
{
	long long temp_ll;
	if (!(temp_ll = read_file_as_ll(BAT_FILE_FULL, 10)))
		return -1;
	assert(temp_ll > 0);
	const double bat_full = (double)temp_ll;
	if (!(temp_ll = read_file_as_ll(BAT_FILE_NOW, 10)))
		return -1;
	assert(temp_ll > 0);
	bs->percentage = (double)temp_ll * 100 / bat_full;

	char status_name[32];
	const size_t status_name_len =
		read_file(BAT_FILE_STATUS, status_name, sizeof status_name);
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

void mod_battery(BarModuleContext *ctx, BarModuleArg status_icons_def)
{
	struct battery_status bs;
	if (battery_status(&bs)) {
		module_string(ctx, NULL, "-?-");
		return;
	}

	const char *status_icon;
	if (!status_icons_def.s)
		status_icon = "";
	else if (bs.charging)
		status_icon = strchr(status_icons_def.s, 0) + 1;
	else if (bs.plugged)
		status_icon = status_icons_def.s;
	else
		status_icon = "";
	module_string(ctx, "%s%02u%%", status_icon, bs.percentage);

	unsigned int new_interval;
	if (bs.plugged || bs.percentage >= 20)
		new_interval = 90;
	else
		new_interval = 20;
	module_interval(ctx, new_interval);
}
