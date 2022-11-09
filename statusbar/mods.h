#ifndef MODULES_H
#define MODULES_H

/* Show battery status. The interval will be automatically controlled. */
void mod_battery(BarModuleContext *ctx);

/* Show date and time. The interval will be automatically controlled. */
void mod_clock(BarModuleContext *ctx);

/* Show RAM and CPU load. */
void mod_sysinfo(BarModuleContext *ctx);

/* Show temperature. The interval will be automatically controlled. */
void mod_thermal(BarModuleContext *ctx);

#endif /* MODULES_H */
