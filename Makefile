NAME = minishell

FILES = main.c utils.c argv_del.c argv_insert.c argv_utils.c argv.c array_copy.c exec.c array_del.c array_insert.c array_utils.c array.c builtins1.c builtins2.c builtins3.c builtins4.c builtins5.c dolar_sign.c redirects.c token.c queto.c exec2.c builtin_exec.c

O_FILES = $(FILES:.c=.o)
OBJ_DIR = ./obj/
OBJS = $(addprefix $(OBJ_DIR), $(O_FILES))

LIBFT_DIR = ./ft_printf/
LIBFT =  $(addprefix $(LIBFT_DIR), libftprintf.a)

CC =  gcc


FLAGS = -g # -Wall -Werror -Wextra  #-fsanitize=address

SECURE = $(addprefix $(LIBFT_DIR), ft_printf.h) minishell.h

.PHONY: all clean fclean re

all : $(NAME)

$(NAME) : $(SECURE) $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline -I./
$(LIBFT) :
	make -C $(LIBFT_DIR)

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(FLAGS) -c $< -o $@ -I./ -g



clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re:
	@make fclean
	@make all

git :
	@git add .
	@git commit -m "auto commit"
	@git push

run : $(NAME)
	@./$(NAME)

debug : $(NAME)
	@lldb ./$(NAME)

r : run

d : debug
