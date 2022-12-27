/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2emul/macvia.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * macintosh VIA emulator
 */

#include <sys/param.h>

#include <mac2/pcb.h>

#include <mac2dev/via.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macvia.h>

#define ipl_lock(xxx) \
{				\
    register s = splhigh();	\
				\
    { xxx; }			\
    splx(s);			\
}

int mac_latentcy = -3;

device_via1(offset, data, size, rw)
unsigned long offset;
register unsigned long *data;
int size, rw;
{
    register struct mac_internal *mac = current_thread_pcb()->pcb_emul;
    register struct via_device *via = &mac->via1;
    register via_reg_t vp = VIA1_ADDR;
    register unsigned char bitmask, reg, dat, dir, tmp;
    
    if (rw == BUSERR_READ) {
	switch (offset >> VIA_REGISTER_SHIFT) {
	  case VIA_REG_DDRA:
	    *data = via->portA.dir;
	    return (1);
	    
	  case VIA_REG_DDRB:
	    *data = via->portB.dir;
	    return (1);
	    
	  case VIA_REG_DRB:
	    dir = via->portB.dir;
	    reg = via->portB.reg;
	    dat = via->portB.dat;
	    tmp = 0;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if (bitmask&dir)
		    tmp |= (reg & bitmask);
		else
		    tmp |= (dat & bitmask);
	    }
	    *data = tmp;
	    return (1);
	    
	  case VIA_REG_DRA:
	    dir = via->portA.dir;
	    reg = via->portA.reg;
	    dat = via->portA.dat;
	    tmp = 0;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if (bitmask&dir)
		    tmp |= (reg & bitmask);
		else
		    tmp |= (dat & bitmask);
	    }
	    *data = tmp;
	    return (1);
	    
	  case VIA_REG_SR:
	    via->IFR &= ~VIA_IF_SR;
	    mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_SR);
	    *data = 0;	/* XXX */
	    return (1);
	    
	  case VIA_REG_ACR:
	    *data = via->ACR;
	    return (1);
	    
	  case VIA_REG_PCR:
	    *data = via->PCR;
	    return (1);
	    
	  case VIA_REG_IFR:
	    ipl_lock(reg = via->IFR);
	    if (reg)
		reg |= VIA_IF_IRQ;
	    *data = reg;
	    return (1);
	    
	  case VIA_REG_IER:
	    *data = VIA_IE_SET | via->IER;
	    return (1);
	    
	  case VIA_REG_T1LL:
	    *data = vp->t1ll;
	    return (1);
	    
	  case VIA_REG_T1LH:
	    *data = vp->t1lh;
	    return (1);
	    
	  case VIA_REG_T1CL:
	    ipl_lock({
		via->IFR &= ~VIA_IF_TIMER1;
		mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_TIMER1);
		vp->ier = VIA_IE_SET | VIA_IE_TIMER1;
	    });
	    *data = vp->t1cl;
	    return (1);
	    
	  case VIA_REG_T1CH:
	    if ((via->ACR&0x40) == 0x40) {
		static unsigned char latent, latch, count;
		
		if (latent && vp->t1lh == latch)
		    *data = count + mac_latentcy*latent;
		else {
		    count = vp->t1ch;
		    latch = vp->t1lh;
		    latent = latch - count;
		    *data = count + mac_latentcy*latent;
		}
#ifdef notdef
		if (latent && vp->t1lh == latch)
		    *data = latch - mac_latentcy*latent;
		else {
		    latch = vp->t1lh;
		    latent = latch - vp->t1ch;
		    *data = latch - mac_latentcy*latent;
		}
#endif
	    }
	    else {
		static unsigned char latent, latch, count;

		if (latent && via->T1CH == latch)
		    *data = count + mac_latentcy*latent;
		else {
		    count = vp->t1ch;
		    latch = via->T1CH;
		    latent = 256 - count;
		    *data = count + mac_latentcy*latent;
		}
	    }
	    return (1);
	    
	  case VIA_REG_T2CL:
	    ipl_lock({
		via->IFR &= ~VIA_IF_TIMER2;
		mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_TIMER2);
		vp->ier = VIA_IE_SET | VIA_IE_TIMER2;
		if (size == BUSERR_SIZE_BYTE)
		    *data = vp->t2cl;
		else
		    *data = (vp->t2ch << 8) | vp->t2cl;
	    });
	    return (1);
	    
	  case VIA_REG_T2CH:
	    *data = vp->t2ch;
	    return (1);
	    
	  default:
	    return (0);
	}
    } else {
	switch (offset >> VIA_REGISTER_SHIFT) {
	  case VIA_REG_DDRA:
	    dat = *data;
#ifdef notdef
	    reg = via->portA.dir;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		}
	    }
#endif
	    via->portA.dir = dat;
	    return (1);
	    
	  case VIA_REG_DDRB:
	    dat = *data;
#ifdef notdef
	    reg = via->portB.dir;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		}
	    }
#endif
	    via->portB.dir = dat;
	    return (1);
	    
	  case VIA_REG_DRB:
	    dat = *data;
	    reg = via->portB.reg;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		  case VIA_PORT_vFDesk2:
		  case VIA_PORT_vFDesk1:
		    tmp = (dat&(VIA_PORT_vFDesk2|VIA_PORT_vFDesk1)) >> 4;
		    if (tmp == ADB_TRANS_EVEN || tmp == ADB_TRANS_ODD) {
			ipl_lock({
			    via->IFR |= VIA_IF_SR;
			    mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_SR);
			    current_thread_pcb()->pcb_ast |= TRACE_AST;
			});			    
		    }
		    else if (tmp == ADB_TRANS_CMD) {
			ipl_lock({
			    via->IFR |= VIA_IF_SR;
			    mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_SR);
			    current_thread_pcb()->pcb_ast |= TRACE_AST;
			});			    
		    }
		    mac->adb_trans_state = tmp;
		    break;
		}
	    }
	    via->portB.reg = dat;
	    return (1);
	    
	  case VIA_REG_DRA:
	    dat = *data;
#ifdef notdef
	    reg = via->portA.reg;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		}
	    }
#endif
	    via->portA.reg = dat;
	    return (1);
	    
	  case VIA_REG_SR:
	    dat = *data;
	    ipl_lock({
		if (mac->adb_trans_state == ADB_TRANS_CMD) {
		    via->IFR |= VIA_IF_SR;
		    mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_SR);
		    current_thread_pcb()->pcb_ast |= TRACE_AST;
		}
		else {
		    via->IFR &= ~VIA_IF_SR;
		    mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_SR);
		}
	    });
	    return (1);
	    
	  case VIA_REG_ACR:
	    dat = *data;
	    vp->acr = (dat&0xe0) | (vp->acr&0x1f);
	    via->ACR = dat;
	    return (1);
	    
	  case VIA_REG_PCR:
	    via->PCR = *data;
	    return (1);
	    
	  case VIA_REG_IFR:
	    dat = *data;
	    for (bitmask = 1; bitmask < VIA_IF_IRQ; bitmask <<= 1) {
		if ((dat&bitmask) == 0)
		    continue;
		switch (bitmask) {
		  case VIA_IF_TIMER1:
		    ipl_lock({
			vp->ifr = VIA_IF_TIMER1;
			via->IFR &= ~VIA_IF_TIMER1;
			mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_TIMER1);
			vp->ier = VIA_IE_SET | VIA_IE_TIMER1;
		    });
		    break;

		  case VIA_IF_TIMER2:
		    ipl_lock({
			vp->ifr = VIA_IF_TIMER2;
			via->IFR &= ~VIA_IF_TIMER2;
			mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_TIMER2);
			vp->ier = VIA_IE_SET | VIA_IE_TIMER2;
		    });
		    break;
		    
		  default:
		    ipl_lock(via->IFR &= ~bitmask);
		    break;
		}
	    }
	    return (1);
	    
	  case VIA_REG_IER:
	    dat = *data;
	    if (dat&VIA_IE_SET) {
		for (bitmask = 1; bitmask < VIA_IE_SET; bitmask <<= 1) {
		    if ((dat&bitmask) == 0)
			continue;
		    switch (bitmask) {
		      case VIA_IE_TIMER1:
			ipl_lock({
			    via->IER |= VIA_IE_TIMER1;
			    if (via->IFR&VIA_IF_TIMER1 &&
				mac->softintr[MAC_INTR_TIMER1].flags&MAC_IF_ENB) {
				mac->softintr_pend |=
				    MAC_INTR_PEND(MAC_INTR_TIMER1);
				current_thread_pcb()->pcb_ast |= TRACE_AST;
			    }
			    else
				vp->ier = VIA_IE_SET | VIA_IE_TIMER1;
			});
			break;
			
		      case VIA_IE_TIMER2:
			ipl_lock({
			    via->IER |= VIA_IE_TIMER2;
			    if (via->IFR&VIA_IF_TIMER2 &&
				mac->softintr[MAC_INTR_TIMER2].flags&MAC_IF_ENB) {
				mac->softintr_pend |=
				    MAC_INTR_PEND(MAC_INTR_TIMER2);
				current_thread_pcb()->pcb_ast |= TRACE_AST;
			    }
			    else
				vp->ier = VIA_IE_SET | VIA_IE_TIMER2;
			});
			break;
			
		      case VIA_IE_SR:
			ipl_lock({
			    via->IER |= VIA_IE_SR;
			    mac->softintr[MAC_INTR_INP].flags |= MAC_IF_ENB;
			    if (via->IFR&VIA_IF_SR &&
				mac->softintr[MAC_INTR_SR].flags&MAC_IF_ENB) {
				mac->softintr_pend |=
				    MAC_INTR_PEND(MAC_INTR_SR);
				current_thread_pcb()->pcb_ast |= TRACE_AST;
			    }
			});
			break;

#ifdef notdef			
		      case VIA_IE_CA1:
			ipl_lock({
			    if (mac->softintr[MAC_INTR_CLK].flags&MAC_IF_ENB)
				via->IER |= VIA_IE_CA1;
			});
			break;
#endif
			
		      default:
			ipl_lock(via->IER |= bitmask);
			break;
		    }
		}
	    } else {
		for (bitmask = 1; bitmask < VIA_IE_SET; bitmask <<= 1) {
		    if ((dat&bitmask) == 0)
			continue;
		    switch (bitmask) {
		      case VIA_IE_TIMER1:
			ipl_lock({
			    mac->softintr_pend &=
				~MAC_INTR_PEND(MAC_INTR_TIMER1);
			    via->IER &= ~VIA_IE_TIMER1;
			});
			break;
			
		      case VIA_IE_TIMER2:
			ipl_lock({
			    mac->softintr_pend &=
				~MAC_INTR_PEND(MAC_INTR_TIMER2);
			    via->IER &= ~VIA_IE_TIMER2;
			});
			break;
			
		      case VIA_IE_SR:
			ipl_lock({
			    mac->softintr_pend &=
				~MAC_INTR_PEND(MAC_INTR_INP);
			    via->IER &= ~VIA_IE_SR;
			});
			break;
			
		      case VIA_IE_CA1:
			ipl_lock({
			    mac->softintr_pend &=
				~MAC_INTR_PEND(MAC_INTR_CLK);
			    via->IER &= ~VIA_IE_CA1;
			});
			break;

		      default:
			ipl_lock(via->IER &= ~bitmask);
		    }
		}
	    }
	    return (1);
	    
	  case VIA_REG_T1LL:
	    vp->t1ll = *data;
	    return (1);
	    
	  case VIA_REG_T1LH:
	    vp->t1lh = *data;
	    ipl_lock({
		via->IFR &= ~VIA_IF_TIMER1;
		mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_TIMER1);
		vp->ier = VIA_IE_SET | VIA_IE_TIMER1;
	    });
	    return (1);

	  case VIA_REG_T1CL:
	    vp->t1cl = via->T1CL = *data;
	    return (1);

	  case VIA_REG_T1CH:
	    vp->t1ch = via->T1CH = *data;
	    ipl_lock({
		via->IFR &= ~VIA_IF_TIMER1;
		mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_TIMER1);
		vp->ier = VIA_IE_SET | VIA_IE_TIMER1;
	    });
	    return (1);		   

	  case VIA_REG_T2CL:
	    vp->t2cl = *data;
	    return (1);
	    
	  case VIA_REG_T2CH:
	    ipl_lock({
		if (size == BUSERR_SIZE_BYTE)
		    vp->t2ch = *data;
		else {
		    vp->t2cl = *data;
		    vp->t2ch = (*data >> 8);
		}
		via->IFR &= ~VIA_IF_TIMER2;
		mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_TIMER2);
		vp->ier = VIA_IE_SET | VIA_IE_TIMER2;
	    });
	    return (1);
	    
	  default:
	    return (0);
	}
    }
}

#if	!defined(device_via2)
device_via2(offset, data, size, rw)
unsigned long offset;
register unsigned long *data;
int size, rw;
{
    register struct mac_internal *mac = current_thread_pcb()->pcb_emul;
    register struct via_device *via = &mac->via2;
    register via_reg_t vp = VIA2_ADDR;
    register unsigned char bitmask, reg, dat, dir, tmp;
    
    if (rw == BUSERR_READ) {
	switch (offset >> VIA_REGISTER_SHIFT) {
	  case VIA_REG_DDRA:
	    *data = via->portA.dir;
	    return (1);
	    
	  case VIA_REG_DDRB:
	    *data = via->portB.dir;
	    return (1);
	    
	  case VIA_REG_DRB:
	    dir = via->portB.dir;
	    reg = via->portB.reg;
	    dat = via->portB.dat;
	    tmp = 0;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if (bitmask&dir)
		    tmp |= (reg & bitmask);
		else
		    tmp |= (dat & bitmask);
	    }
	    ipl_lock({
		via->IFR &= ~VIA_IF_CB1;
		mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_ASC);
		vp->ifr = VIA_IF_CB1;
		vp->ier = VIA_IE_SET | VIA_IE_CB1;
	    });
	    *data = tmp;
	    return (1);
	    
	  case VIA_REG_DRA:
	    dir = via->portA.dir;
	    reg = via->portA.reg;
	    ipl_lock({
		dat = via->portA.dat;
		via->portA.dat |= 0x7f;
	    });
	    tmp = 0;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if (bitmask&dir)
		    tmp |= (reg & bitmask);
		else
		    tmp |= (dat & bitmask);
	    }
	    *data = tmp;
	    return (1);
	    
	  case VIA_REG_ACR:
	    *data = via->ACR;
	    return (1);
	    
	  case VIA_REG_PCR:
	    *data = via->PCR;
	    return (1);
	    
	  case VIA_REG_IFR:
	    ipl_lock(reg = via->IFR);
	    if (reg)
		reg |= VIA_IF_IRQ;
	    *data = reg;
	    return (1);
	    
	  case VIA_REG_IER:
	    *data = VIA_IE_SET | via->IER;
	    return (1);
	    
	  case VIA_REG_T1CL:
	    *data = vp->t1cl;
	    ipl_lock(via->IFR &= ~VIA_IF_TIMER1);
	    return (1);
	    
	  case VIA_REG_T1CH:
	    *data = vp->t1ch;
	    return (1);
	    
	  case VIA_REG_T1LL:
	    *data = vp->t1ll;
	    return (1);
	    
	  case VIA_REG_T1LH:
	    *data = vp->t1lh;
	    return (1);
	    
	  default:
	    return (0);
	}
    } else {
	switch (offset >> VIA_REGISTER_SHIFT) {
	  case VIA_REG_DDRA:
	    dat = *data;
#ifdef notdef
	    reg = via->portA.dir;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		}
	    }
#endif
	    via->portA.dir = dat;
	    return (1);
	    
	  case VIA_REG_DDRB:
	    dat = *data;
	    reg = via->portB.dir;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		  case VIA_PORT_v2PowerOff:
		    if ((dat & bitmask) &&
			(via->portB.reg & bitmask) == 0)
			exit(0);
		    break;
		}
	    }
	    via->portB.dir = dat;
	    return (1);
	    
	  case VIA_REG_DRB:
	    dat = *data;
	    reg = via->portB.reg;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		  case VIA_PORT_v2PowerOff:
		    if ((dat & bitmask) == 0 &&
			(via->portB.dir & bitmask))
			exit(0);
		    break;
		}
	    }
	    ipl_lock({
		via->IFR &= ~VIA_IF_CB1;
		mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_ASC);
		vp->ifr = VIA_IF_CB1;
		vp->ier = VIA_IE_SET | VIA_IE_CB1;
	    });
	    via->portB.reg = dat;
	    return (1);
	    
	  case VIA_REG_DRA:
	    dat = *data;
#ifdef notdef
	    reg = via->portA.reg;
	    for (bitmask = 1; bitmask; bitmask <<= 1) {
		if ((reg&bitmask) == (dat&bitmask))
		    continue;
		switch (bitmask) {
		}
	    }
#endif
	    via->portA.reg = dat;
	    return (1);
	    
	  case VIA_REG_ACR:
	    via->ACR = *data;
	    return (1);
	    
	  case VIA_REG_PCR:
	    dat = *data;
	    vp->pcr = (dat&0x10) | (vp->pcr&0xef);
	    via->PCR = dat;
	    return (1);
	    
	  case VIA_REG_IFR:
	    dat = *data;
	    for (bitmask = 1; bitmask < VIA_IF_IRQ; bitmask <<= 1) {
		if ((dat&bitmask) == 0)
		    continue;
		switch (bitmask) {
		  case VIA_IF_CB1:
		    ipl_lock({
			via->IFR &= ~VIA_IF_CB1;
			mac->softintr_pend &= ~MAC_INTR_PEND(MAC_INTR_ASC);
			vp->ifr = VIA_IF_CB1;
			vp->ier = VIA_IE_SET | VIA_IE_CB1;
		    });
		    break;

		  default:
		    ipl_lock(via->IFR &= ~bitmask);
		    break;
		}
	    }
	    return (1);
	    
	  case VIA_REG_IER:
	    dat = *data;
	    if (dat&VIA_IE_SET) {
		for (bitmask = 1; bitmask < VIA_IE_SET; bitmask <<= 1) {
		    if ((dat&bitmask) == 0)
			continue;
		    switch (bitmask) {
		      case VIA_IE_CB1:
			ipl_lock({
			    via->IER |= VIA_IE_CB1;
			    if (via->IFR&VIA_IF_CB1 &&
				mac->softintr[MAC_INTR_ASC].flags&MAC_IF_ENB) {
				mac->softintr_pend |=
				    MAC_INTR_PEND(MAC_INTR_ASC);
				current_thread_pcb()->pcb_ast |= TRACE_AST;
			    }
			    else
				vp->ier = VIA_IE_SET | VIA_IE_CB1;
			});
			break;

		      default:
			ipl_lock(via->IER |= bitmask);
			break;
		    }
		}
	    } else {
		for (bitmask = 1; bitmask < VIA_IE_SET; bitmask <<= 1) {
		    if ((dat&bitmask) == 0)
			continue;
		    switch (bitmask) {
		      case VIA_IE_CB1:
			ipl_lock({
			    mac->softintr_pend  &=
				~MAC_INTR_PEND(MAC_INTR_ASC);
			    via->IER &= ~VIA_IE_CB1;
			});
			break;

		      default:
			ipl_lock(via->IER &= ~bitmask);
			break;
		    }
		}
	    }
	    return (1);
	    
	  case VIA_REG_T1CL:
	    return (1);
	    
	  case VIA_REG_T1CH:
	    ipl_lock({
		via->IFR &= ~VIA_IF_TIMER1;
		mac->softintr[MAC_INTR_CLK].flags |= MAC_IF_ENB;
	    });
	    return (1);
	    
	  case VIA_REG_T1LL:
	    return (1);
	    
	  case VIA_REG_T1LH:
	    return (1);

	  default:
	    return (0);
	}
    }
}
#endif
