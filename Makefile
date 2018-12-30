GCC=/usr/bin/gcc

simplefs: shell.o disk.o bitmap.o fs.o
	$(GCC) shell.o disk.o bitmap.o fs.o -o simplefs

shell.o: shell.c
	$(GCC) -Wall shell.c -c -o shell.o -g

bitmap.o: bitmap.c
	$(GCC) -Wall bitmap.c -c -o bitmap.o -g

fs.o: fs.c fs.h
	$(GCC) -Wall fs.c -c -o fs.o -g

disk.o: disk.c disk.h
	$(GCC) -Wall disk.c -c -o disk.o -g

run:
	./simplefs images/image.200 200

clean:
	rm simplefs disk.o fs.o shell.o
