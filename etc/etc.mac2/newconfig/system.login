# MacMach user's .login
umask 22
stty dec
if (`tty` == '/dev/console') then
  if (`machine` == 'mac2') then
    set term = "mac2"
    echo -n "startx "
    set i = $<
    /usr/bin/X11/startx $i
  endif
else
  echo -n "term: "
  set i = $<
  if ("$i" == '') then
    set term = "ansi"
  else
    set term = "$i"
  endif
endif
unset i
unset d
