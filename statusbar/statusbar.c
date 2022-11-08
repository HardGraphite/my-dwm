#include "statusbar.h"

#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <X11/Xlib.h>

#include "util.h"

#include "mods.h"
#include "config.h"

/* Context of a status bar module. */
struct BarModuleContext {
	unsigned int total_ticks;
	unsigned int rest_ticks;
	char string[MOD_MAXLEN];
};

void module_interval(BarModuleContext *ctx, unsigned int n_sec)
{
	if (n_sec) {
		const unsigned int n_ticks = n_sec / TICK_TIME;
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

static BarModuleContext modctx[sizeof moddefs / sizeof moddefs[0]];
static char bar_buf[BAR_MAXLEN];
static int bar_stop;

static void bar_init(void)
{
	const size_t modcnt = sizeof moddefs / sizeof moddefs[0];

	for (size_t i = 0; i < modcnt; i++) {
		const BarModuleDef *const def = &moddefs[i];
		BarModuleContext *const ctx = &modctx[i];
		module_interval(ctx, def->interval);
		def->update(ctx, def->argument);
	}

	bar_stop = 0;
}

static void bar_update(Display *disp, Window root_win)
{
	const size_t modcnt = sizeof moddefs / sizeof moddefs[0];
	char *bar_buf_pos = bar_buf;
	char *const bar_buf_end = bar_buf + sizeof bar_buf - 1;
	for (size_t i = 0; i < modcnt; i++) {
		const BarModuleDef *const def = &moddefs[i];
		BarModuleContext *const ctx = &modctx[i];

		if (ctx->rest_ticks) {
			ctx->rest_ticks--;
		} else if (ctx->total_ticks) {
			def->update(ctx, def->argument);
			ctx->rest_ticks = ctx->total_ticks;
		}

		if (i) {
			bar_buf_pos += str_copy(
				bar_buf_pos, bar_buf_end - bar_buf_pos, modsep, strlen(modsep));
		}

		if (def->prefix) {
			bar_buf_pos += str_copy(
				bar_buf_pos, bar_buf_end - bar_buf_pos,
				def->prefix, strlen(def->prefix));
		}

		bar_buf_pos += str_copy(
			bar_buf_pos, bar_buf_end - bar_buf_pos,
			ctx->string, strlen(ctx->string));
	}
	*bar_buf_pos = '\0';

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
	sleep(TICK_TIME);
}

static void bar_terminate(int sig)
{
	unused_var(sig);
	bar_stop = 1;
}

static void init(void)
{
	nice(19);
	signal(SIGINT, bar_terminate);
	signal(SIGTERM, bar_terminate);
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
