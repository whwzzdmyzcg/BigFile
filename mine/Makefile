All:child-0 child-1 parent gen_file
child-0:child-0.o
	gcc -Wall -g -o child-0 child-0.c -lpthread
child-1:child-1.o
	gcc -Wall -g -o child-1 child-1.c -lpthread
parent:parent.o
	gcc -Wall -g -o parent parent.c -lpthread
gen_file:gen_file.o
	gcc -Wall -g -o gen_file gen_file.c
PHONY:clean
clean:
	rm child-0 child-0.o child-1 child-1.o parent parent.o gen_file gen_file.o
