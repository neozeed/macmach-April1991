/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	scc.h,v $
 * Revision 2.2  90/08/30  11:06:56  bohman
 * 	Created.
 * 	[90/08/29  12:53:09  bohman]
 * 
 */

/*
 *   Zilog Z8530 SCC Chip
 *
 *      Copyright 1985 UniSoft Corporation
 */
 
/* read register 0 bits */
#define R0RXRDY 0x01            /* receiver is ready */
#define R0ZERO  0x02            /* zero count */
#define R0TXRDY 0x04            /* transmitter is ready */
#define R0DCD   0x08            /* data carrier detect */
#define R0SYNC  0x10            /* sync */
#define R0CTS   0x20            /* clear to send */
#define R0UNDERRUN 0x40         /* transmitter underrun */
#define R0BREAK 0x80            /* break */
 
/* read register 1 bits */
#define R1ALLSENT       0x01    /* Async mode: all data cleared transmitter */
#define R1RESIDUE       0x0E    /* residue bits (see tables) */
#define R1PARERR        0x10    /* parity error */
#define R1OVRERR        0x20    /* lost characters due to overrun */
#define R1FRMERR        0x40    /* CRC or framing error */
#define R1ENDFRAME      0x80    /* SDLC: end of frame */
 
/* write register 0 - command register */
#define W0NULL          0x00    /* null code */
#define W0POINTHIGH     0x08    /* select second bank of registers */
#define W0REXT          0x10    /* reset external/status interrupt */
#define W0ABORT         0x18    /* send abort */
#define W0RXINT         0x20    /* enable interrupt on next Rx char */
#define W0RTXPND        0x28    /* reset transmitter interrupt pending */
#define W0RERROR        0x30    /* error reset */
#define W0RIUS          0x38    /* reset highest interrupt under service */
#define W0RXCRC         0x40    /* reset Rx crc checker */
#define W0TXCRC         0x80    /* reset Tx crc generator */
#define W0TXURUN        0xC0    /* reset Tx underrun/eom latch */
 
/* write register 1 - xmit/rcv interrupt and data xfer mode */
#define W1EXTIEN        0x01    /* external interrupt enable */
#define W1TXIEN         0x02    /* transmitter interrupt enable */
#define W1PSC           0x04    /* parity is a special condition */
#define W1RXDI          0x00    /* Rx int disable */
#define W1RXIFIRST      0x08    /* Rx interrupt on first char or special */
#define W1RXIALL        0x10    /* Rx interrupt on all chars or special */
#define W1RXISC         0x18    /* Rx interrupt on special condition only */
#define W1WDMARCV       0x20    /* wait/dma request on receive/transmit */
#define W1WDMARF        0x40    /* wait/dma request function */
#define W1WDMARE        0x80    /* wait/dma request enable */
 
/* write register 2 - interrupt vector */
 
/* write register 3 - receive parameters and control */
#define W3RXENABLE      0x01    /* Rx enable */
#define W3SCLI          0x02    /* sync character load inhibit */
#define W3ASM           0x04    /* address search mode (sdlc) */
#define W3RXCRC         0x08    /* Rx crc enable */
#define W3EHUNT         0x10    /* enter hunt mode */
#define W3AUTOENABLES   0x20    /* auto enables */
#define W35BIT          0x00    /* 5 bit data */
#define W36BIT          0x80    /* 6 bit data */
#define W37BIT          0x40    /* 7 bit data */
#define W38BIT          0xC0    /* 8 bit data */
 
/* write register 4 - xmit/rcv misc parameters and modes */
#define W4PARENABLE     0x01    /* parity enable */
#define W4PAREVEN       0x02    /* even parity */
#define W4ENSYNC        0x00    /* enable sync modes */
#define W41STOP         0x04    /* 1 stop bit */
#define W415STOP        0x08    /* 1.5 stop bits */
#define W42STOP         0x0C    /* 2 stop bits */
#define W48SYNC         0x00    /* 8 bit sync character */
#define W416SYNC        0x10    /* 16 bit sync character */
#define W4SDLC          0x20    /* sdlc mode (01111110 flag) */
#define W4EXTSYNC       0x30    /* external sync mode */
#define W4CLK1          0x00    /* X 1 clock mode */
#define W4CLK16         0x40    /* X 16 clock mode */
#define W4CLK32         0x80    /* X 32 clock mode */
#define W4CLK64         0xC0    /* X 64 clock mode */
 
/* write register 5 - transmit parameter and control */
#define W5TXCRC         0x01    /* Tx crc enable */
#define W5RTS           0x02    /* rts */
#define W5CRC16         0x04    /* crc-16/sdlc */
#define W5TXENABLE      0x08    /* transmitter enable */
#define W5BREAK         0x10    /* send break */
#define W55BIT          0x00    /* 5 or less bit data */
#define W56BIT          0x40    /* 6 bit data */
#define W57BIT          0x20    /* 7 bit data */
#define W58BIT          0x60    /* 8 bit data */
#define W5DTR           0x80    /* data terminal ready */
 
/* write register 6 - sync chars or sdlc address field */
 
/* write register 7 - sync chars or sdlc address flag */
 
/* write register 8 - transmit buffer */
 
/* write register 9 - master interrupt control */
#define W9VIS           0x01    /* vector includes status */
#define W9NV            0x02    /* no vector */
#define W9DLC           0x04    /* disable lower chain */
#define W9MIE           0x08    /* master interrupt enable */
#define W9HIGHSTATUS    0x10    /* status high/low */
#define W9BRESET        0x40    /* channel b reset */
#define W9ARESET        0x80    /* channel a reset */
#define W9HRESET        0xC0    /* force hardware reset */
 
/* write register 10 - misc transmitter/receiver control bits */
#define W106BITSYNC     0x01    /* 6 bit/8 bit sync */
#define W10LOOP         0x02    /* loop mode */
#define W10AUR          0x04    /* abort/flag on underrun */
#define W10MARK         0x08    /* mark/flag idle */
#define W10APOLL        0x10    /* go active on poll */
#define W10NRZ          0x00    /* nrz */
#define W10NRZI         0x20    /* nrzi */
#define W10FM1          0x40    /* fm1 */
#define W10FM0          0x60    /* fm0 */
#define W10CRCPRESET    0x80    /* crc preset 1/0 */
 
/* write register 11 - clock control mode */
#define W11OXTAL        0x00    /* crystal output */
#define W11OXCLK        0x01    /* transmit clock */
#define W11OBR          0x02    /* baud rate generator output */
#define W11ODPLL        0x03    /* dpll output */
#define W11TRxC         0x04    /* TRxC 0/1 */
#define W11TRTxC        0x00    /* transmit clock RTxC pin */
#define W11TTRxC        0x08    /* transmit clock TRxC pin */
#define W11TBR          0x10    /* transmit clock baud rate generator */
#define W11TDPLL        0x18    /* transmit clock dpll output */
#define W11RRTxC        0x00    /* receive clock RTxC pin */
#define W11RTRxC        0x20    /* receive clock TRxC pin */
#define W11RBR          0x40    /* receive clock baud rate generator */
#define W11RDPLL        0x60    /* receive clock dpll output */
#define W11XTAL         0x80    /* crystal */
 
/* write register 12 - lower byte of baud rate generator */
 
/* write register 13 - upper byte of baud rate generator */
 
/* write register 14 - misc control bits */
#define W14BRGE         0x01    /* baud rate generator enable */
#define W14BRINT        0x02    /* baud rate generator internal */
#define W14DTRREQ       0x04    /* dtr/req function */
#define W14AUTOECHO     0x08    /* auto echo */
#define W14LOCALLB      0x10    /* local loop back */
#define W14NULL         0x00    /* null command */
#define W14ESM          0x20    /* enter search mode */
#define W14RMC          0x40    /* reset missing clock */
#define W14DDPLL        0x60    /* disable dpll */
#define W14SBRGEN       0x80    /* set source baud rate generator */
#define W14SRTxC        0xA0    /* set source RTxC */
#define W14SFM          0xC0    /* set fm mode */
#define W14SNRZI        0xE0    /* set nrzi mode */
 
/*
 * Device registers.
 */

#define scc_reg(devp, chan)	((scc_reg_t)&(devp->chan_##chan))

typedef volatile struct {
    unsigned char	csr,	csrx[3];
    unsigned char	data,	datax[3];
} *scc_reg_t;

typedef volatile struct {
    unsigned char	chan_b,	chan_bx;
    unsigned char	chan_a,	chan_ax;
} *scc_dev_t;

extern scc_dev_t	sccaddr;
