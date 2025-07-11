CC = g++
CCFLAGS = -O3 -s -DNDEBUG
TARGET = progetto.exe

# Configurazione dipendente dal sistema operativo
ifeq ($(OS),Windows_NT)
	BASEDIR = ../lib
	INCLUDEDIRS = -I$(BASEDIR)/freeglut/include \
				-I$(BASEDIR)/glew/include \
				-I$(BASEDIR)/glm \
				-I$(BASEDIR)/assimp

	LIBDIRS = -L$(BASEDIR)/freeglut/lib \
			-L$(BASEDIR)/glew/lib/Release/Win32 \
			-L$(BASEDIR)/assimp/lib

	LIBS = -lfreeglut -lopengl32 -lglew32 -lassimp
else
	LIBS = -lglut -lGLEW -lGL -lassimp
endif

# Lista dei file sorgente
SRCS = main.cpp utilities.cpp common.cpp transform.cpp camera.cpp \
		shaderclass.cpp myshaderclass.cpp light.cpp texture2D.cpp \
		mesh.cpp noise.cpp terrain.cpp chunkManager.cpp skybox.cpp

# Genera automaticamente la lista degli object files
OBJS = $(SRCS:.cpp=.o)

# Target principale
$(TARGET): $(OBJS) 
	$(CC) $(CCFLAGS) $^ $(LIBDIRS) $(LIBS) -o $@
	@echo "Build completata con successo!"

# Pattern rule per compilare i file .cpp in .o
%.o: %.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $< -o $@

.PHONY: clean clean-obj all

# Pulisce solo i file oggetto
clean-obj:
	@echo "Pulizia dei file oggetto..."
	@rm -f $(OBJS)

# Pulisce tutto
clean:
	@echo "Pulizia completa..."
	@rm -f $(OBJS) $(TARGET)

# Ricompila tutto
all: clean $(TARGET)