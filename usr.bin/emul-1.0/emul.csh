#! /bin/csh -f

# This script starts up the MacMach Macintosh Emulator

# The argument is the amount of memory to give the emulator, in hex.

if (`tty` == '/dev/console') then
  macserver -l /tmp/mac_console_log -e `wh -L macpatches` $1
  echo -n ''
else
  macserver -l /tmp/mac_console_log -e `wh -L macpatches` $1
endif
