# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmevelec <gmevelec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/12/10 14:41:44 by tvallee           #+#    #+#              #
#    Updated: 2016/12/08 21:43:41 by gmevelec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC		= main.cpp Parser.cpp Exception.cpp Node.cpp implementation.cpp
NAME	= n_puzzle
CC		= g++
CFLAGS 	= -Wall -Wextra -std=c++11 -O3
SRC_DIR	= srcs
VPATH	= $(SRC_DIR)
INC		= headers/
OBJ_DIR	= obj
OBJ		= $(addprefix $(OBJ_DIR)/,$(SRC:.cpp=.o))
LIB		= 
LIBFT   = 

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	@echo "Linkin'"
	@$(CC) -o $(NAME) $(CFLAGS) $(OBJ) -I $(INC)
	@echo "Done !"

$(OBJ_DIR)/%.o: %.cpp
	@echo "Compiling $<."
	@$(CC)  $(CFLAGS) -c $<  -o $@ -I $(INC)
	echo  "done"
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "Deleting obj files."
	rm -Rf $(OBJ_DIR)

fclean: clean
	@echo "Deleting binary."
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re