#include <time.h>

#include "config.h"
#include "statusbar.h"

void mod_clock(BarModuleContext *ctx)
{
	const struct tm *const tm = localtime(&(time_t){time(NULL)});

	char time_str[64];
	if (strftime(time_str, sizeof time_str, MOD_CLOCK_TIME_FMT, tm))
		module_string(ctx, MOD_CLOCK_FORMAT);
	else
		module_string(ctx, NULL, asctime(tm));

	const unsigned int sec = (unsigned int)tm->tm_sec;
	module_interval(ctx, (sec < 60) ? (60 - sec + 5) : (sec - 60 + 5));
}
