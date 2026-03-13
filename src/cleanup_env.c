/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:40:29 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 15:42:17 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_env(t_env_list *env)
{
	t_env_list	*last;

	while (env)
	{
		last = env;
		env = env->next;
		free(last->key);
		free(last->value);
		free(last);
	}
}
