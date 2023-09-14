#ifndef EXEC_H
#define EXEC_H

# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <sys/ioctl.h>
#include "libft/libft.h"



typedef enum s_type
{
	COMMAND,
	PIPE,
	NUL,
	S_QUOTE,
	D_QUOTE,
	HEREDOC,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	FLAG,
	PARAM,
	HERDOC_KEY,
	ENV_VAR,
}	t_type;

typedef struct s_var
{
	int		exit_status;
	int		_status;
	char	*tty_name;
}	t_var;

t_var	g_glb;

typedef struct s_pipe
{
	int					pipe[2];
	pid_t				pid;
	int					temp; // STDIN 0
	int					cmd_no;
}	t_pipe;

typedef struct s_tokens
{
	char			*token;
	t_type			type;
	struct s_tokens	*next;
}	t_tokens;

typedef struct s_env
{
	char			*var;
	struct s_env	*next;
}	t_env;

typedef struct s_global
{
	char		*line;
	t_env		*env;
	bool		cmd_status;
	bool		heredoc_activ;
	t_tokens	*tokens;
	int			fd[2];
	bool		is_redir;
	bool		herdoc_stat;
}	t_global;


int exec(t_global *global);
void	ft_free(char **arr);

void	ft_exit(t_global *global, int exit_status);
int		is_blank(char c);
int		is_charachter(char c);
int		len_key(char *env);
int		is_param(t_global *global, int	*i);
int     is_flag(t_global *global, int *i);
int     is_command(t_global *global, int *i);
t_type	is_pipe(t_global *global, int *i);

void    parse(t_global *global);
t_tokens *add_token(t_global *global, int *i);


void    print_tokenz(t_tokens *tok);
char	*get_path(t_env *envirement, char *name);
#endif