/*
 *  $Id: termios_baud2index.c,v 1.2 2001-09-27 12:01:42 chris Exp $
 */

#include <sys/termios.h>

int termios_baud_to_index(
  int termios_baud
)
{
  int baud_index;

  switch (termios_baud) {
    case B0:        baud_index =  0;  break;
    case B50:       baud_index =  1;  break;
    case B75:       baud_index =  2;  break;
    case B110:      baud_index =  3;  break;
    case B134:      baud_index =  4;  break;
    case B150:      baud_index =  5;  break;
    case B200:      baud_index =  6;  break;
    case B300:      baud_index =  7;  break;
    case B600:      baud_index =  8;  break;
    case B1200:     baud_index =  9;  break;
    case B1800:     baud_index = 10;  break;
    case B2400:     baud_index = 11;  break;
    case B4800:     baud_index = 12;  break;
    case B9600:     baud_index = 13;  break;
    case B19200:    baud_index = 14;  break;
    case B38400:    baud_index = 15;  break;
    case B57600:    baud_index = 16;  break;
    case B115200:   baud_index = 17;  break;
    case B230400:   baud_index = 18;  break;
    case B460800:   baud_index = 19;  break;
    default:        baud_index = -1;  break;
  }

  return baud_index;
}

