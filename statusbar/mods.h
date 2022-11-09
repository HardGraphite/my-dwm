#ifndef MODULES_H
#define MODULES_H

/* Show battery status. The interval will be automatically controlled. */
void mod_battery(BarModuleContext *ctx);

/* Show date and time. The interval will be automatically controlled. */
void mod_clock(BarModuleContext *ctx);

#endif /* MODULES_H */
