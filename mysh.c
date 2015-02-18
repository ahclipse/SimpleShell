#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(void)
{
  int maxLength = 1024;
  char *quitString = "exit";
  char str[maxLength +1 ];
  char *argv[100];//TODO 100 is temporary
  int argc;
  int i; //couter for loops;

  while(1)
  {
    printf( "%>");
    if ( fgets( str , maxLength , stdin ) == NULL )
    {
      fprintf( stderr ,"Error!\n");
      exit(1);
    }

    printf("%s\n", str);
    
    //TODO check for quit here?
    i = 0;
    argv[i] = strtok ( str, "\n " );
    while( argv[i] != NULL )
    {
      printf ("%d\t%s.\n",i,argv[i]);
      i++;     
      argv[i] = strtok (NULL, "\n ");
    }
    argc = i;
    printf( "#args:%d\n" , argc );
    
    if( strcmp( argv[0], quitString) == 0)
    { 
      exit(0);
    }
  }
  exit(0);
}
