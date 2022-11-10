#include "statusbar.h"

#include <signal.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <X11/Xlib.h>

#include "config.h"
#include "mods.h"
#include "util.h"

/* Context of a status bar module. */
struct BarModuleContext {
	unsigned int total_ticks;
	unsigned int rest_ticks;
	char string[MOD_MAXLEN];
};

/* Definition of a status bar module. */
typedef struct BarModuleDef {
	void (*update)(BarModuleContext *); /* Update callback function. */
	unsigned int interval; /* Update interval (in sec). */
} BarModuleDef;

void module_interval(BarModuleContext *ctx, unsigned int n_sec)
{
	if (n_sec) {
		const unsigned int n_ticks = n_sec / BAR_TICK_TIME;
		ctx->total_ticks = n_ticks ? n_ticks : 1;
	} else {
		ctx->total_ticks = 0;
	}
	ctx->rest_ticks = ctx->total_ticks;
}

void module_string(BarModuleContext *ctx, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	if (fmt) {
		const int n = vsnprintf(ctx->string, sizeof ctx->string, fmt, ap);
		if (n < 0)
			ctx->string[0] = '\0';
	} else {
		const char *const s = va_arg(ap, const char *);
		const size_t n = strlen(s) + 1;
		if (n <= sizeof ctx->string) {
			memcpy(ctx->string, s, n);
		} else {
			memcpy(ctx->string, s, sizeof ctx->string - 1);
			ctx->string[sizeof ctx->string - 1] = '\0';
		}
	}
	va_end(ap);
}

static const BarModuleDef moddefs[] = BAR_MOD_DEF_LIST;
static BarModuleContext modctx[array_length(moddefs)];
static const char modsep[] = BAR_MODE_SEP;
static char bar_buf[BAR_MAXLEN];
static size_t prev_bar_len;
static size_t prev_bar_hash;
static bool bar_force_update;
static bool bar_stop;

static void bar_init(void)
{
	for (size_t i = 0; i < array_length(moddefs); i++) {
		const BarModuleDef *const def = &moddefs[i];
		BarModuleContext *const ctx = &modctx[i];
		module_interval(ctx, def->interval);
		def->update(ctx);
	}

	bar_buf[0] = '\0';
	prev_bar_len = 0;
	prev_bar_hash = str_hash(bar_buf, prev_bar_len);
	bar_force_update = false;
	bar_stop = false;
}

static void bar_update(Display *disp, Window root_win)
{
	const bool force_update = bar_force_update;
	bar_force_update = false;

	char *bar_buf_pos = bar_buf;
	char *const bar_buf_end = bar_buf + sizeof bar_buf - 1;
	for (size_t i = 0; i < array_length(modctx); i++) {
		BarModuleContext *const ctx = &modctx[i];

		if (force_update || (ctx->rest_ticks == 0 && ctx->total_ticks)) {
			moddefs[i].update(ctx);
			ctx->rest_ticks = ctx->total_ticks;
		} else {
			ctx->rest_ticks--;
		}

		if (i) {
			bar_buf_pos += str_copy(
				bar_buf_pos, bar_buf_end - bar_buf_pos, modsep, strlen(modsep));
		}

		bar_buf_pos += str_copy(
			bar_buf_pos, bar_buf_end - bar_buf_pos,
			ctx->string, strlen(ctx->string));
	}
	*bar_buf_pos = '\0';

	const size_t bar_len = bar_buf_pos - bar_buf;
	const size_t bar_hash = str_hash(bar_buf, bar_len);
	if (bar_len == prev_bar_len && bar_hash == prev_bar_hash)
		return;
	prev_bar_len = bar_len;
	prev_bar_hash = bar_hash;

	if (disp) {
		if (XStoreName(disp, root_win, bar_buf) < 0) {
			fputs("XStoreName() failed\n", stderr);
			exit(EXIT_FAILURE);
		}
		XFlush(disp);
	} else {
		puts(bar_buf);
	}
}

static void bar_wait(void)
{
	sleep(BAR_TICK_TIME);
}

static void bar_sighandler_terminate(int sig)
{
	unused_var(sig);
	bar_stop = true;
}

static void bar_sighandler_update(int sig)
{
	unused_var(sig);
	bar_force_update = true;
}

static void init(void)
{
	nice(19);
	signal(SIGINT, bar_sighandler_terminate);
	signal(SIGTERM, bar_sighandler_terminate);
	signal(SIGUSR1, bar_sighandler_update);
}

static void run(Display *disp)
{
	const Window root_win = disp ? XDefaultRootWindow(disp) : 0;

	bar_init();
	while (!bar_stop) {
		bar_update(disp, root_win);
		bar_wait();
	}
}

int main(int argc, char *argv[])
{
	Display *disp;

	if (argc == 2 && !strcmp(argv[1], "-p")) {
		disp = NULL;
	} else if (argc != 1) {
		printf("usage: %s [-p]\n", argv[0]);
		return EXIT_FAILURE;
	} else if (!(disp = XOpenDisplay(NULL))) {
		fputs("XOpenDisplay() failed\n", stderr);
		return EXIT_FAILURE;
	}

	init();
	run(disp);

	if (disp)
		XCloseDisplay(disp);
}
