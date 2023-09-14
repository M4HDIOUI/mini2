#include "exec.h"
#include <string.h>

void sig_handler(int num)
{
    if (num == SIGINT)
    {
        write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		rl_replace_line("", 0);
    }
}

void    add_env(char *env, t_env **begin)
{
    t_env *new_node;
    t_env *current;

    if (!(new_node = (t_env *)malloc(sizeof(t_env))))
        return ;
    new_node->next = NULL;
    new_node->var = strdup(env);
    if (!(*begin))
        *begin = new_node;
    else
    {
        current = *begin;
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_node;
    }
    
}


// void    add_token(char *env, t_tokens **begin)
// {
//     t_tokens *new_node;
//     t_tokens *current;

//     if (!(new_node = (t_tokens *)malloc(sizeof(t_tokens))))
//         return ;
//     new_node->next = NULL;
//     new_node->token = strdup(env);
//     if (!(*begin))
//         *begin = new_node;
//     else
//     {
//         current = *begin;
//         while (current->next)
//         {
//             current = current->next;
//         }
//         current->next = new_node;
//     }
    
// }


void    stock_env(t_global *global, char **env)
{
    int i;

    global->env = NULL;
    i = 0;
    while (env[i] != NULL)
    {
        add_env(env[i], &global->env);
        i++;
    }
}

void    print_tokenz(t_tokens *tok)
{
    while (tok)
    {
        printf("token = %s\ttype = %d\n", tok->token, tok->type);
        tok = tok->next;
    }
}

int main(int ac, char **av, char **env)
{
    t_global global;

    (void)av;
    if (ac > 1)
        return (1);
    ft_bzero(&global, sizeof(t_global));
    stock_env(&global, env);
    rl_catch_signals = 0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);


    while (1)
    {
        global.line = readline("$SHELL> ");
        if (global.line == NULL || ft_strcmp(global.line, "exit") == 0)
        {
            write(1, "exit", 4);
            ft_exit(&global, 0);
        }
        if (global.line[0])
            add_history(global.line);
        parse(&global);

        if (global.tokens) // print_tokenz
        {
            print_tokenz(global.tokens);
        }
        if (global.tokens)
            exec(&global);
    }
    return 0;

}