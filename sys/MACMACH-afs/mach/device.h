#ifndef	_device
#define	_device

/* Module device */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_device
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include <mach/std_types.h>
#include <sys/types.h>
#include <mach/device_types.h>

/* SimpleRoutine block_write */
mig_external kern_return_t block_write
#if	defined(LINTLIBRARY)
    (port, device, block_number, block_size, block)
	port_t port;
	dev_t device;
	daddr_t block_number;
	int block_size;
	io_buf_t block;
{ return block_write(port, device, block_number, block_size, block); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	int block_size,
	io_buf_t block
);
#else
    ();
#endif
#endif

/* SimpleRoutine block_write_async */
mig_external kern_return_t block_write_async
#if	defined(LINTLIBRARY)
    (port, device, block_number, block_size, block)
	port_t port;
	dev_t device;
	daddr_t block_number;
	int block_size;
	io_buf_t block;
{ return block_write_async(port, device, block_number, block_size, block); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	int block_size,
	io_buf_t block
);
#else
    ();
#endif
#endif

/* Routine block_read */
mig_external kern_return_t block_read
#if	defined(LINTLIBRARY)
    (port, device, block_number, block_size, block, blockCnt)
	port_t port;
	dev_t device;
	daddr_t block_number;
	int block_size;
	io_buf_ptr_t *block;
	unsigned int *blockCnt;
{ return block_read(port, device, block_number, block_size, block, blockCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	int block_size,
	io_buf_ptr_t *block,
	unsigned int *blockCnt
);
#else
    ();
#endif
#endif

/* Routine block_read_ahead */
mig_external kern_return_t block_read_ahead
#if	defined(LINTLIBRARY)
    (port, device, block_number, ra_block_number, block_size, block, blockCnt)
	port_t port;
	dev_t device;
	daddr_t block_number;
	daddr_t ra_block_number;
	int block_size;
	io_buf_ptr_t *block;
	unsigned int *blockCnt;
{ return block_read_ahead(port, device, block_number, ra_block_number, block_size, block, blockCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	daddr_t ra_block_number,
	int block_size,
	io_buf_ptr_t *block,
	unsigned int *blockCnt
);
#else
    ();
#endif
#endif

#endif	_device
