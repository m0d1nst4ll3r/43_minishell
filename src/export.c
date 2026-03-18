/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:33:04 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 19:57:58 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 1. Verify arg
// 2. Search if already exists
//	a. Y -> strdup, delete old, replace
//	b. N -> remalloc env, copy all, add new to end
// Nope, needs to be a loop
int	builtin_export(int ac, char **av, char ***ep)
{
	if (ac < 2 || !*ep)
		return (0);
	(void)av;
	return (0);
}

static int	delete_from_env(char *to_delete, char **old_env, char **new_env)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (ep[i])
	{
		if (ep[i] != to_delete)
		{
			new_env[j] = ep[i];
			j++;
		}
		i++;
	}
}

int	builtin_unset(int ac, char **av, char ***ep)
{
	char	**new_env;
	char	*to_delete;

	if (ac < 2 || !*ep)
		return (0);
	to_delete = get_env(av[1], *ep);
	if (!to_delete)
		return (0);
	new_env = ft_malloc(sizeof(*new_env) * get_env_size(*ep));
	if (!new_env)
	{
		print_error_builtin(av[0], ERR_MALLOC);
		return (1);
	}
	delete_from_env(to_delete, *ep, new_env);
	free(*ep);
	*ep = new_env;
	return (0);
}
