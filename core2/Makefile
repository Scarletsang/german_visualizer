CC= clang
CFLAGS= -pedantic -Wall -Werror -Wextra
LDFLAGS= -pedantic -v -lglfw -lvulkan
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
LDFLAGS += -L$(shell brew --prefix glfw)/lib -framework Cocoa -framework OpenGL -framework IOKit -L/usr/local/lib
endif

all: $(TARGET)

$(TARGET): $(OBJ) lalala.h
ifeq ($(UNAME),Darwin)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS) && install_name_tool -add_rpath /usr/local/lib ./$(TARGET) && echo "Compilation successful"
else
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS) && echo "Compilation successful"
endif

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
