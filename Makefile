# ===== CONFIG =====
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Wno-unused-parameter -pthread
LDFLAGS = -pthread
INCLUDES = -Idisplay -Iutils
TARGET  = bin/demi_le_cas bin/2048

# ===== SOURCES =====
SRC = display/display.c \
      utils/utils.c \
	  game/game_feature.c

OBJ = $(SRC:.c=.o)

# ===== RULES =====
all: $(TARGET)

bin/demi_le_cas : $(OBJ) main.o
	mkdir -p bin
	$(CC) $(OBJ) main.o $(LDFLAGS) -o $@

bin/2048 : $(OBJ) game/2048.o
	mkdir -p bin
	$(CC) $(OBJ) game/2048.o $(LDFLAGS) -o $@
	
%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) main.o game/2048.o display/display.o
	rm -fr bin

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
