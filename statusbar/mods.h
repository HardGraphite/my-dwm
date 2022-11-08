#ifndef MODULES_H
#define MODULES_H

/* Show date and time. The interval will be automatically controlled. */
void mod_clock(BarModuleContext *ctx, BarModuleArg strftime_format);

/* Show battery status. The param `status_icons` is a UTF-8 string in which
there are icons for power plugged and charging, and the delimiter is "\0".
The interval will be automatically controlled. */
void mod_battery(BarModuleContext *ctx, BarModuleArg status_icons);

#endif /* MODULES_H */
