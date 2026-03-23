/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:58:12 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 16:04:42 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_env(char **env, char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			print_error(ERR_MALLOC);
			break ;
		}
		i++;
	}
	env[i] = NULL;
}

// minishell cannot crash, so in case of error, we do not exit,
//	we just print the error and do with what we have.
char	**build_env(char **envp)
{
	char	**env;
	size_t	len;

	len = 0;
	while (envp[len])
		len++;
	env = ft_malloc(sizeof(*env) * (len + 1));
	if (!env)
		return (NULL);
	fill_env(env, envp);
	return (env);
}
