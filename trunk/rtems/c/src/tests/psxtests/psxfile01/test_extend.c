/*
 *  A test support function which extends the file to the specified 
 *  length.  This handles the implied open(), lseek(), write(), and close()
 *  operations.
 *
 *  The defined behavior is a seek() followed by a write() extends the file
 *  and zero fills the new length part.
 *
 *  $Id: test_extend.c,v 1.2 2001-09-27 12:02:25 chris Exp $
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
 *  test_extend routine
 */

void test_extend(
  char   *file,
  off_t  offset
)
{
  int   fd;
  int   status;
  char  c = 0;

  fd = open( file, O_WRONLY );
  if ( fd == -1 ) {
    printf( "test_extend: open( %s ) failed : %s\n", file, strerror( errno ) );
    exit( 0 );
  }

  status = lseek( fd, offset - 1, SEEK_SET );
  assert( status != -1 );

  status = write( fd, &c, 1 );
  if ( status == -1 ) {
    printf( "test_extend: write( %s ) failed : %s\n", file, strerror( errno ) );
    exit( 0 );
  }

  if ( status != 1 ) {
    printf( "test_extend: write( %s ) only wrote %d of %d bytes\n",
            file, status, 1 );
    exit( 0 );
  }

  status = close( fd );
  assert( !status );
}
