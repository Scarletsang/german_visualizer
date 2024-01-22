################################
######     Variables     #######
################################

NAME:=markdown.out

CC:=g++
CXXFLAGS= -std=c++17 -pedantic -Wall -Wextra -Werror -MMD -MP -O2
LDFLAGS= -std=c++17 -pedantic
ifdef FSANITIZE
	CXXFLAGS+= -g3 -fsanitize=address -DDEBUG=1
	LDFLAGS+= -g3 -fsanitize=address
endif
INCLUDE_DIR:= src

###################################
######     Source files     #######
###################################

# To add souce files, create a varaible for each folder, and then
# contatenate them in the SRC variable like this:

MAIN_SRC:= \
	Elements.cpp \
	main.cpp
TOKENIZER_SRC:= \
	Parser/Token/Character.cpp \
	Parser/Token/Number.cpp \
	Parser/Token/Space.cpp \
	Parser/Token/Symbol.cpp \
	Parser/Token/Word.cpp \
	Parser/Tokenizer.cpp \
	Parser/Markdown/Tokenizer/Settings.cpp \
	Parser/Markdown/Tokenizer.cpp \
	Parser/Markdown/Parser.cpp
ELEMENT_SRC:= \
	Ast/Atom/Character.cpp \
	Ast/Atom/Number.cpp \
	Ast/Atom/Delimiter.cpp \
	Ast/Atom/Word.cpp \
	Ast/DomCollection/Paragraph.cpp \
	Ast/DomCollection/Section.cpp \
	Ast/DomCollection/Document.cpp \
	Ast/DomCollection/Sentence.cpp \
	Ast/Title.cpp
ENCODER_SRC:= \
	Encoder/Json/Settings.cpp \
	Encoder/Json.cpp
SRC:= $(MAIN_SRC) $(TOKENIZER_SRC) $(ELEMENT_SRC) $(ENCODER_SRC)

####################################
######     Library files     #######
####################################

# To compile a library, store a variable for their library file like this
# and add a rule for it after the main rules:
# LIBRARY_NAME=lib/LIBRARY_NAME/LIBRARY_NAME.a

# To add a library, add the library header file like this:
# INCLUDE_DIR+= lib/LIBRARY_NAME/include

# Then add the library to the linking process in one of the following ways:
# LDFLAGS+= -Llib/LIBRARY_NAME -lLIBRARY_NAME
# LDFLAGS+= lib/LIBRARY_NAME/libLIBRARY_NAME.a

###########################################
######     Object name reformat     #######
###########################################

# This in effect makes all the object files to be compiled in the OBJ_DIR directory

SRC_DIR:=src
OBJ_DIR:=obj
OBJ:=$(addprefix $(OBJ_DIR)/,$(subst /,@,$(SRC:.cpp=.o)))
DEPENDS:=$(OBJ:.o=.d)

#################################
######     Main rules     #######
#################################

all:
	@$(MAKE) $(NAME) -j

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $(NAME) $(LDFLAGS) && echo "Compilation of $(NAME) successful"

##########################################
######     Library compilation     #######
##########################################

# To compile a library, add a rule like this:

# $(LIBRARY_NAME):
# 	@${MAKE} $(if $(FSANITIZE),FSANITIZE=yes,) -C lib/LIBRARY_NAME

#########################################
######     Object compilation     #######
#########################################

-include $(DEPENDS)

.SECONDEXPANSION:
$(OBJ_DIR)/%.o: $(SRC_DIR)/$$(subst @,/,$$*).cpp
	@$(CC) $(CXXFLAGS) $(addprefix -iquote ,$(INCLUDE_DIR)) -c $< -o $@

$(OBJ): $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

###############################
######     Cleaning     #######
###############################

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re