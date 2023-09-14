
#include "exec.h"


t_type      add_type(t_global *global, int *i)
{
    if (is_pipe(global, i))
        return (PIPE);
    else if (is_command(global, i))
        return (COMMAND);
    else if (is_flag(global, i))
        return (FLAG);
    else if (is_param(global, i))
        return (PARAM);
    return (NUL);
}

t_tokens *add_token(t_global *global, int *i)
{
    int start;
    int len;
    t_tokens *new_tok;

    while (is_blank(global->line[*i]))
       *i += 1;
    new_tok = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new_tok)
		return (NULL);
   
    start = *i;
    new_tok->type = add_type(global, i);
    if (new_tok->type == NUL)
    {
        free(new_tok);
        global->cmd_status = 0;
        global->is_redir = 0;
        // ft_bzero(global, sizeof(t_global));
        return(NULL);
    }
    len = *i - start;
    new_tok->token = ft_substr(global->line, start, len);
    new_tok->next = NULL;
    return (new_tok);
}

void    parse(t_global *global)
{
    int i;
    t_tokens *head;

    i = 0;
    global->tokens = add_token(global, &i);
    head = global->tokens;
    while (head)
    {
        head->next = add_token(global, &i);
        head = head->next;
    }
}