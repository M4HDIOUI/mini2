
#include "exec.h"

char	*get_path(t_env *envirement, char *name)
{
	char	*tmp;
	int		len;
	t_env	*env;

	env = envirement;
	if (name && env)
	{
		while (env)
		{
			len = len_key(env->var);
			if (ft_strncmp(env->var, name, len) == 0)
			{
				tmp = ft_strchr(env->var, '=');
				if (tmp)
				{
					tmp = ft_substr(tmp + 1, 0, ft_strlen(env->var) - len);
					return (tmp);
				}
			}
			env = env->next;
		}
	}
	return (NULL);
}