/* motorola.h
 *
 *  This include file describe the data structure and the functions implemented
 *  by rtems to identify motorola boards.
 *
 *  CopyRight (C) 1999 valette@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: motorola.h,v 1.2 2001-09-27 12:01:07 chris Exp $
 */

#ifndef LIBBSP_POWERPC_SHARED_MOTOROLA_MOTOROLA_H
#define LIBBSP_POWERPC_SHARED_MOTOROLA_MOTOROLA_H

#include <bsp/residual.h>

typedef enum {
  PREP_IBM 	= 0,
  PREP_Radstone = 1,
  PREP_Motorola = 2
}prep_t;

typedef enum {
  MVME_2400 			= 0,
  GENESIS   			= 1,
  POWERSTACK_E			= 2,
  BLACKAWK			= 3,
  OMAHA				= 4,
  UTAH				= 5,
  POWERSTACK_EX			= 6,
  MESQUITE			= 7,
  SITKA				= 8,
  MESQUITE_W_HAC		= 9,
  MTX_PLUS			= 10,
  MTX_WO_PP			= 11,
  MTX_W_PP			= 12,
  MVME_2300			= 13,
  MVME_2300SC_2600		= 14,
  MVME_2600_W_MVME712M		= 15,
  MVME_2600_2700_W_MVME761	= 16,
  MVME_3600_W_MVME712M		= 17,
  MVME_3600_W_MVME761		= 18,
  MVME_1600			= 19,
  MOTOROLA_UNKNOWN		= 255
} motorolaBoard;

typedef enum {
  HOST_BRIDGE_RAVEN	= 0,
  HOST_BRIDGE_HAWK	= 1,
  HOST_BRIDGE_UNKNOWN	= 255
}motorolaHostBridge;
  
#define MOTOROLA_CPUTYPE_REG	0x800
#define MOTOROLA_BASETYPE_REG	0x803  

extern prep_t 			checkPrepBoardType(RESIDUAL *res);
extern prep_t 			currentPrepType;
extern motorolaBoard		getMotorolaBoard();
extern motorolaBoard		currentBoard;
extern const char*		motorolaBoardToString(motorolaBoard);


#endif /* LIBBSP_POWERPC_SHARED_MOTOROLA_MOTOROLA_H */

