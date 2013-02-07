/**************************************************************/
/* srecsize: a program for calculating the number of bytes    */
/*           encoded in Motorola-SREC-Format.                 */
/*           It has no parameters and reads SREC lines from   */
/*           STDIN.                                           */
/**************************************************************/

// original file from avrfix: http://sourceforge.net/projects/avrfix/
// I made printf modifications for nicer formatting

#include <stdio.h>
#include <string.h>

#define MAX 200

int main(int argc, char* argv[])
{
  const char* hexstr = "0123456789ABCDEF";
  char* name = "";
  int count = 0;
  char buffer[MAX+1];
  if(argc > 1)
    name = argv[1];
#define hexdigit(c) (strchr(hexstr, (c))-hexstr)
  while(fgets(buffer, MAX, stdin) != NULL) {
    if(buffer[1] == '0' || buffer[1] == '1') {
      count += hexdigit(buffer[2])*16 + hexdigit(buffer[3]) - 3;
    }
  }
  printf("%-24s%-d\n", name, count);
  return 0;
}
