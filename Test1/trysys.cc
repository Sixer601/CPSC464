#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <sys/stat.h>
#include <string.h>

int main ()
{
  freopen ("errors.txt","w",stderr);
  int i;
  printf ("Executing command ls...\n");
  i=system ("ls");
  if (i != 0)
    printf ("There were errors.\n");
  i = system ("split -l 10 -d tryexec.cc input");
  if (i != 0)
    printf ("Errors in split.\n");
  return 0;
}
