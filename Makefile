OPENGLLIB = -lopengl32
GLEWLIB = -lglew32s -lglew32d
GLFWLIB = -lglfw3
LIBS = $(OPENGLLIB) $(GLFWLIB) $(GLEWLIB) 
LDFLAGS=-L"D:/lib"
CPPFLAGS=-I"D:/include"

BIN = Physicc

SRCS1 = Physicc.cpp utility/skybox.cpp utility/Texture.cpp

INCLUDES=utility/camera.h utility/shader.h utility/skybox.h utility/stb_image.h

all: $(BIN)

$(BIN): $(SRCS1) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)