CC=gcc
CFLAGS_SIMD=-std=c99 -Wall -O0 -c -mavx -mfma -g

ex1: ex1_test.o ex1.o
	$(CC) $^ -o $@

ex1_test.o: ex1_test.c
	$(CC) $(CFLAGS_SIMD) $< -o $@

ex1.o: ex1.c ex1.h
	$(CC) $(CFLAGS_SIMD) $< -o $@


clean:
	rm -f ex1
	rm -f *.o
