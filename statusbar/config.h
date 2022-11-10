/* Maximum output string length. */
#define BAR_MAXLEN 512
/* Maximum string length of a single module. */
#define MOD_MAXLEN (128 - sizeof(unsigned int) * 2)

/* Time of a tick (seconds). */
#define BAR_TICK_TIME 5

/* List of enabled bar modules. Available modules are declared in "mods.h" */
#define BAR_MOD_DEF_LIST { \
	/* function      interval (sec) */ \
	{ mod_memory,                20 }, \
	{ mod_cpu,                   10 }, \
	{ mod_thermal,        /*auto*/0 }, \
	{ mod_battery,        /*auto*/0 }, \
	{ mod_clock,          /*auto*/0 }, \
}

/* Seperaters beetween modules. */
#define BAR_MODE_SEP " | "

#define MOD_BATTERY_FORMAT             "%s%02u%%", status_icon, percentage
#define MOD_BATTERY_ICON_PLUGGED       "\uf1e6"
#define MOD_BATTERY_ICON_CHARGING      "\u26a1"
#define MOD_BATTERY_ICON_RAMP_ARR      {"\uf244", "\uf243", "\uf242", "\uf241", "\uf240"}
#define MOD_BATTERY_FILE_FULL          "/sys/class/power_supply/BAT0/energy_full"
#define MOD_BATTERY_FILE_NOW           "/sys/class/power_supply/BAT0/energy_now"
#define MOD_BATTERY_FILE_STATUS        "/sys/class/power_supply/BAT0/status"

#define MOD_CLOCK_FORMAT               "%s", time_str
#define MOD_CLOCK_TIME_FMT             "%F %a %H:%M"

#define MOD_CPU_FORMAT                 "%s%02u%%", cpu_icon, cpu_percent
#define MOD_CPU_ICON_RAMP_ARR          {"\u25d4", "\u25d1", "\u25d5", "\u25cf"}

#define MOD_MEMORY_FORMAT             "%s%02u%%", mem_icon, mem_percent
#define MOD_MEMORY_ICON_RAMP_ARR      {"\ue266"}

#define MOD_THERMAL_FORMAT             "%s%u\u2103", thermal_icon, temperature
#define MOD_THERMAL_ICON_RAMP_ARR      {"\uf2cb", "\uf2ca", "\uf2c9", "\uf2c8"}
#define MOD_THERMAL_FILE_TEMP          "/sys/class/thermal/thermal_zone0/temp"
#define MOD_THERMAL_TEMP_CONV(TEMP)    ((TEMP) / 1000)
