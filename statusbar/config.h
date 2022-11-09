/* Maximum output string length. */
#define BAR_MAXLEN 512
/* Maximum string length of a single module. */
#define MOD_MAXLEN (128 - sizeof(unsigned int) * 2)

/* Time of a tick (seconds). */
#define BAR_TICK_TIME 5

/* List of enabled bar modules. Available modules are declared in "mods.h" */
#define BAR_MOD_DEF_LIST { \
	/* function      interval (sec) */ \
	{ mod_battery,        /*auto*/0 }, \
	{ mod_clock,          /*auto*/0 }, \
}

/* Seperaters beetween modules. */
#define BAR_MODE_SEP " | "

#define MOD_BATTERY_FORMAT             "%s%02u%%", status_icon, percentage
#define MOD_BATTERY_ICON_PLUGGED       "\uf1e6"
#define MOD_BATTERY_ICON_CHARGING      "\uf1e6"
#define MOD_BATTERY_ICON_RAMP_ARR      {"\uf244", "\uf243", "\uf242", "\uf241", "\uf240"}
#define MOD_BATTERY_FILE_FULL          "/sys/class/power_supply/BAT0/energy_full"
#define MOD_BATTERY_FILE_NOW           "/sys/class/power_supply/BAT0/energy_now"
#define MOD_BATTERY_FILE_STATUS        "/sys/class/power_supply/BAT0/status"

#define MOD_CLOCK_FORMAT               "%s", time_str
#define MOD_CLOCK_TIME_FMT             "%F %a %H:%M"
