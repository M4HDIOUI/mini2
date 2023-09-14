
NAME = minishell

SRC = main.c execute.c ft_exit.c parsing.c tools.c tokenization.c env.c

OBJ = $(SRC:.c=.o)

HEADER = exec.h


CC = gcc
LIB = libft/libft.a
FLAGS = -Werror -Wall -Wextra  -fsanitize=address 

all :$(NAME) 


$(NAME) : $(OBJ)
	@echo "\n"
	@echo "\033[0;32mCompiling libft..."
	@make -C libft/
	@echo "\033[0;32mCompiling minishell..."
	@$(CC) $(FLAGS) $(OBJ) -L $(shell brew --prefix readline)/lib -lreadline  $(LIB) -o $@ 
	
	@echo "\n\033[0mCompilation Done !"

%.o : %.c  $(HEADER)
	@printf "\033[0;33mcreating minishell objects... %-33.33s\r" $@
	$(CC) $(FLAGS) -I $(shell brew --prefix readline)/include -c $< -o $@
clean :
	@echo "\nDeleting minishell objects..."
	@rm -f $(OBJ)
	@echo "\nDeleting libft objects..."
	@make -C libft/ clean
	@echo "\033[0m"

fclean : clean
	@echo "\nDeleting minishell objects..."
	@rm -f $(NAME)
	@echo "\nDeleting libft objects..."
	@make -C libft/ fclean
	@echo "\nDeleting executable..."
	@echo "\033[0m"
re : fclean all
