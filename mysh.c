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
  if( ((sub_arr) = malloc( (copyNum + 2) * sizeof(char*))) == NULL)//take away null 1 so space for NULL
  {
    return -1;
  }
  memcpy( sub_arr, ((char *)(*array + startIndex )), copyNum * sizeof(char*) );
  sub_arr[copyNum + 1] = NULL;
  free(*array);
  int i;
  for( i = 0; i < copyNum ; i ++)
  {
    //printf("subAR %d\t%s\n",i,sub_arr[i]);
  }
  (*array) = sub_arr;
  return copyNum;
}

int execute( int argc, char **argv)
{
  int redirectNotFound;
  int out;
  int i;
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
  //for( i = 0 ; i < argc ; i++){ printf("%d\t%s\n",i,argv[i]);}
  if( execvp( argv[0], argv) == -1)
  { 
	  fprintf(stderr, "Error!\n" );
	  exit(0);//child process should close even if exec fails
  }
  exit(0);//Should never reach here
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
  int status;
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
  if( strcmp( argv[0], pwdString) == 0)
  { //Code to print out current directory when pwd is inserted
   if(getcwd(currentDir, sizeof(currentDir))!= NULL){
    printf("%s\n", currentDir); 
    return 0;
   }
    else
    {
      return -1;
    }
  }
  //check for cd
  if( strcmp( argv[0], cdString) == 0)
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
    if( argc < 3)
    {
      return 0; 
    }
  
    if( -1 == (argc = sub_array( argc, 2 , argc , &argv)))
    {
      return -1;
    }
  }

  //Pipes
  int pipe1[2];
  int pipe2[2];
  int pids[3];
  int sub_argc;
  char **sub_argv;  

  i = 0;
  int pipesFound = 0;
  while( i < argc && pipesFound < 2)  
  {
    if( strcmp( argv[i] ,"|\0" ) == 0 )
    {
      if( pipesFound == 0)
      {
        pipe(pipe1);
      }
      if( pipesFound == 1)
      {
        pipe(pipe2);
      }

      if( !(pids[pipesFound] = fork()) )
      {
        if( pipesFound == 0){
          close(pipe1[0]);    
          dup2( pipe1[1], 1);
        }
        else
        {
          close( pipe1[1]);
          close( pipe2[0]);
          dup2(pipe1[0], 0);
          dup2(pipe2[1],1);
        }
        argc = sub_array( argc , 0 , i , &argv);
        //for( i = 0 ; i < argc ; i ++){  fprintf(stderr, "pipe1 args:%d\t%s\n",i,argv[i]);}
        execute( argc, argv);
        return -1;
      }

      argc = sub_array( argc , i+1, argc , &argv);
      i = 0;
      pipesFound++;
      
    }
    i ++;
  }

  //right side of pipe
  if( (pids[2] = fork()) == 0 )
  {
    if(pipesFound == 1)
    {
      close(pipe1[1]);
      dup2( pipe1[0], 0);
    } 
    if(pipesFound == 2)
    {
      close(pipe2[1]);
      dup2( pipe2[0], 0);
    }
    //for( i = 0 ; i < argc ; i++){ printf("%s\n",argv[i]);}
    //for( i = 0 ; i < argc ; i ++){  fprintf(stderr, "pipe2 args:%d\t%s\n",i,argv[i]);}
    execute( argc, argv);
  }

  if( pipesFound > 0)
  {
    close(pipe1[0]);
    close(pipe1[1]);
    if( pipesFound > 1)
    {
      close(pipe2[0]);
      close(pipe2[1]);
    }
  }

  //parent
  if( waitpid( pids[2], &status, 0) == -1)
  {
    free( argv );
    return -1;
  }

  if( pipesFound > 0)
  {
    if( waitpid( pids[0], &status, 0) == -1)
    {
      free( argv );
      return -1;
    }
    
    if( pipesFound > 1)
    {
      if( waitpid( pids[1], &status, 0) == -1)
      {
        free( argv );
        return -1;
      }
    }
  } 
  free( argv);
  
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
