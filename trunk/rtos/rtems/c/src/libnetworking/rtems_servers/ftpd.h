/*
 *  FTP Server Information
 *
 *  $Id: ftpd.h,v 1.2 2001-09-27 12:02:00 chris Exp $
 */

#ifndef __FTPD_H__
#define __FTPD_H__


#define FTPD_CONTROL_PORT   21

typedef int (*rtems_ftpd_hookfunction)(unsigned char *, unsigned long);

struct rtems_ftpd_hook
{
   char                    *filename;
   rtems_ftpd_hookfunction hook_function;
};

struct rtems_ftpd_configuration
{
   rtems_task_priority     priority;           /* FTPD task priority  */
   unsigned long           max_hook_filesize;  /* Maximum buffersize  */
                                               /*    for hooks        */
   int                     port;               /* Well-known port     */
   struct rtems_ftpd_hook  *hooks;             /* List of hooks       */
};

/*
 * Reply codes.
 */
#define PRELIM          1       /* positive preliminary */
#define COMPLETE        2       /* positive completion */
#define CONTINUE        3       /* positive intermediate */
#define TRANSIENT       4       /* transient negative completion */
#define ERROR           5       /* permanent negative completion */

int rtems_initialize_ftpd();

#endif  /* __FTPD_H__ */

