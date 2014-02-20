CC=g++
EXEC=tp
TMPFOLDER=tmp

all: tp

		
tp: tp.o ImageBase.o
	if [ ! -d $(TMPFOLDER) ]; then mkdir $(TMPFOLDER); fi
	$(CC)  $(TMPFOLDER)/ImageBase.o $(TMPFOLDER)/tp.o -o exe

profil: profil.o ImageBase.o
	if [ ! -d $(TMPFOLDER) ]; then mkdir $(TMPFOLDER); fi
	$(CC) $(TMPFOLDER)/profil.o $(TMPFOLDER)/ImageBase.o -o profil

	
tp.o: tp.cpp ImageBase.h
	if [ ! -d $(TMPFOLDER) ]; then mkdir $(TMPFOLDER); fi
	$(CC) -o $(TMPFOLDER)/tp.o -c tp.cpp
	
profil.o: profil.cpp ImageBase.h
	if [ ! -d $(TMPFOLDER) ]; then mkdir $(TMPFOLDER); fi
	$(CC) -o $(TMPFOLDER)/profil.o -c profil.cpp

ImageBase.o: ImageBase.cpp
	if [ ! -d $(TMPFOLDER) ]; then mkdir $(TMPFOLDER); fi
	$(CC) -o $(TMPFOLDER)/ImageBase.o -c ImageBase.cpp
	
clean: 
	rm -rf *.o $(EXEC)
	rm -rf $(TMPFOLDER)/*.o $(EXEC)

