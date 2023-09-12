# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tda-silv <tda-silv@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/30 09:01:22 by tda-silv          #+#    #+#              #
#    Updated: 2023/09/04 12:41:07 by tda-silv         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= client_test

SRC_DIR		= source/
OBJ_DIR		= object/
INC_DIR		= include/

CC			= c++

CFLAGS		= -Wall -Wextra -Werror -Wshadow -Wconversion -std=c++98

# **************************************************************************** #
#                                                                              #
#   -I   | Chemin du dossier où trouver un .h								   #
#   -L   | Chemin du dossier où trouver un .a								   #
#   -l   | Nom du .a sans le préfixe "lib"									   #
#                                                                              #
# **************************************************************************** #

I_HEADER	= -I $(INC_DIR)

HEADER		= $(shell find include/ -type f)

SEARCH_FILE	= $(shell find $(SRC_DIR) -type f -name "*.cpp")
NAME_FILE	= $(shell echo $(SEARCH_FILE) | sed 's|source/||g' | sed 's|\.cpp||g')

SRC			= $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(NAME_FILE)))
OBJ			= $(addsuffix .o, $(addprefix $(OBJ_DIR), $(NAME_FILE)))
DEPENDS		= $(addsuffix .d, $(addprefix $(OBJ_DIR), $(NAME_FILE)))

# **************************************************************************** #
#                                                                              #
#   $@   | Le nom de la cible												   #
#   $<   | Le nom de la première dépendance									   #
#   $^   | La liste des dépendances											   #
#   $?   | La liste des dépendances plus récentes que la cible				   #
#   $*   | Le nom du fichier sans suffixe									   #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#   Relink si les headers ou le Makfile sont changés                           #
#                                                                              #
# *****************************vvvvvvvvvvvvvvvvvv***************************** #

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADER) Makefile
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(I_HEADER) -MMD -MP -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(I_HEADER) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

valgrind_memcheck v: $(OBJ)
	valgrind --tool=memcheck --track-origins=yes --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

valgrind_helgrind: $(OBJ)
	valgrind --tool=helgrind --history-level=full ./$(NAME)

-include $(DEPENDS)

.PHONY: all clean fclean re valgrind_memcheck v valgrind_helgrind
