#ifndef STATUSBAR_H
#define STATUSBAR_H

typedef struct BarModuleContext BarModuleContext;

/* Argument for a status bar module. */
typedef union BarModuleArg {
	int          i;
	unsigned int u;
	const char  *s;
} BarModuleArg;

/* Definition of a status bar module. */
typedef struct BarModuleDef {
	const char *prefix; /* Prefix string or NULL. */
	void (*update)(BarModuleContext *, BarModuleArg); /* Update callback function. */
	BarModuleArg argument; /* Argument which will be passed as the 2nd param of update(). */
	unsigned int interval; /* Update interval (in sec). */
} BarModuleDef;

/* Set module update interval. */
void module_interval(BarModuleContext *ctx, unsigned int n_sec);
/* Format and assign module string. If `fmt` is NULL, take first arg in va_list as a string. */
void module_string(BarModuleContext *ctx, const char *fmt, ...);

#endif /* STATUSBAR_H */
