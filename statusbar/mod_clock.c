#include <time.h>

#include "statusbar.h"

void mod_clock(BarModuleContext *ctx, BarModuleArg strftime_format)
{
	const struct tm *const tm = localtime(&(time_t){time(NULL)});

	char buffer[64];
	if (strftime(buffer, sizeof buffer, strftime_format.s, tm))
		module_string(ctx, NULL, buffer);
	else
		module_string(ctx, NULL, asctime(tm));

	const unsigned int sec = (unsigned int)tm->tm_sec;
	module_interval(ctx, (sec < 60) ? (60 - sec + 5) : (sec - 60 + 5));
}
