CC = g++ 
LIBS = -ldl 
 .cc.o:
	$(CC) -ggdb -fPIC -c $<
default:
	make testdcl
	make testdcl1
OBJS = testdcl.o testdcl1.o 
testdcl: testdcl.o
	$(CC) -rdynamic -o testdcl testdcl.o $(LIBS)
testdcl1: testdcl1.o
	$(CC) -rdynamic -o testdcl1 testdcl1.o $(LIBS)
libcircle.so:	circle.o
	g++ -shared -o libcircle.so circle.o
libsquare.so:	square.o
	g++ -shared -o libsquare.so square.o
all: testdcl libcircle.so libsquare.so testdcl1
clean:
	rm -f *.so *.o testdcl testdcl1
