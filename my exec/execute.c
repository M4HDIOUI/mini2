#include "exec.h"

int	get_len_pipe(t_tokens *token)
{
	int	i;

	i = 0;
	while (token && token->type != PIPE)
	{
		token = token->next;
		i++;
	}
	return (i);
}

int	is_there_pipe(t_tokens *head)
{
	while (head)
	{
		if (head->type == PIPE)
			return (1);
		head = head->next;
	}
	return (0);
}

t_tokens	*go_to_redir(t_tokens *head)
{
	while (head)
	{
		if (head->type == PIPE)
			break ;
		if (head->type == REDIR_IN || head->type == REDIR_OUT
			|| head->type == APPEND || head->type == HEREDOC)
			return (head);
		head = head->next;
	}
	return (NULL);
}

void    init_pipe(t_pipe *pipe)
{
    pipe->cmd_no = 0;
    pipe->pid = -1;
    pipe->pipe[0] = 0;
    pipe->pipe[1] = 0;
    pipe->temp = 0;
}

char	**list_to_tab(t_env *env_list)
{
	char	**tabs;
	t_env	*current;
	int		i;

	current = env_list;
	if (!current)
		return (NULL);
	tabs = NULL;
	i = -1;
	while (++i >= 0 && current != NULL)
		current = current->next;
	tabs = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tabs)
		return (NULL);
	current = env_list;
	i = 0;
	while (current != NULL)
	{
		if (current->var)
			tabs[i++] = ft_strdup(current->var);
		current = current->next;
	}
	tabs[i] = NULL;
	return (tabs);
}


char	**get_cmd(t_tokens *tokens)
{
    char **arr;
    int len;
    int i;

    len = get_len_pipe(tokens);

    if (!(arr = (char **)malloc(sizeof(char *) * (len + 1))))
        return NULL; 
    i = 0;
    while (tokens && tokens->type != PIPE)
    {
        if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT || tokens->type == APPEND)
            tokens = tokens->next;
        else if (tokens->type == COMMAND || tokens->type == FLAG
            || tokens->type == PARAM
            || tokens->type == D_QUOTE || tokens->type == S_QUOTE
            || tokens->type == ENV_VAR)
            {
                arr[i] = ft_strdup(tokens->token);
                i++;
            }
        if (!tokens)
            break;
        tokens = tokens->next;
    }
    arr[i] = NULL;
    return (arr);
}

void        ft_free(char **arr)
{
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

t_tokens *skip_to_pipe(t_tokens *tokens)
{
    while (tokens && tokens->type != PIPE)
    {
        tokens = tokens->next;
    }
    if (tokens && tokens->type == PIPE && tokens->next)
        return (tokens->next);
    return (NULL);
}

t_tokens    *redir_token(t_tokens *tokens)
{
    while (tokens)
    {
        if (tokens->type == PIPE)
            break;
        if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT || tokens->type  == APPEND || tokens->type == HEREDOC)
            return (tokens);
        tokens = tokens->next;
    }
    return (NULL);
}

void	redirect_in_out(t_tokens *token)
{
	int	fd;

	fd = 0;
	if (token->type == REDIR_OUT)
	{
		fd = open(token->next->token, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd > 0)
			dup2(fd, STDOUT_FILENO);
	}
	else if (token->type == REDIR_IN)
	{
		fd = open(token->next->token, O_RDONLY);
		if (fd > 0)
			dup2(fd, STDIN_FILENO);
	}
	else if (token->type == APPEND)
	{
		fd = open(token->next->token, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd > 0)
			dup2(fd, STDOUT_FILENO);
	}
	if (fd < 0)
		return ;
	close(fd);
}

void	execute_redirection(t_tokens *red)
{
	while (red != NULL)
	{
		if ((red->token && (red->type == REDIR_IN || red->type == REDIR_OUT
					|| red->type == APPEND)))
			redirect_in_out(red);
		// else if (red->token && red->type == HEREDOC)
		// 	redir_here_doc(red);
		red = go_to_redir(red->next);
	}
}

void    my_execve(char *binfile, char **cmd, char **tab)
{
    if (access(binfile, X_OK) == 0)
	{
		if (execve(binfile, cmd, tab) == -1)
		{
			ft_putendl_fd("minishell: Error: Execution Failed.", 2);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		ft_putendl_fd("minishell: Command not found ", 2);
		exit (EXIT_FAILURE);
	}
}

char	*get_binfile(char *cmd, t_env *env)
{
	int i;
	char buf[1096];
	char **paths;
	char *path_value;
	char *full_path;
	
	if (!(path_value = get_path(env, "PATH")))
		return (NULL);
	if (!(paths = ft_split(path_value, ':')))
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = NULL;
		ft_strcpy(buf, paths[i]);
		ft_strcpy(buf + ft_strlen(paths[i]), "/");
		full_path = ft_strjoin(buf, cmd);
		if (full_path && access(full_path, F_OK) == 0)
			break ;
		free(full_path);
		i++;
	}
	ft_free(paths);
	free(path_value);
	return (full_path);
}

void    execute_indirect(char **cmd, char **tab, t_env *env)
{
    // int status;
    char *binfile;
    // pid_t pid;

    binfile = get_binfile(cmd[0], env);
	printf("bin = %s\n", binfile);
	// exit(EXIT_SUCCESS);
    if (!binfile)
    {
        g_glb.exit_status = 127;
        write(2, "minishell: command not found\n", 50);
        return ;
    }
    if(access(binfile, F_OK) == 0)
	{
		printf("excve");
        my_execve(binfile, cmd, tab);
	}
}

int	is_valid_cmd(const char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
		return (1);
	if (buf.st_mode & S_IXUSR)
		return (0);
	return (2);
}


void	execute_direct(char **cmd, char **env)
{
	char	*file_name;
	pid_t		pid;
	int			status;

	if (cmd[0][0] == '.' && cmd[0][1] == '/')
		file_name = ft_strrchr(cmd[0], '/') + 1;
	else
		file_name = cmd[0];
	g_glb._status = 1;
	pid = fork();
	if (pid < 0)
		return (ft_putendl_fd("minishell: Error: forking Failded.\n", 2));
	if (pid == 0)
	{
		// if (access(file_name, F_OK))
        if (!is_valid_cmd(file_name))
			my_execve(file_name, cmd, env);
		else
			ft_putendl_fd("minishell: no such file or directory: ", 2);
		exit(1);
	}
	else
	{
        wait(&status);
        if (WIFEXITED(status))
		g_glb.exit_status = WEXITSTATUS(status);
    }
}


void    execute_pipe_child(t_tokens *tokens, char **cmd, t_pipe *pipes, t_env *env)
{
    t_tokens *redirection;

    // signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
    redirection = NULL;

    close(pipes->pipe[0]); // reading end
    if (pipes->cmd_no != 0)
    {
        dup2(pipes->temp, STDIN_FILENO);
    }
    if (is_there_pipe(tokens))
        dup2(pipes->pipe[1], STDOUT_FILENO);

    redirection = redir_token(tokens); // get redirection token
    if (redirection)
        execute_redirection(redirection);
    char **tab = list_to_tab(env);
    // execute_undirect:
    // execute_indirect:
	if (tab && ((cmd[0][0] == '.' && cmd[0][1] == '/' && cmd[0][2]) || (cmd[0][0] == '/' && cmd[0][1])))
        execute_direct(cmd, tab);
    else if (cmd[0] && tab)
		execute_indirect(cmd, tab, env);
    exit(g_glb.exit_status);
}


void    execute_pipe(t_tokens *tokens, char **cmd, t_pipe *pipes, t_env *env)
{
    // if (is_builtin(cmd[0]))
    // {
    //     exec_builtin(tokens, env);
    //     return ;
    // }
    if (pipe(pipes->pipe) == -1)
        return ;
    if ((pipes->pid = fork()) == -1)
        return ;
    if(pipes->pid == 0) // child
    {
        execute_pipe_child(tokens, cmd, pipes, env);
    }
    else // parent
    {
        close(pipes->pipe[1]); // Writing end
        if (pipes->temp)
        	close(pipes->temp);
        pipes->temp = pipes->pipe[0];
        if (!is_there_pipe(tokens))
            close(pipes->temp);
        pipes->cmd_no++;
    }
    return ;
}


int exec(t_global *global)
{
    t_pipe pipes;
    t_tokens *token;
    t_env *env = global->env;
    char **cmd;
    int status;

    init_pipe(&pipes);
    token = global->tokens;
    while(token)
    {
        if (!(cmd = get_cmd(token)))
            break;
        execute_pipe(token, cmd, &pipes, env);
        ft_free(cmd);
        token = skip_to_pipe(token);
    }
    if (pipes.temp)
		close(pipes.temp);
	if (pipes.pid)
		while (wait(&status) > 0)
			;
	if (WIFEXITED(status))
		g_glb.exit_status = WEXITSTATUS(status);
    return (0);
}