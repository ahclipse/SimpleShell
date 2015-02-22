#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{

  char str[1025];
  FILE * out; 
  out = fopen ("tee.txt", "w"); 
  if(out == NULL) exit(1);

  while(fgets (str, 1024, stdin) != NULL){
	printf("%s", str); 
	fprintf(out, "%s", str);
  }

  exit(0);
}
