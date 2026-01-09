# ===== CONFIG =====
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11
INCLUDES = -Idisplay -Iutils
TARGET  = demi_le_cas

# ===== SOURCES =====
SRC = main.c \
      display/display.c \
      utils/utils.c

OBJ = $(SRC:.c=.o)

# ===== RULES =====
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
