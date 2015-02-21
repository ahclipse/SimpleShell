#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

//includsinve start index, exclusive endIndex
int sub_array(int totalElm, int startIndex, int endIndex, char*** array)
{
  char **sub_arr;
  int copyNum;//number of elements to be compied

  if( endIndex <= startIndex || totalElm+1 < endIndex)//plus one to get final elemnt exclusivity
  {
    return -1;
  }
  
  copyNum = endIndex - startIndex ;

  //+1 for NULL at end
  if( (sub_arr = malloc( (copyNum + 1) * sizeof(char*))) == NULL)//take away null 1 so space for NULL
  {
    printf("made it here\n");
    return -1;
  }
  memcpy( sub_arr, ((char *)(*array + startIndex )), copyNum * sizeof(char*) );
  printf( "%s", sub_arr[0]);
  sub_arr[copyNum + 1] = NULL;
  free(*array);
  *array = sub_arr;
  return copyNum;
}

int execute( int argc, char **argv)
{
  int pid;
  int redirectNotFound;
  int out;
  int i;
  int status;

  if( (pid = fork()) == -1)
  {
    return -1;
  }
  else
  {
    if( pid == 0)
    {       
      //begin exec function
      i = 0;
      redirectNotFound = 1;
      while( i < argc && redirectNotFound )  
      {
        if( strcmp( argv[i] ,">>\0" ) == 0 || strcmp( argv[i] ,">\0" ) == 0 )
        {
          redirectNotFound = 0;//found redirect, assuming only one
        
          if( argc < 3 || argc != i+1+1)//one for zero indexing, one for next element to exist
          {
            return -1;// input after filename, or no filename given
          }

          //change stdout to newfile
          if(  strcmp( argv[i] ,">>" ) == 0 )
          {
	    printf(">>");
            out = open( argv[i+1], O_APPEND | O_CREAT | O_WRONLY, 0666); 
          }
          if(  strcmp( argv[i] ,">" ) == 0 )
          {
            out = open( argv[i+1], O_TRUNC | O_CREAT | O_WRONLY, 0666); 
          }

          if ( out == -1)
          {
            return -1;
          }

          if( dup2( out, 1 ) ==  -1)//1 means stdout
          {
            return -1;
          }

          close(out);
          //create sub array of args
          if( (argc = sub_array( argc, 0 , i, &argv)) == - 1) //to keep naming in rest of the code the same
          {
             fprintf(stderr, "Error!\n" );
             exit(1);//child process should close even if exec fails
          }
        }
        i++;       
      }

      if( execvp( argv[0], argv) == -1)
      { 
        fprintf(stderr, "Error!\n" );
        exit(0);//child process should close even if exec fails
      }
      exit(0);//Should never reach here
    }
    else
    {
      if( waitpid( pid, &status, 0) == -1)
      {
        free( argv );
        return -1;
      }
    }
    return 0;
  }
}

int mysh(void)
{
  int maxLength = 1024;
  char *quitString = "exit";
  char *tempPointer;//used for counting number of arguments
  char *path;//used for storing and checking path exists
  char *pwdString = "pwd";
  char *cdString = "cd";
  char str[maxLength];
  char str_dup[maxLength];
  char **argv;;
  char currentDir[maxLength]; 
  int argc;
  int i; //couter for loops;

  printf( "mysh>  ");
  if ( fgets( str , maxLength , stdin ) == NULL )
  {
   return -1;
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
    return -1;
  }

  //store String in array
  i = 0;
  argv[i] = strtok ( str, "\n " );
  //check if nothing 
  if(argv[i] == NULL){
      free( argv );
      return 0;//nothing in command line, restart shell prompt
  }
  while( argv[i] != NULL )
  {
    i++;     
    argv[i] = strtok (NULL, "\n ");
  }
  
//******************BUILT IN FUNCTIONALITY************ //
  //check for quit string, exits if user enters "exit" as first argument, other arguments are ignored
  if( strcmp( argv[0], quitString) == 0 && argc == 1)
  { 
    exit(0);
  }
  else if( strcmp( argv[0], pwdString) == 0)
  { //Code to print out current directory when pwd is inserted
   if(getcwd(currentDir, sizeof(currentDir))!= NULL){
    printf("%s\n", currentDir); 
    }
    else
    {
      return -1;
    }
  }
  //check for cd
  else if( strcmp( argv[0], cdString) == 0)
  {
    if( argc == 1 )
    {
      path =  getenv("HOME");
    }
    else
    {
      path = argv[1];
    }
    
    if( path == NULL ||  chdir(path ) == -1)
    {
      free( argv );
      return -1;
    }
    if( argc > 2)
    { 
      if( -1 == (argc = sub_array( argc, 2 , argc , &argv)))
      {
        return -1;
      }
      if( execute(argc , argv) == -1)
      {
        return -1;
      }
    }
  }
  else if( execute( argc, argv) == -1)
  {
    return -1;
  }
  else
  {
 //free memory of last input
    free( argv);
  }
  return 0;
}

int main(void)
{
  while(1)
  {
    if( mysh() == -1 )
    {
      fprintf(stderr, "Error!\n" );
    } 
  }
  exit(0);
}
