CC = g++

CC_FLAGS = -lSDL2\
	   -lSDL2_gfx\
	   -lSDL2_image\
	   -lSDL2_mixer\
	   -lSDL2_ttf

all: ./heat

./heat: ./2Dheat.cpp 
	$(CC) $< -o heat $(CC_FLAGS)
