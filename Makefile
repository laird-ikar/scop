# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/15 16:05:50 by bguyot            #+#    #+#              #
#    Updated: 2023/11/17 10:52:08 by bguyot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	scop

INC_FILE	=	scop
SRC_FILE	=	scop

INCS 		=	$(addprefix incs/, $(addsuffix .hpp, $(INC_FILE)))
SRCS 		=	$(addprefix srcs/, $(addsuffix .cpp, $(SRC_FILE)))
OBJS		=	$(SRCS:.cpp=.o)
LIBS		=	-lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lGLEW -lGL

CC			=	c++
FLAGS		=	-Wall -Wextra -Werror -I incs
DEBUG_FLAGS	=	-fsanitize=address -g

all: $(NAME)

$(NAME): $(OBJS) $(INCS) $(LIBS)
	$(CC) $(FLAGS) $(DEBUG_FLAGS) $(OBJS) -o $(NAME) $(LIBS)

.cpp.o:
	$(CC) -c $(FLAGS) $(DEBUG_FLAGS) -o $@ $<

re: fclean all

fclean: clean
	rm -rf $(NAME)

clean:
	rm -rf $(OBJS)

.PHONY: all re fclean clean .cpp.o
