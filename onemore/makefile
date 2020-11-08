CC=gcc -c
CFLAGS=
CFLAGSMT=-O3 
LINKER=gcc
GUILIBS=
trail : thost tsupport

thost : thost.o
	$(LINKER) $(GUIFLAGS)  thost.o  $(GUILIBS) -o thost

thost.o : hmb.c
	$(CC) $(CFLAGSMT) hmb.c -o thost.o

tsupport : tsupport.o
	$(LINKER) $(GUIFLAGS)  tsupport.o  $(GUILIBS) -o tsupport

tsupport.o : jyg.c
	$(CC) $(CFLAGSMT) jyg.c -o tsupport.o