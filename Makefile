#!/usr/bin/make -f

LIBRARY = ./libft/libft.a

NAME = ft_contrast

HEADERS = ft_contrast.h

OBJECTS = ft_contrast.o

CFLAGS = -Wall -Werror -Wextra#-g -fno-omit-frame-pointer -fsanitize=address

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C ./libft/
	$(CC) $(CFLAGS) $(OBJECTS) -L. ./libft/libft.a -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
fclean: clean
	rm -f $(NAME) $(OBJECTS)
re: fclean all

