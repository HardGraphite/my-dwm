# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

all: options dwm

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

dwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	${STRIP} dwm -o ${DESTDIR}${PREFIX}/bin/dwm
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	mkdir -p ${DESTDIR}${XSESSIONDESKTOPPREFIX}
	cp -f dwm.desktop ${DESTDIR}${XSESSIONDESKTOPPREFIX}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm
	rm -f ${DESTDIR}${XSESSIONDESKTOPPREFIX}/dwm.desktop

.PHONY: all options clean install uninstall
