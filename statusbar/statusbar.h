#ifndef STATUSBAR_H
#define STATUSBAR_H

typedef struct BarModuleContext BarModuleContext;

/* Set module update interval. */
void module_interval(BarModuleContext *ctx, unsigned int n_sec);
/* Format and assign module string. If `fmt` is NULL, take first arg in va_list as a string. */
void module_string(BarModuleContext *ctx, const char *fmt, ...);

#endif /* STATUSBAR_H */
