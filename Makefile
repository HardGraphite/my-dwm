SUBDIR = dwm statusbar

define foreach_subdir
	@for __dir in ${SUBDIR}; do make -C $$__dir ${1}; done
endef

all:
	$(call foreach_subdir,)

clean:
	$(call foreach_subdir,clean)

install:
	$(call foreach_subdir,install)

uninstall:
	$(call foreach_subdir,uninstall)

.PHONY: all dwm clean install uninstall
