# Makefile for audio catalogging app

app: app.o listfiles.o setdata.o binarytree.o linkedlist.o tag.o strset.o
	gcc -g -Wall app.o listfiles.o setdata.o binarytree.o linkedlist.o tag.o strset.o -o app

app.o: app.c listfiles.h setdata.h tagtype.h strset.h
	gcc -g -Wall -c app.c
	
listfiles.o: listfiles.c listfiles.h setdata.h tag.h strset.h
	gcc -g -Wall -c listfiles.c

setdata.o: setdata.c setdata.h binarytree.h tag.h tagtype.h
	gcc -g -Wall -c setdata.c
	
binarytree.o: binarytree.c binarytree.h linkedlist.h tagtype.h
	gcc -g -Wall -c binarytree.c

linkedlist.o: linkedlist.c linkedlist.h tag.h tagtype.h
	gcc -g -Wall -c linkedlist.c

tag.o: tag.c tag.h tagtype.h strset.h
	gcc -g -Wall -c tag.c

strset.o: strset.c strset.h
	gcc -g -Wall -c strset.c
