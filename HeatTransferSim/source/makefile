# Specify extensions of files to delete when cleaning
CLEANEXTS   = o a

CC = gcc
CFLAGS = -Wall -g

# Specify the target file and the install directory
OUTPUTFILE  = libbinimglib.a
INSTALLDIR  = ../binaries

.PHONY: all
all: libbinimglib.a
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) -o tester main.o -L. -lbinimglib  -lm




# Build libbinimglib.a
libbinimglib.a: binpypng.o binimg.o
	ar r $@ $^
	ranlib $@

binpypng.o: binpypng.c binpypng.h
	$(CC) $(CFLAGS) -c binpypng.c

binimg.o: binimg.c binimg.h
	$(CC) $(CFLAGS) -c binimg.c -lm

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)


.PHONY: clean
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done
	rm tester
