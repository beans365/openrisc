/*  Hwr_init.c
 *
 *  $Id: 
 */

#include <bsp.h>

#define PPC603e_SPR_HID0        1008
#define PPC603e_SPR_HID1        1009
#define PPC603e_SPR_IBAT0U       528
#define PPC603e_SPR_IBAT0L       529
#define PPC603e_SPR_DBAT0U       536
#define PPC603e_SPR_DBAT0L       537
#define PPC603e_SPR_IBAT1U       530
#define PPC603e_SPR_IBAT1L       531
#define PPC603e_SPR_DBAT1U       538
#define PPC603e_SPR_DBAT1L       539
#define PPC603e_SPR_IBAT2U       532
#define PPC603e_SPR_IBAT2L       533
#define PPC603e_SPR_DBAT2U       540
#define PPC603e_SPR_DBAT2L       541
#define PPC603e_SPR_IBAT3U       534
#define PPC603e_SPR_IBAT3L       535
#define PPC603e_SPR_DBAT3U       542
#define PPC603e_SPR_DBAT3L       543
#define PPC603e_SPR_DMISS        976
#define PPC603e_SPR_DCMP         977
#define PPC603e_SPR_HASH1        978
#define PPC603e_SPR_HASH2        979
#define PPC603e_SPR_IMISS        980
#define PPC603e_SPR_ICMP         981
#define PPC603e_SPR_RPA          982
#define PPC603e_SPR_SDR1          25
#define PPC603e_SPR_PVR          287
#define PPC603e_SPR_DAR           19
#define PPC603e_SPR_SPRG0        272
#define PPC603e_SPR_SPRG1        273
#define PPC603e_SPR_SPRG2        274
#define PPC603e_SPR_SPRG3        275
#define PPC603e_SPR_DSISR         18
#define PPC603e_SPR_SRR0          26
#define PPC603e_SPR_SRR1          27
#define PPC603e_SPR_TBL_WRITE    284
#define PPC603e_SPR_TBU_WRITE    285
#define PPC603e_SPR_DEC           22
#define PPC603e_SPR_IABR        1010
#define PPC603e_SPR_EAR          282



#define PCI_MEM_CMD   (SCORE603E_PCI_MEM_BASE >> 16)


typedef struct {
  rtems_unsigned32  counter_1_100;
  rtems_unsigned32  counter_hours;
  rtems_unsigned32  counter_min;
  rtems_unsigned32  counter_sec;
  rtems_unsigned32  counter_month;
  rtems_unsigned32  counter_date;
  rtems_unsigned32  counter_year;
  rtems_unsigned32  counter_day_of_week;

  rtems_unsigned32  RAM_1_100;
  rtems_unsigned32  RAM_hours;
  rtems_unsigned32  RAM_month;
  rtems_unsigned32  RAM_date;
  rtems_unsigned32  RAM_year;
  rtems_unsigned32  RAM_day_of_week;

  rtems_unsigned32  interupt_status_mask;
  rtems_unsigned32  command_register;
}Harris_RTC;

void init_RTC()
{
  volatile Harris_RTC *the_RTC;
  
  the_RTC = (volatile Harris_RTC *)SCORE603E_RTC_ADDRESS;

  the_RTC->command_register = 0x0;
}

void init_PCI()
{  
#if (SCORE603E_USE_SDS) | (SCORE603E_USE_OPEN_FIRMWARE) | (SCORE603E_USE_NONE)
  rtems_unsigned32 value;

 /*
  * NOTE:  Accessing any memory location not mapped by the BAT 
  * registers will cause a TLB miss exception.   
  * Set the DBAT1 to be configured for 256M of PCI MEM 
  * at 0xC0000000 with Write-through and Guarded Attributed and 
  * read/write access allowed
  */

 /* load DBAT1U (spr538) - 256Mbytes, User, Super */
  value = SCORE603E_PCI_MEM_BASE | 0x1FFF;
  asm volatile(
    "isync;"
    "mtspr 538, %0"
    : "=r" (value)
    : "0" (value)
  );

  /* load DBAT1L (spr539) - Write-through, Guarded and Read/Write */
  value = SCORE603E_PCI_MEM_BASE | 0x0002;
  asm volatile (
      "mtspr 539, %0;"
      "isync"
      : "=r" (value)
      : "0" (value)
  );

#elif (SCORE603E_USE_DINK)
  /* DINK Monitor setsup and uses all 4 BAT registers.  */
  /* The fourth BAT register can be modified to access this area */

#if (0)
 /*
  * NOTE:  Accessing any memory location not mapped by the BAT 
  * registers will cause a TLB miss exception.   
  * Set the DBAT3 to be configured for 256M of PCI MEM 
  * at 0xC0000000 with Write-through and Guarded Attributed and 
  * read/write access allowed
  */

 /* load DBAT3U (spr542) - 256Mbytes, User, Super */
  value = SCORE603E_PCI_MEM_BASE | 0x1FFF;
  asm volatile(
    "isync;"
    "mtspr 542, %0"
    : "=r" (value)
    : "0" (value)
  );

  /* load DBAT3L (spr543) - Write-through, Guarded and Read/Write */
  value = SCORE603E_PCI_MEM_BASE | 0x0002;
  asm volatile (
      "mtspr 543, %0;"
      "isync"
      : "=r" (value)
      : "0" (value)
  );
#endif

#else
#error "SCORE603E BSPSTART.C -- what ROM monitor are you using"
#endif
}

#define PPC_Get_HID0( _value ) \
  do { \
      _value = 0;        /* to avoid warnings */ \
      asm volatile( \
          "mfspr %0, 0x3f0;"     /* get HID0 */ \
          "isync" \
          : "=r" (_value) \
          : "0" (_value) \
      ); \
  } while (0)

#define PPC_Set_HID0( _value ) \
  do { \
      asm volatile( \
          "isync;" \
          "mtspr 0x3f0, %0;"     /* load HID0 */ \
          "isync" \
          : "=r" (_value) \
          : "0" (_value) \
      ); \
  } while (0)


void instruction_cache_enable ()
{
  rtems_unsigned32 value;

  /*
   * Enable the instruction cache
   */

  PPC_Get_HID0( value );

  value |= 0x00008000;       /* Set ICE bit */

  PPC_Set_HID0( value );
}

void data_cache_enable ()
{
  rtems_unsigned32 value;

  /*
   * enable data cache 
   */

  PPC_Get_HID0( value );

  value |= 0x00004000;        /* set DCE bit */

  PPC_Set_HID0( value );
}










