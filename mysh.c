#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int parserFunction(void){

  int maxLength = 1024;
  char *quitString = "exit";
  char *tempPointer;//used for counting number of arguments
  char *path;//used for storing and checking path exists
  char *pwdString = "pwd";
  char *cdString = "cd";
  char str[maxLength +1 ];
  char str_dup[maxLength +1];
  char **argv;
  int argc;
  int i; //couter for loops;


  printf( "mysh>");
  if ( fgets( str , maxLength , stdin ) == NULL )
  {
    fprintf( stderr ,"Error!\n");
    exit(1);
  }
  
  strcpy( str_dup , str );
  
  //count number words of input
  i = 0;
  tempPointer = strtok(str_dup,"\n ");
  while( tempPointer != NULL )
  {
    i++; 
    tempPointer = strtok (NULL, "\n ");
  }
   
  //allocate memory for array of input
  argv = malloc( i * sizeof(char*) );
  //error checking
  if( argv == NULL){
    fprintf( stderr ,"Error!\n");
    exit(1);
  }

  //store String in array
  i = 0;
  argv[i] = strtok ( str, "\n " );
  //check if nothing 
  if(argv[i] == NULL){
      free( argv );
      return;//nothing in command line
  }

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
  //Code to print out current directory when pwd is inserted
  if( strcmp( argv[0], pwdString) == 0){ 
    char currentDir[maxLength]; 
    if(getcwd(currentDir, sizeof(currentDir))!= NULL){
      printf("%s\n", currentDir); 
    }
    else
    {
      perror("getcwd() errror");
    }
  }
  //check for cd
  if( strcmp( argv[0], cdString) == 0)
  {
    if( argc == 1 )
    {
      path =  getenv("HOME");
      if( path == NULL ||  chdir(path ) == -1)
      {
        fprintf(stderr, "Error!" );
      }
    }
    else
    {
      path = argv[1];
      if( path == NULL ||  chdir(path ) == -1)
      {
        fprintf(stderr, "Error!" );
      }
    } 
  }

    //free memory of last input
  free( argv);
}

int
main(void)
{
  while(1)
  {
    parserFunction(); 
  }
  exit(0);
}
