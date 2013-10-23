# Утилита, которая делает fs.img

all: mkfs

mkfs: mkfs.c fsutil.c ../include/sys/fs.h
	gcc -I .. -I -Wall -o mkfs mkfs.c

clean:
	rm -f mkfs *.o

