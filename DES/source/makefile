# Specify extensions of files to delete when cleaning
CLEANEXTS   = o a

CC = gcc
CFLAGS = -Wall -g

# Specify the target file and the install directory
OUTPUTFILE  = libdesnoxlib.a
INSTALLDIR  = ../binaries
HEADERDIR = ../headers

# Default target
.PHONY: all
all: $(OUTPUTFILE)

# Build libdesnoxlib.a
$(OUTPUTFILE): desfel.o desdis.o desgrocery.o desairportcheckout.o dumptruckoperation.o dessupercomp.o
	ar r $@ $^
	ranlib $@

desfel.o: desfel.c desfel.h
	$(CC) $(CFLAGS) -c desfel.c

desdis.o: desdis.c desdis.h
	$(CC) $(CFLAGS) -c desdis.c

desgrocery.o: desgrocery.c desgrocery.h
	$(CC) $(CFLAGS) -c desgrocery.c

desairportcheckout.o: desairportcheckout.c desairportcheckout.h
	$(CC) $(CFLAGS) -c desairportcheckout.c
	
dumptruckoperation.o: dumptruckoperation.c dumptruckoperation.h
	$(CC) $(CFLAGS) -c dumptruckoperation.c

dessupercomp.o: dessupercomp.c dessupercomp.h
	$(CC) $(CFLAGS) -c dessupercomp.c


.PHONY: test
test:
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) -o tester main.o -L. -ldesnoxlib -lm


.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)
	mkdir -p $(HEADERDIR)
	cp -p *.h $(HEADERDIR)

.PHONY: clean
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done
	rm tester
