all: mysh mytee
	
mysh: mysh.c mytee  
	gcc mysh.c -Wall -o mysh -g

mytee: mytee.c
	gcc mytee.c -Wall -o mytee -g

clean:
	rm mysh
	rm mytee
