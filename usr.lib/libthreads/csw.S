| Context switch and cproc startup for mac2 COROUTINE implementation.

	.text

| Suspend the current thread and resume the next one.
|
|	void
|	cproc_switch(cur, next)
|		int *cur;
|		int *next;

	.globl	_cproc_switch
_cproc_switch:
	movl	sp@(4),a0	| a0 = cur
	movl	sp@(8),a1	| a1 = next
	moveml	#Lpush,sp@-	| save current registers
	movl	sp,a0@		| save current sp
	movl	a1@,sp		| restore next sp
	moveml	sp@+,#Lpop	| restore next registers
	rts			| return to next thread

|	void
|	cproc_start(parent_context, child, stackp)
|		int *parent_context;
|		cproc_t child;
|		int stackp;

	.globl	_cproc_start
_cproc_start:
	movl	sp@(4),a0	| a0 = parent
	movl	sp@(12),a1	| a1 = stackp
	movl	sp@(8),a1@-	| push child onto stackp
	moveml	#Lpush,sp@-	| save parent registers
	movl	sp,a0@		| save parent sp
	movl	a1,sp		| child sp = stackp
	jbsr	_cthread_body	| cthread_body(child)

	| Control never returns from cthread_body().


	| Register save masks for moveml instruction.
	| The pop mask is for post-increment mode.
	| The push mask is for pre-decrement mode.
	Lpop	= 0x7cfc	| A6 .. A2, D7 .. D2
	Lpush	= 0x3f3e	| D2 .. D7, A2 .. A6
