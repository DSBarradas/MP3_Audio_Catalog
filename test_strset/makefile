# Makefile for testing strset

test_strset: test_strset.o strset.o
	gcc -g -Wall test_strset.o strset.o -o test_strset
	
test_strset.o: test_strset.c strset.h
	gcc -g -Wall -c test_strset.c
	
strset.o: strset.c strset.h
	gcc -g -Wall -c strset.c
