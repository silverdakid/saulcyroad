CFLAGS = -std=c99 -Wall -Wextra -pedantic  -Ilibs -I/opt/homebrew/include  -fsanitize=address
CC = gcc
LIBS = -lncurses
LDFLAGS = -L/opt/homebrew/lib -lcunit -lSDL2_ttf

# Ajout des flags pour SDL2 et SDL2_image
CFLAGS  += $(shell pkg-config --cflags sdl2)
LDFLAGS += $(shell pkg-config --libs   sdl2)
CFLAGS  += $(shell pkg-config --cflags sdl2_image)
CFLAGS  += $(shell pkg-config --cflags sdl2_mixer)
LDFLAGS += $(shell pkg-config --libs   sdl2_image)
LDFLAGS += $(shell pkg-config --libs   sdl2_mixer)

LIBS_DIR = libs
TESTS_DIR = tests

# Trouve les fichiers .c
APP= app.c 
FILES =  $(wildcard $(LIBS_DIR)/**/*.c) $(wildcard $(LIBS_DIR)/*.c)
TEST_FILES = $(wildcard $(TESTS_DIR)/*.c) $(wildcard $(TESTS_DIR)/**/*.c) $(wildcard $(TESTS_DIR)/***/**/*.c)

# Génére les fichiers .o
APP_OBJ= $(APP:.c=.o)
OBJ = $(FILES:.c=.o)
TEST_OBJ = $(TEST_FILES:.c=.o)

# compilation de l'exécutable
build: $(OBJ) $(APP)
	$(CC) $(CFLAGS) -o app $^ $(LIBS) $(LDFLAGS)

# Compilation des tests
build_test: $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o test $^ $(LIBS) $(LDFLAGS)

# Compilation des fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

# Exécution du programme
run: build
	./app

# Exécuter les tests
test: build_test
	./test
#Suppression des exécutables et .o
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(APP_OBJ) app test