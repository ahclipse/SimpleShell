all: mysh
	
mysh: mysh.c
	gcc mysh.c -Wall -o mysh -g

clean:
	rm mysh
