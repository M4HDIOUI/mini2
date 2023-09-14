

#include "exec.h"

t_type  is_pipe(t_global *global, int *i)
{
    if (global->line[*i] == '|')
    {
        *(i) += 1;
        global->cmd_status = 0;
    }
    else
        return (0);
    return (PIPE);
}

int  is_command(t_global *global, int *i)
{
    int start;

    start = *i;
    while (global->line[*i] && !is_blank(global->line[*i]) && 
        global->is_redir == 0 && global->cmd_status == 0
        && !is_charachter(global->line[*i]))
    {
        *i += 1;
        if (is_blank(global->line[*i]) ||
            is_charachter(global->line[*i]) || global->line[*i] == '\0' )
            {
                global->cmd_status = 1;
                return (1);
            }
    }
    *i = start;
    return (0);
}

int     is_flag(t_global *global, int *i)
{
    int start;

    start = *i;
    while (global->line[*i] == '-' && global->cmd_status == 1 && ft_isalpha(global->line[*i + 1]))
    {
        (*i)++;
		while (ft_isalpha(global->line[*i]))
			(*i)++;
        return (1);
    }
    *i = start;
    return (0);
}

int	is_param(t_global *global, int	*i)
{
	int	start;

	start = *i;
	while ((global->cmd_status == 1 || global->is_redir == 1)
		&& !is_blank(global->line[*i]) && global->line[*i])
	{
		(*i)++;
		if (is_blank(global->line[*i]) || !global->line[*i]
			|| is_charachter(global->line[*i]))
		{
			// if (!is_blank(global->line[*i]) && global->line[*i] != '|'
			// 	&& global->line[*i] != '<' && global->line[*i] != '>')
			// 	*no_space = 1;
			global->is_redir = 0;
			return (1);
		}
	}
	*i = start;
	return (0);
}