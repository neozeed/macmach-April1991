| Mutex implementation for mac2.

|	int
|	mutex_try_lock(m)
|		mutex_t m;	(= int *m for our purposes)

	.globl	_mutex_try_lock
_mutex_try_lock:
	movl	sp@(4),a0
	tas	a0@
	bne	1f
	moveq	#1,d0		| yes
	rts
1:
	clrl	d0		| no
	rts

|	void
|	mutex_unlock(m)
|		mutex_t m;	(= int *m for our purposes)

	.globl	_mutex_unlock
_mutex_unlock:
	movl	sp@(4),a0
	clrl	a0@
	rts
