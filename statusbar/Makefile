include config.mk

SRC = statusbar.c util.c  $(wildcard mod_*.c)
OBJ = ${SRC:.c=.o}

all: options statusbar

options:
	@echo statusbar build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

statusbar: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f statusbar ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	${STRIP} statusbar -o ${DESTDIR}${PREFIX}/bin/dwm-statusbar
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm-statusbar

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm-statusbar

.PHONY: all options clean install uninstall
