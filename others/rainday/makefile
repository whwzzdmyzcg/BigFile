compute_sum : main.o io_sum mmap_sum 
	gcc -o compute_sum main.o
io_sum:	io_sum.o
	gcc -o io_sum -lpthread io_sum.o
mmap_sum: mmap_sum.o
	gcc -o mmap_sum -lpthread mmap_sum.o
main.o : main.c
	gcc -c main.c
io_sum.o: io_sum.c
	gcc -c io_sum.c
mmap_sum.o : mmap_sum.c
	gcc -c mmap_sum.c 

.PHONY : clean
clean :
	rm *.o
	rm mmap_sum
	rm compute_sum
	rm io_sum

