all: glast

clean:
	rm *.o

CFLAGS := -Wall -Werror -fPIC

INCFLAGS := -I.

LIBFLAGS := -lblah 

GLASTFILES := $(subst glast.c, , $(shell echo *.c))

GLASTOBJS := $(subst .c,.o, $(GLASTFILES))

%.o : %.c %.h
	$(CC) $(INCFLAGS) -c $(CFLAGS)  $< -o $@


#GLASTOBJS = asteroid.o matrix.o point.o ship.o util.o vector.o
	
glast: glast.c $(GLASTOBJS)
	gcc $(INCFLAGS) $(CFLAGS) glast.c $(GLASTOBJS) $(LIBFLAGS) -o glast

glast_static: glast.c $(GLASTOBJS)
	gcc $(INCFLAGS) $(CFLAGS) glast.c ..\Blah\$(GLASTOBJS) $(LIBFLAGS) -o glast
