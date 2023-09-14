
#include "exec.h"

void    free_tokens(t_tokens *tokens)
{
    t_tokens *next;

    next = NULL;
    while (tokens)
    {
        next = tokens->next;
        if (tokens->token) // token->data char *
            free(tokens->token);
        free(tokens); // free node (tokens);
        tokens = next;
    }
}

void    ft_exit(t_global *global, int exit_status)
{
    if (global->line)
        free(global->line);
    if (global->tokens)
        free_tokens(global->tokens);
    exit(exit_status);
}