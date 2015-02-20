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
  char str[maxLength];
  char str_dup[maxLength];
  char **argv;
  int argc;
  int i; //couter for loops;


  printf( "mysh>  ");
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
  argc = i; 
  //allocate memory for array of input
  argv = malloc( (argc+1) * sizeof(char*) );//plus one to account for the final NULL arg
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
      return;//nothing in command line, restart shell prompt
  }

  while( argv[i] != NULL )
  {
    printf ("%d\t%s.\n",i,argv[i]);
    i++;     
    argv[i] = strtok (NULL, "\n ");
  }
  argv[i+1] = NULL;//last arg should be NULL as stated in project files
  printf( "#args:%d\n" , argc );


//******************BUILT IN FUNCTIONALITY************ //
  //check for quit string, exits if user enters "exit" as first argument, other arguments are ignored
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
