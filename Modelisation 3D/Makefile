CC=g++
TMP=tmp

all: tp

		
tp: TP_OPENGL.o point.o vector.o
	if [ ! -d $(TMP) ]; then mkdir $(TMP); fi
	$(CC)  $(TMP)/TP_OPENGL.o $(TMP)/point.o $(TMP)/vector.o -o tp -lglut -lGL

TP_OPENGL.o: TP_OPENGL.cpp point.o vector.o
	if [ ! -d $(TMP) ]; then mkdir $(TMP); fi
	$(CC) -o $(TMP)/TP_OPENGL.o -c TP_OPENGL.cpp
	
point.o: point.cpp
	if [ ! -d $(TMP) ]; then mkdir $(TMP); fi
	$(CC) -o $(TMP)/point.o -c point.cpp

vector.o: vector.cpp
	if [ ! -d $(TMP) ]; then mkdir $(TMP); fi
	$(CC) -o $(TMP)/vector.o -c vector.cpp
	
clean: 
	rm -rf *.o
	rm -rf $(TMP)/*.o

