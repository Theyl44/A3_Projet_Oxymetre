
NAME = oxymetre

SRC = 	main.c						\
		affichage.c					\
		fichiers.c					\
		fir.c				        \
		integration.c		        \
		lecture.c				    \
		mesure.c				    \
		iir.c                       \


OBJ = $(SRC:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Wshadow

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) -o $(NAME) $(OBJ)

clean:
		$(RM) $(OBJ)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re