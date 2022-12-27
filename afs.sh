#! /bin/sh

# This script removes or restores AFS sources in the kernel
# The file list was generated from: sys/conf/files

if [ "$1" = "OFF" -o "$1" = "off" ]; then

echo "removing AFS files to afs.off"

set -x
cd sys

[ -d afs.off ] || mkdir afs.off
[ -d afs.off/afs] || mkdir afs.off/afs
[ -d afs.off/afsint ] || mkdir afs.off/afsint
[ -d afs.off/rx ] || mkdir afs.off/rx
mv afs/afs_buffer.c afs.off/afs/afs_buffer.c
mv afs/afs_cache.c afs.off/afs/afs_cache.c
mv afs/afs_call.c afs.off/afs/afs_call.c
mv afs/afs_callback.c afs.off/afs/afs_callback.c
mv afs/afs_daemons.c afs.off/afs/afs_daemons.c
mv afs/afs_dir.c afs.off/afs/afs_dir.c
mv afs/afs_gateway.c afs.off/afs/afs_gateway.c
mv afs/afs_istuff.c afs.off/afs/afs_istuff.c
mv afs/afs_lock.c afs.off/afs/afs_lock.c
mv afs/afs_osi.c afs.off/afs/afs_osi.c
mv afs/afs_osifile.c afs.off/afs/afs_osifile.c
mv afs/afs_osinet.c afs.off/afs/afs_osinet.c
mv afs/afs_physio.c afs.off/afs/afs_physio.c
mv afs/afs_pioctl.c afs.off/afs/afs_pioctl.c
mv afs/afs_resource.c afs.off/afs/afs_resource.c
mv afs/afs_vfsops.c afs.off/afs/afs_vfsops.c
mv afs/afs_vnodeops.c afs.off/afs/afs_vnodeops.c
mv afs/fcrypt.c afs.off/afs/fcrypt.c
mv afs/rxkad_client.c afs.off/afs/rxkad_client.c
mv afs/rxkad_common.c afs.off/afs/rxkad_common.c
mv afs/nfs_gateway.c afs.off/afs/nfs_gateway.c
mv afsint/Kcallback.ss.c afs.off/afsint/Kcallback.ss.c
mv afsint/Kvice.cs.c afs.off/afsint/Kvice.cs.c
mv afsint/Kvice.xdr.c afs.off/afsint/Kvice.xdr.c
mv afsint/afsaux.c afs.off/afsint/afsaux.c
mv afsint/afsvlint.cs.c afs.off/afsint/afsvlint.cs.c
mv afsint/afsvlint.xdr.c afs.off/afsint/afsvlint.xdr.c
mv rx/rx.c afs.off/rx/rx.c
mv rx/rx_bcrypt.c afs.off/rx/rx_bcrypt.c
mv rx/rx_clock.c afs.off/rx/rx_clock.c
mv rx/rx_event.c afs.off/rx/rx_event.c
mv rx/rx_globals.c afs.off/rx/rx_globals.c
mv rx/rx_kernel.c afs.off/rx/rx_kernel.c
mv rx/rx_null.c afs.off/rx/rx_null.c
mv rx/rx_vab.c afs.off/rx/rx_vab.c
mv rx/xdr_rx.c afs.off/rx/xdr_rx.c

else

echo "restoring AFS files from afs.off"

set -x
cd sys

mv afs.off/afs/afs_buffer.c afs/afs_buffer.c
mv afs.off/afs/afs_cache.c afs/afs_cache.c
mv afs.off/afs/afs_call.c afs/afs_call.c
mv afs.off/afs/afs_callback.c afs/afs_callback.c
mv afs.off/afs/afs_daemons.c afs/afs_daemons.c
mv afs.off/afs/afs_dir.c afs/afs_dir.c
mv afs.off/afs/afs_gateway.c afs/afs_gateway.c
mv afs.off/afs/afs_istuff.c afs/afs_istuff.c
mv afs.off/afs/afs_lock.c afs/afs_lock.c
mv afs.off/afs/afs_osi.c afs/afs_osi.c
mv afs.off/afs/afs_osifile.c afs/afs_osifile.c
mv afs.off/afs/afs_osinet.c afs/afs_osinet.c
mv afs.off/afs/afs_physio.c afs/afs_physio.c
mv afs.off/afs/afs_pioctl.c afs/afs_pioctl.c
mv afs.off/afs/afs_resource.c afs/afs_resource.c
mv afs.off/afs/afs_vfsops.c afs/afs_vfsops.c
mv afs.off/afs/afs_vnodeops.c afs/afs_vnodeops.c
mv afs.off/afs/fcrypt.c afs/fcrypt.c
mv afs.off/afs/rxkad_client.c afs/rxkad_client.c
mv afs.off/afs/rxkad_common.c afs/rxkad_common.c
mv afs.off/afs/nfs_gateway.c afs/nfs_gateway.c
mv afs.off/afsint/Kcallback.ss.c afsint/Kcallback.ss.c
mv afs.off/afsint/Kvice.cs.c afsint/Kvice.cs.c
mv afs.off/afsint/Kvice.xdr.c afsint/Kvice.xdr.c
mv afs.off/afsint/afsaux.c afsint/afsaux.c
mv afs.off/afsint/afsvlint.cs.c afsint/afsvlint.cs.c
mv afs.off/afsint/afsvlint.xdr.c afsint/afsvlint.xdr.c
mv afs.off/rx/rx.c rx/rx.c
mv afs.off/rx/rx_bcrypt.c rx/rx_bcrypt.c
mv afs.off/rx/rx_clock.c rx/rx_clock.c
mv afs.off/rx/rx_event.c rx/rx_event.c
mv afs.off/rx/rx_globals.c rx/rx_globals.c
mv afs.off/rx/rx_kernel.c rx/rx_kernel.c
mv afs.off/rx/rx_null.c rx/rx_null.c
mv afs.off/rx/rx_vab.c rx/rx_vab.c
mv afs.off/rx/xdr_rx.c rx/xdr_rx.c

rm -rf afs.off

fi
