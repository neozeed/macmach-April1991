# MacMach user's .profile
PATH=/etc:/bin:/usr/ucb:/usr/bin:/usr/old:/usr/bin/X11:/usr/local/bin:/usr/games:$HOME/bin:./:/usr/vice/etc:/usr/andrew/bin
PS1="${USER}# "
export PATH PS1
umask 22
stty dec
echo -n "term: "
read TERM
export TERM
