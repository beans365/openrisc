/*-------------------------------------*/
/* prcb.c                              */
/* Last change : 11.10.94              */
/*-------------------------------------*/
/*
 *  $Id: rom_prcb.c,v 1.2 2001-09-27 11:59:59 chris Exp $
 */

#include "flttbl.h"
#include "cntrltbl.h"
#include "intrtbl.h"
#include "systbl.h"
#include "prcb.h"
/*-------------------------------------*/
  /* RAM based PRocess Control Block  
   */      
#ifdef DBPRECISE_FAULTS
#define AC                      (INT_OVFL_DISABLE | PRECISE_FLTS)
#else
#define AC                      (INT_OVFL_DISABLE | IMPRECISE_FLTS)
#endif

 /* Initial Fault Configuration Word Image.
   * As to this 1, I don't know why but without it
   * fault hanlder wouldn't be invoked.
   */
#define FAULT_CONFIG            (UNLGND_FAULT_DISABLE | 1)
extern SystemTbl nulsystemTbl;

  /* Initial Instruction Cache Configuration Word Image.
   */
#ifdef DBCACHE_OFF
#define INST_CACHE_CONFIG       (INST_CACHE_DISABLE)
#else
#define INST_CACHE_CONFIG       (INST_CACHE_ENABLE)
#endif

  /* Initial Register Cache Configuration Word Image.
   */
#define REG_CACHE_CONFIG        0x200


struct PRCB rom_prcb = {
  & faultTbl[0],                        /* Fault Table Base */   
  & rom_controlTbl[0],                  /* Control Table Base */    
  AC,                                   /* AC */ 
  FAULT_CONFIG,                         /* Fault Configuration Word */
  & interruptTbl,                       /* Interrupt Table Base */ 
  & nulsystemTbl,                       /* System Procedure Table Base */   
  0,                                    /* Reserved */  
  & intStackPtr[0],                     /* Interrupt Stack Pointer */
  INST_CACHE_CONFIG,                    /* Instruction Cache Config */ 
  REG_CACHE_CONFIG,                     /* Reg Cache Config */ 
};  

/*-------------*/
/* End of file */
/*-------------*/

