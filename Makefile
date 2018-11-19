test: test.c
	gcc test.c -o test.o
# command: dependency (command to be called from the terminal)
# 			action
#
# e.g. make clean
clean:
	rm *.o 
