# paths
PREFIX = /usr/local

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# includes and libs
INCS = -I${X11INC}
LIBS = -L${X11LIB} -lX11

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L -DNDEBUG
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os -flto ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS} -flto

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
STRIP = strip
