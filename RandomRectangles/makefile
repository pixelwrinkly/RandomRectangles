FLAGS = -Wall -pedantic -o game
LINKER_FLAGS = -lSDL2 -lSDL2_image
INCLUDE = -I /usr/local/Cellar/sdl2_image/2.0.5/include
LIB = -L /usr/local/Cellar/sdl2_image/2.0.5/lib
LIB_SDL_MAIN = -L /usr/local/Cellar/sdl2/2.0.12_1/lib/
FILES = ./src/*.c

build:
	gcc $(FILES) $(INCLUDE) $(LIB) $(LIB_SDL_MAIN) $(FLAGS) $(LINKER_FLAGS)
	
run:
	./game
	
clean:
	rm game