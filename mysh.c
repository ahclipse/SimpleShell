#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(void)
{
  int maxLength = 1024;
  char *quitString = "exit";
  char *tempPointer;
  char str[maxLength +1 ];
  char str_dup[maxLength +1];
  char **argv;//TODO 100 is temporary
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
  
    strcpy( str_dup , str );
  
    //count number words of input
    i = 1;
    tempPointer = strtok(str_dup,"\n ");
    while( tempPointer != NULL )
    {
      i++; 
      tempPointer = strtok (NULL, "\n ");
    }
   
    //allocate memory for array of input
    argv = malloc( i * sizeof(char*) );

    //store String in array
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

    //free memory of last input
    free( argv);
  }
  exit(0);
}
