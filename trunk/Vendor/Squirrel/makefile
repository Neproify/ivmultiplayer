SQUIRREL= .

OUT= $(SQUIRREL)/libsquirrel.a
INCZ= -I$(SQUIRREL)/include -I. -Iinclude -I../../Shared -D_LINUX
DEFS= 
LIB=	

OBJS= *.o
	
SRCS= *.cpp

sq32:
	gcc-4.4 -m32 -O2 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing -c $(SRCS) $(INCZ) $(DEFS)
	ar rc $(OUT) *.o
	rm *.o

sqprof:
	gcc-4.4 -m32 -O2 -pg -fno-exceptions -fno-rtti -pie -gstabs -g3 -Wall -fno-strict-aliasing -c $(SRCS) $(INCZ) $(DEFS)
	ar rc $(OUT) *.o
	rm *.o

sq64:
	gcc-4.4 -O2 -m64 -D_SQ64 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing -c $(SRCS) $(INCZ) $(DEFS)
	ar rc $(OUT) *.o
	rm *.o
