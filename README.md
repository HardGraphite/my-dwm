# My dwm

This is my build of [dwm](https://dwm.suckless.org/) and other tools
with which I can set up a usable GUI environment on Linux.

## dwm

A fork of official dwm (https://git.suckless.org/dwm), with modifications.

Script file "`~/.dwm/autostart_blocking.sh`" and "`~/.dwm/autostart.sh`"
will be executed when starting up.

### Applied patches

- [autostart](https://dwm.suckless.org/patches/autostart/)
(dwm-autostart-20210120-cb3f58a.diff),
make dwm run "`~/.dwm/autostart_blocking.sh`" and "`~/.dwm/autostart.sh &`"
before entering the handler loop.
- [systray](https://dwm.suckless.org/patches/systray/)
(dwm-systray-6.3.diff), a simple system tray implementation.

## status bar

A status bar for dwm.

### Available modules

- **battery**: show battery percentage; send notification when battery is low
- **clock**: show date and time
- **cpu**: show CPU load
- **memory**: show how much memory is used
- **thermal**: show temperature

### Handled signals

- **SIGINT**, **SIGTERM**: quit
- **SIGUSR1**: force update all modules
