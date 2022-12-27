PATH=/bin:/etc
export PATH
stty -tabs crt kill '^u' intr '^c'
exec /bin/sh /etc/rc
