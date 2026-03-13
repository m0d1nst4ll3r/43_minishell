/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:58:12 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 15:07:44 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// env_line is guaranteed to be KEY=VALUE
static int	fill_node(t_env_list *node, char *env_line)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env_line[i] != '=')
		i++;
	node->key = ft_strndup(env_line, i);
	if (!node->key)
		return (0);
	j = 0;
	while (env_line[j])
		j++;
	node->value = ft_strndup(env_line + i + 1, j - 1);
	if (!node->value)
	{
		free(node->key);
		return (0);
	}
	return (1);
}

static t_env_list	*new_node(char *env_line)
{
	t_env_list	*node;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	if (!fill_node(node, env_line))
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

// minishell cannot crash, so in case of error, we do not exit,
//	we just print the error and do with what we got.
t_env_list	*build_env(char **envp)
{
	t_env_list	*env;
	t_env_list	*cur;
	t_env_list	*last;
	int			i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		cur = new_node(envp[i]);
		if (!cur)
		{
			print_error(ERR_MALLOC);
			break ;
		}
		if (!env)
			env = cur;
		else
			last->next = cur;
		last = cur;
		i++;
	}
	return (env);
}
