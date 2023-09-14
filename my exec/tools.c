#include "exec.h"

int	is_blank(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}


int	is_charachter(char c)
{
	if (c == '<' || c == '|' || c == '>' || c == ';'
		|| c == '$' || c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	len_key(char *env)
{
	int	i;

	i = 0;
	while (env[i] != '\0' && env[i] != '=')
		i++;
	return (i);
}