
#include <sys/ioctl.h>
#include "macmach.h"

#define	CLUTSIZE    256
static ColorSpec CLUT[CLUTSIZE];    /* a software colormap used to map onto the hardware colormap */

#define NGC_COLOR(x) (x>>8)	/* only the top 8 bits are significant */

/*
  *	This initialises the Apple TFB after a reset
  */

int
macmachVideoSetInit(index)
int index; /* index of video monitor in the macmachFb array */
/* NOTE: This sets things to 1bit/pixel mode!! -- Sohan */
{
    VDPgInfoPtr vp;

    vp = &(macmachFbs[index].vpinfo[0]);
    if ( ioctl(macmachFbs[index].fd, VIDEO_CTRL_Init, vp ) < 0 )
    {   
	FatalError("could not initialize video monitor\n");
	(void) close (macmachFbs[index].fd); return(!Success);
    }
    else return(Success);
}




/*
 * Zap a colormap into the hardware ColorLookUpTable
 */
int
macmachVideoSetCLUT(index, start, count, pdefs)
int index;		/* index of video monitor in the macmachFb array */
int  start;		/* which entry in colormap to start with */
register int count;	/* number of entries in the colormap */
register xColorItem *pdefs;	/* list of colors to be stored in the hardware CLUT */
{
    VDEntryRecord ve;
    ColorSpec col;
    register ColorSpec *colp;
    register xColorItem *xcp;
    int i;

    /* copy needed values into ColorSpecs array */
    xcp = pdefs;
    for ( i = start; i < start+count; i++)
    {
	col.value = (unsigned short)(xcp->pixel);
	col.rgb.red = (unsigned short)(xcp->red);
	col.rgb.green = (unsigned short)(xcp->green);
	col.rgb.blue = (unsigned short)(xcp->blue);
	CLUT[col.value].rgb.red = col.rgb.red;
	CLUT[col.value].rgb.green = col.rgb.green;
	CLUT[col.value].rgb.blue = col.rgb.blue;
	xcp++;
    }
    /* zap it ... */
    ve.csTable = CLUT ; ve.csStart = 0; ve.csCount = CLUTSIZE - 1;
    if ( ioctl(macmachFbs[index].fd, VIDEO_CTRL_SetEntries, &ve) < 0 )
    {
	FatalError("Could not update colormap, ioctl VIDEO_CTRL_SetEntries failed!\n");
	return(!Success);
    }
    return(Success);
}

macmachVideoSetGray(fbNum, gray)
    int fbNum;
    Bool gray;
{
    VDPageInfo	vpi;

    vpi.csMode = gray? 1: 0;
    ioctl(macmachFbs[fbNum].fd, VIDEO_CTRL_SetGray, &vpi);
}


/*
 * macmachVideoSetPixelSize(index, pixelsize)
 *
 *	Set the screen mode to one of {1,2,4,8} bits per pixel.
 *
 *	returns:	zero if either arg is invalid.
 *               $$NOTE: Fix the return codes etc on this -- Sohan
 *
 *	Caveat: This routine work with the TFB-based card, and 8 bits per
 *		pixel works only when 512K RAM is installed.
 */
int
macmachVideoSetPixelSize(fbNum, pixelsize)
     int fbNum;		/* index of video monitor in the macmachFb array */
     int pixelsize;	/* number of bits per pixel to set card to */
{
  register int i;
  register unsigned char *cp;
  VDPgInfoPtr vp;
  VPBlockPtr v;

  switch (pixelsize)
  {
      case 1:
	  macmachFbs[fbNum].mode = 0;
	  macmachFbs[fbNum].vpinfo[0].csMode = oneBitMode;
	  break;
      case 2:
	  macmachFbs[fbNum].mode = 1;
	  macmachFbs[fbNum].vpinfo[1].csMode = twoBitMode;
	  break;
      case 4:
	  macmachFbs[fbNum].mode = 2;
	  macmachFbs[fbNum].vpinfo[2].csMode = fourBitMode;
	  break;
      case 8:
	  macmachFbs[fbNum].mode = 3;
	  macmachFbs[fbNum].vpinfo[3].csMode = eightBitMode;
	  break;
      default:
	  return !Success;
	  break;
  }

  vp = &(macmachFbs[fbNum].vpinfo[macmachFbs[fbNum].mode]);
  if ( ioctl(macmachFbs[fbNum].fd, VIDEO_STAT_GetMode, vp) < 0 )
  {   
      FatalError("could not get mode info for video monitor, ioctl VIDEO_STAT_GetMode failed!\n");
      (void) close (macmachFbs[fbNum].fd); return(!Success);
  }
 switch (pixelsize)
  {
      case 1:
	  macmachFbs[fbNum].mode = 0;
	  macmachFbs[fbNum].vpinfo[0].csMode = oneBitMode;
	  break;
      case 2:
	  macmachFbs[fbNum].mode = 1;
	  macmachFbs[fbNum].vpinfo[1].csMode = twoBitMode;
	  break;
      case 4:
	  macmachFbs[fbNum].mode = 2;
	  macmachFbs[fbNum].vpinfo[2].csMode = fourBitMode;
	  break;
      case 8:
	  macmachFbs[fbNum].mode = 3;
	  macmachFbs[fbNum].vpinfo[3].csMode = eightBitMode;
	  break;
      default:
	  return !Success;
	  break;
  }
  if ( ioctl(macmachFbs[fbNum].fd, VIDEO_CTRL_SetMode, vp ) < 0 )
  {   
      FatalError("could not set requested depth %d for video monitor, ioctl VIDEO_CTRL_SetMode failed!\n", pixelsize);
      (void) close (macmachFbs[fbNum].fd); return(!Success);
  }
  v = &(macmachFbs[fbNum].info[macmachFbs[fbNum].mode]);
  if (ioctl(macmachFbs[fbNum].fd, VIDEO_PARAMS, v) < 0 )
  {
      FatalError("could not obtain characteristics (ioctl VIDEO_PARAMS    failed!)\n");
      (void) close (macmachFbs[fbNum].fd); return(!Success);
  }
/*
  if ( macmachFbs[fbNum].info[macmachFbs[fbNum].mode].vpPixelSize != 1 )
  {
      ioctl(macmachFbs[fbNum].fd, VIDEO_CTRL_GrayScreen, vp);
      {FILE *f; f = fopen("/usr/user/serv.out", "a"); fprintf(f, "successfully ioctl'd GrayScreen\n"); fsync(f); fclose(f); fflush(f); sync();}
  }
  */
  return Success;
}
