/*-------------------------------------*/
/* rom_ibr.h                           */
/* Last change :  23. 1.95             */
/*-------------------------------------*/\
/*
 *  $Id: rom_ibr.c,v 1.2 2001-09-27 11:59:59 chris Exp $
 */

#include "rom_ibr.h"
#include "prcb.h"
#include "cntrltbl.h"
#include <i960RP.h>
#include "../include/rxgen960_config.h"
/*-------------------------------------*/
extern void romStart(void);


struct IBR rom_ibr = { 
 {((REGION_BOOT_CONFIG) & 0xff),            /* Initial Bus Configuration */
  ((REGION_BOOT_CONFIG) >> 8) & 0xff,  
  ((REGION_BOOT_CONFIG) >> 16) & 0xff,
  ((REGION_BOOT_CONFIG) >> 24) & 0xff},   
  romStart,                         /* Start Point */
  & rom_prcb,                       /* PRCB */
  {-2,                               /* CheckSum */ 
  0,
  0,
  0,
  0,
  (unsigned int) rom_ibr_cksum}
};
/*-------------*/
/* End of file */
/*-------------*/
