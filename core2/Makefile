CC= clang
CFLAGS= -Wall -Werror -Wextra
LDFLAGS= -v
TARGET=core2

SRC:= main.c
OBJ:= $(SRC:.c=.o)

ifdef DEBUG
CFLAGS += -DDEBUG -g
LDFLAGS += -DDEBUG -g
else ifdef ASAN
CFLAGS += -DDEBUG -g -fsanitize=address
LDFLAGS += -DDEBUG -g -fsanitize=address
else
CFLAGS += -O3
LDFLAGS += -O3
endif

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
CFLAGS += -I$(shell brew --prefix glfw)/include -I/usr/local/include
LDFLAGS += -L$(shell brew --prefix glfw)/lib -framework Cocoa -framework OpenGL -framework IOKit -L/usr/local/bin
endif

all: $(TARGET)

$(TARGET): $(OBJ) lalala.h
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
