all:
	make -C dwm

clean:
	make -C dwm clean

install:
	make -C dwm install

uninstall:
	make -C dwm uninstall

.PHONY: all dwm clean install uninstall
