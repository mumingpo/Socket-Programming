all: test.c server.c
	gcc test.c -o test.o
	gcc server.c -o server.o
# command: dependency (command to be called from the terminal)
# 			action
#
# e.g. make clean
clean:
	rm *.o 
