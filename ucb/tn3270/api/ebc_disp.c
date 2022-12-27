/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 */

#ifndef lint
static char sccsid[] = "@(#)ebc_disp.c	3.3 (Berkeley) 3/28/88";
#endif /* not lint */

/*
 * Translate table to map EBCDIC into 3270 display codes.
 */

unsigned char ebc_disp[256] = {
/*00*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*08*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*10*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*18*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*20*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*28*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*30*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*38*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
/*40*/	0x10,	0x0a,	0x0b,	0x1c,	0x1d,	0x1e,	0x1f,	0x2a,
/*48*/	0x2b,	0x37,	0x1b,	0x32,	0x09,	0x0d,	0x35,	0x16,
/*50*/	0x30,	0x38,	0x39,	0x3a,	0x3c,	0x3e,	0x3f,	0x40,
/*58*/	0x41,	0x42,	0x19,	0x1a,	0xbf,	0x0c,	0xbe,	0x36,
/*60*/	0x31,	0x14,	0x43,	0x44,	0x45,	0x46,	0x47,	0x48,
/*68*/	0x49,	0x4a,	0x17,	0x33,	0x2e,	0x2f,	0x08,	0x18,
/*70*/	0x4b,	0x4c,	0x4d,	0x4e,	0x4f,	0x50,	0x51,	0x52,
/*78*/	0x53,	0x3d,	0x34,	0x2c,	0x2d,	0x12,	0x11,	0x13,
/*80*/	0x54,	0x80,	0x81,	0x82,	0x83,	0x84,	0x85,	0x86,
/*88*/	0x87,	0x88,	0x55,	0x56,	0x57,	0x58,	0x59,	0x5a,
/*90*/	0x5b,	0x89,	0x8a,	0x8b,	0x8c,	0x8d,	0x8e,	0x8f,
/*98*/	0x90,	0x91,	0x5c,	0x5d,	0x5e,	0x5f,	0x60,	0x61,
/*A0*/	0x62,	0x3b,	0x92,	0x93,	0x94,	0x95,	0x96,	0x97,
/*A8*/	0x98,	0x99,	0x63,	0x64,	0x65,	0x66,	0x67,	0x68,
/*B0*/	0x69,	0x6a,	0x6b,	0x6c,	0x6d,	0x6e,	0x6f,	0x70,
/*B8*/	0x71,	0x72,	0x73,	0x74,	0x75,	0x76,	0x77,	0x78,
/*C0*/	0x0f,	0xa0,	0xa1,	0xa2,	0xa3,	0xa4,	0xa5,	0xa6,
/*C8*/	0xa7,	0xa8,	0x79,	0x7a,	0x7b,	0x7c,	0x01,	0x02,
/*D0*/	0x0e,	0xa9,	0xaa,	0xab,	0xac,	0xad,	0xae,	0xaf,
/*D8*/	0xb0,	0xb1,	0x7d,	0x7e,	0x7f,	0x03,	0x04,	0x05,
/*E0*/	0x15,	0x9a,	0xb2,	0xb3,	0xb4,	0xb5,	0xb6,	0xb7,
/*E8*/	0xb8,	0xb9,	0x9b,	0x9c,	0x9d,	0x06,	0x07,	0x9e,
/*F0*/	0x20,	0x21,	0x22,	0x23,	0x24,	0x25,	0x26,	0x27,
/*F8*/	0x28,	0x29,	0xba,	0xbb,	0xbc,	0xbd,	0x9f,	0x00,
};

/*
 * Translate table to map 3270 display codes to EBCDIC.
 */

unsigned char disp_ebc[192] = {
/*00*/	0x00,	0xce,	0xcf,	0xdd,	0xde,	0xdf,	0xed,	0xee,
/*08*/	0x6e,	0x4c,	0x41,	0x42,	0x5d,	0x4d,	0xd0,	0xc0,
/*10*/	0x40,	0x7e,	0x7d,	0x7f,	0x61,	0xe0,	0x4f,	0x6a,
/*18*/	0x6f,	0x5a,	0x5b,	0x4a,	0x43,	0x44,	0x45,	0x46,
/*20*/	0xf0,	0xf1,	0xf2,	0xf3,	0xf4,	0xf5,	0xf6,	0xf7,
/*28*/	0xf8,	0xf9,	0x47,	0x48,	0x7b,	0x7c,	0x6c,	0x6d,
/*30*/	0x50,	0x60,	0x4b,	0x6b,	0x7a,	0x4e,	0x5f,	0x49,
/*38*/	0x51,	0x52,	0x53,	0xa1,	0x54,	0x79,	0x55,	0x56,
/*40*/	0x57,	0x58,	0x59,	0x62,	0x63,	0x64,	0x65,	0x66,
/*48*/	0x67,	0x68,	0x69,	0x70,	0x71,	0x72,	0x73,	0x74,
/*50*/	0x75,	0x76,	0x77,	0x78,	0x80,	0x8a,	0x8b,	0x8c,
/*58*/	0x8d,	0x8e,	0x8f,	0x90,	0x9a,	0x9b,	0x9c,	0x9d,
/*60*/	0x9e,	0x9f,	0xa0,	0xaa,	0xab,	0xac,	0xad,	0xae,
/*68*/	0xaf,	0xb0,	0xb1,	0xb2,	0xb3,	0xb4,	0xb5,	0xb6,
/*70*/	0xb7,	0xb8,	0xb9,	0xba,	0xbb,	0xbc,	0xbd,	0xbe,
/*78*/	0xbf,	0xca,	0xcb,	0xcc,	0xcd,	0xda,	0xdb,	0xdc,
/*80*/	0x81,	0x82,	0x83,	0x84,	0x85,	0x86,	0x87,	0x88,
/*88*/	0x89,	0x91,	0x92,	0x93,	0x94,	0x95,	0x96,	0x97,
/*90*/	0x98,	0x99,	0xa2,	0xa3,	0xa4,	0xa5,	0xa6,	0xa7,
/*98*/	0xa8,	0xa9,	0xe1,	0xea,	0xeb,	0xec,	0xef,	0xfe,
/*A0*/	0xc1,	0xc2,	0xc3,	0xc4,	0xc5,	0xc6,	0xc7,	0xc8,
/*A8*/	0xc9,	0xd1,	0xd2,	0xd3,	0xd4,	0xd5,	0xd6,	0xd7,
/*B0*/	0xd8,	0xd9,	0xe2,	0xe3,	0xe4,	0xe5,	0xe6,	0xe7,
/*B8*/	0xe8,	0xe9,	0xfa,	0xfb,	0xfc,	0xfd,	0x5e,	0x5c,
};
