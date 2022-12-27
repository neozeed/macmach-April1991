#! /bin/sh

# /usr/src/ramdisk.sh

# This shell script makes a ramdisk image.

# See /usr/src/etc/etc.mac2/newconfig/rc.ramdisk for other ramdisk
# configuration parameters

# See /usr/src/etc/etc.mac2/newconfig/README.INSTALL for installation
# documentation

# To build a ramdisk kernel, /usr/src/sys/MACMACH-macos_emul+ramdisk/vmunix,
# do the following: cd /usr/src; make ramdiskvmunix

PWD="`pwd`"
DIR="$PWD/ramdisk_dir"
MNT="$PWD/ramdisk_mnt"
IMAGE="$PWD/ramdisk_image"

AFSD="/usr/vice/etc/afsd"
THISCELL="/usr/vice/etc/ThisCell"
CELLSERVDB="/usr/vice/etc/CellServDB"

# This is the size, in 512 byte sectors, of the ramdisk image.
# This MUST agree with "mac2ramdisk" in /etc/disktab.
SIZE=1536
BYTES=`expr $SIZE \* 512`

echo -n "Enter scratch disk SCSIID: "
read SCSIID

DEVICE=/dev/sd"$SCSIID"a
RDEVICE=/dev/rsd"$SCSIID"a

while true; do
  echo "MACMACH RAMDISK BUILDER"
  echo ""
  echo "PWD =        $PWD"
  echo "DEVICE =     $DEVICE"
  echo "IMAGE =      $IMAGE"
  echo "DIR =        $DIR"
  echo "MNT =        $MNT"
  echo "AFSD =       $AFSD"
  echo "THISCELL =   $THISCELL"
  echo "CELLSERVDB = $CELLSERVDB"
  echo ""
  df $DEVICE
  echo "WARNING: This operation will trash any file system on disk $SCSIID"
  echo "WARNING: If any file system on disk $SCSIID is currently mounted,"
  echo "         the following operation may crash the system."
  echo -n "Ok to proceed? "
  read YES
  case $YES in
    Y|y|YES|yes) break
      ;;
    N|n|NO|no) exit 0
      ;;
    *) echo "Please enter YES or NO."
      ;;
  esac
done

if [ ! -d $DIR ]; then
  echo "Creating ramdisk directory: $DIR"
  set -x
  mkdir $DIR
  cd $DIR
  mkdir mnt disk bin etc dev afs vice.etc
  chmod 755 mnt disk bin etc dev afs vice.etc
  sh $PWD/etc/etc.mac2/newconfig/MAKEDEV $DIR/dev
  [ -f $AFSD ] && install -c -s -m 555 -o root -g bin $AFSD $DIR/vice.etc/afsd
  [ -f $CELLSERVDB ] && install -c -m 444 -o root -g bin $CELLSERVDB $DIR/vice.etc/CellServDB
  [ -f $THISCELL ] && install -c -m 444 -o root -g bin $THISCELL $DIR/vice.etc/ThisCell
  echo "/afs:/usr/vice/cache:5000" > $DIR/vice.etc/cacheinfo
  ln -s /mnt/tmp tmp
  ln -s /mnt/usr usr
  chmod 755 mnt disk bin etc
  cd $PWD/bin
  install -c -m 555 -o root -g bin true.sh $DIR/bin/true
  install -c -m 555 -o root -g bin false.sh $DIR/bin/false
  cc -n -O cp.c -o cp
  install -s -m 555 -o root -g bin cp $DIR/bin
  cc -n -O ls.c -o ls
  install -s -m 555 -o root -g bin ls $DIR/bin
  cc -n -O dd.c -o dd
  install -s -m 555 -o root -g bin dd $DIR/bin
#  cc -n -O sony.c -o sony
#  install -s -m 555 -o root -g bin sony $DIR/bin
  cc -n -O pwd.c -o pwd
  install -s -m 555 -o root -g bin pwd $DIR/bin
  cc -n -O cmp.c -o cmp
  install -s -m 555 -o root -g bin cmp $DIR/bin
  cc -n -O ln.c -o ln
  install -s -m 555 -o root -g bin ln $DIR/bin
  cc -n -O rm.c -o rm
  install -s -m 555 -o root -g bin rm $DIR/bin
  cc -n -O mkdir.c -o mkdir
  install -s -m 555 -o root -g bin mkdir $DIR/bin
  cc -n -O cat.c -o cat
  install -s -m 555 -o root -g bin cat $DIR/bin
  cc -n -O sync.c -o sync
  install -s -m 555 -o root -g bin sync $DIR/bin
  cc -n -O stty.c -o stty
  install -s -m 555 -o root -g bin stty $DIR/bin
  cc -n -O echo.c -o echo
  install -s -m 555 -o root -g bin echo $DIR/bin
  cc -n -O chmod.c -o chmod
  install -s -m 555 -o root -g bin chmod $DIR/bin
  cc -n -O ed.c -o ed
  install -s -m 555 -o root -g bin ed $DIR/bin
  cc -n -O od.c -o od
  install -s -m 555 -o root -g bin od $DIR/bin
  cc -n -O df.c -o df
  install -s -m 555 -o root -g bin df $DIR/bin
  cc -n -O test.c -o test
  install -s -m 555 -o root -g bin test $DIR/bin
  rm -f $DIR/bin/[
  ln $DIR/bin/test $DIR/bin/[
  make expr.c
  cc -n -O expr.c -o expr
  install -s -m 555 -o root -g bin expr $DIR/bin
  cd $PWD/bin/sh
  make clean; make CFLAGS="-n -O -w" sh
  install -s -m 555 -o root -g bin sh $DIR/bin
  cd $PWD/ucb/ftp
  make clean; make CFLAGS="-n -O -w" ftp
  install -s -m 555 -o root -g bin ftp $DIR/bin
  cd $PWD/etc
  install -c -m 555 -o root -g bin mklost+found.sh $DIR/etc/mklost+found
  cc -n -O init.c -o init
  install -s -m 555 -o root -g bin init $DIR/etc
  cc -n -O mknod.c -o mknod
  install -s -m 555 -o root -g bin mknod $DIR/etc
  cc -n -O mount.c -o mount
  install -s -m 555 -o root -g bin mount $DIR/etc
  cc -n -O umount.c -o umount
  install -s -m 555 -o root -g bin umount $DIR/etc
  cc -n -O ifconfig.c -o ifconfig
  install -s -m 555 -o root -g bin ifconfig $DIR/etc
  cc -n -O bootp.c -o bootp
  install -s -m 555 -o root -g bin bootp $DIR/etc
  cc -n -O route.c -o route
  install -s -m 555 -o root -g bin route $DIR/etc
  cc -n -O reboot.c -o reboot
  install -s -m 555 -o root -g bin reboot $DIR/etc
  cd $PWD/etc/fsck
  make clean; make CFLAGS="-n -O" fsck
  install -s -m 555 -o root -g bin fsck $DIR/etc
  cd $PWD/etc/newfs
  make clean; make CFLAGS="-n -O" newfs
  install -s -m 555 -o root -g bin newfs $DIR/etc
  cd $PWD/etc/etc.mac2
  install -c -m 555 -o root -g bin newsys.sh $DIR/etc/newsys
  cc -n -O mac2part.c -o mac2part
  install -s -m 555 -o root -g bin mac2part $DIR/etc
  cc -n -O getpass.c -o getpass
  install -s -m 555 -o root -g bin getpass $DIR/etc
  cd $PWD/etc/etc.mac2/newconfig
  install -c -m 555 -o root -g bin rc.ramdisk $DIR/etc/rc
  install -c -m 444 -o root -g bin ramdisk.profile $DIR/.profile
  install -c -m 444 -o root -g bin services $DIR/etc
  set +x
fi

echo "Creating ramdisk file system."
disklabel -w $RDEVICE mac2ramdisk
newfs $DEVICE

echo "Mounting ramdisk file system."
[ -d $MNT ] || mkdir $MNT
mount $DEVICE $MNT || {
  echo "mount failed"
  exit 1
}

echo "Copying ramdisk files."
rm -rf $MNT/*
(cd $DIR; find . -print | cpio -pduvm $MNT) || {
  echo "cpio failed"
  umount $DEVICE
  rm -rf $MNT
  exit 1
}

echo "Creating: $IMAGE"
df $DEVICE
umount $DEVICE
rm -rf $MNT
dd if=$RDEVICE of=$IMAGE.tmp bs=$BYTES count=1 || {
  echo "dd failed"
  rm -f $IMAGE.tmp
  exit 1
}

mv $IMAGE.tmp $IMAGE
echo "Operation complete."
