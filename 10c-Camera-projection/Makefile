# $@ nome del target
# @^ lista delle dipendenze

CC = g++
CCFLAGS = -O3 -s -DNDEBUG

ifeq ($(OS),Windows_NT)
	BASEDIR = ../base
	INCLUDEDIRS += -I$(BASEDIR)/freeglut/include
	INCLUDEDIRS += -I$(BASEDIR)/glew/include
	INCLUDEDIRS += -I$(BASEDIR)/glm

	LIBDIRS += -L$(BASEDIR)/freeglut/lib
	LIBDIRS += -L$(BASEDIR)/glew/lib/Release/Win32

	LIBS += -lfreeglut 
	LIBS += -lopengl32 
	LIBS += -lglew32

else
	LIBS += -lglut 
	LIBS += -lGLEW
	LIBS += -lGL
endif

OBJS = main.o utilities.o transform.o camera.o

camera-projection.exe : $(OBJS)
	$(CC) $(CCFLAGS) $^ $(LIBDIRS) $(LIBS) -o $@

main.o : main.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

utilities.o : utilities.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

transform.o : transform.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

camera.o : camera.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

.PHONY clean:
clean:
	rm *.o *.exe


