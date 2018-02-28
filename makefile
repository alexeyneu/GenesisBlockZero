CC=cl -c
CFLAGS= -O2 -DSTRICT -Ow -W3 -Tp
CFLAGSMT=
LINKER=cl
GUILIBS=/NXCOMPAT /MACHINE:X64 /ERRORREPORT:NONE 

k.exe : BlockZero.obj
	$(LINKER) $(GUILIBS) BlockZero.obj   


BlockZero.obj :BlockZero.cpp 
	$(CC) $(CFLAGS) BlockZero.cpp


