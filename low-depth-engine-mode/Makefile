CXX = g++
CXXFLAGS = -Wall -std=c++17
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
CPP_FILES = main.cpp draw.cpp images.cpp bitboard.cpp input.cpp moveGen.cpp specialMove.cpp player.cpp state.cpp eval.cpp alphabeta.cpp
HEADER_FILES = constants.h alphabeta.h bitboard.h draw.h eval.h images.h input.h moveGen.h player.h specialMove.h state.h
TARGET = chess.exe

all: $(TARGET)

$(TARGET) : $(CPP_FILES) $(HEADER_FILES)
	$(CXX) $(CPP_FILES) $(CXXFLAGS) $(LIBS) -o $(TARGET)

clean:
	del $(TARGET)