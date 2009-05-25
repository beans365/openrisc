/*
 *  A test support function which performs a crude version of 
 *  "cat" so you can look at specific parts of a file.
 *
 *  $Id: test_cat.c,v 1.2 2001-09-27 12:02:25 chris Exp $
 */

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include <assert.h>

/*
 *  test_cat routine
 */

unsigned char test_cat_buffer[ 1024 ];

void test_cat(
  char *file,
  int   offset_arg,
  int   length
)
{
  int            fd;
  int            status;
  int            is_printable = 0;
  int            my_length;
  int            i;
  unsigned char  c;
  int            count = 0;
  off_t          offset = (off_t)offset_arg;

  my_length = (length) ? length : sizeof( test_cat_buffer );
  assert( my_length <= sizeof( test_cat_buffer ) );

  fd = open( file, O_RDONLY );
  if ( fd == -1 ) {
    printf( "test_cat: open( %s ) failed : %s\n", file, strerror( errno ) );
    exit( 0 );
  }

  for ( ;; ) {
    status = lseek( fd, offset, SEEK_SET );
    assert( status != -1 );

    status = read( fd, test_cat_buffer, sizeof(test_cat_buffer) );
    if ( status <= 0 ) {
      if (!is_printable)
        printf( "(%d)", count );
      puts( "" );
      break;
    }

    for ( i=0 ; i<status ; i++ ) {
      c = test_cat_buffer[i];
      if (isprint(c) || isspace(c)) {
        if (!is_printable) {
          printf( "(%d)", count );
          count = 0;
          is_printable = 1;
        }
        putchar(c);
      } else {
        is_printable = 0;
        count++;
      }
    }
    offset += status;
  }

  status = close( fd );
  assert( !status );
}
