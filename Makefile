# ===== CONFIG =====
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Wno-unused-parameter -std=c11
INCLUDES = -Idisplay -Iutils
TARGET  = bin/demi_le_cas bin/game/2048

# ===== SOURCES =====
SRC = main.c \
      display/display.c \
      utils/utils.c \
	  game/game_feature.c

SRC2 = game/2048.c \
      display/display.c \
      utils/utils.c \
	  game/game_feature.c

OBJ = $(SRC:.c=.o)
OBJ2 = $(SRC2:.c=.o)

# ===== RULES =====
all: $(TARGET)

bin/demi_le_cas : $(OBJ)
	mkdir -p bin
	$(CC) $(OBJ) -o $@

bin/game/2048 : $(OBJ2)
	mkdir -p bin/game
	$(CC) $(OBJ2) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
