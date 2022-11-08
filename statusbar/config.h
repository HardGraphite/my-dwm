/* Maximum output string length. */
#define BAR_MAXLEN 1024
/* Maximum string length of a single module. */
#define MOD_MAXLEN (BAR_MAXLEN / 4 - sizeof(unsigned int) * 2)

/* Time of a tick (seconds). */
#define TICK_TIME 5

/* List of enabled bar modules. Available modules are declared in "mods.h" */
static const BarModuleDef moddefs[] = {
	/* prefix     function           argument                 interval (sec) */
	{ "\uf242 ",  mod_battery,       {.s = "\uf1e6 \0\u26a1"},              1 },
	{ NULL,       mod_clock,         {.s = "%F %a %H:%M"},                  1 },
};

/* Seperaters beetween modules. */
static const char modsep[] = " | ";
