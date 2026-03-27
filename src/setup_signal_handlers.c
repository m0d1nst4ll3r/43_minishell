/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signal_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:09:43 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/27 12:10:54 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler(int signum)
{
	(void)signum;
	rl_replace_line("", 1);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

int	reset_signal_handlers(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	if (sigemptyset(&sa.sa_mask))
		return (1);
	sa.sa_handler = SIG_DFL;
	if (sigaction(SIGQUIT, &sa, NULL) || sigaction(SIGINT, &sa, NULL))
		return (1);
	return (0);
}

int	setup_signal_handlers(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	if (sigemptyset(&sa.sa_mask))
		return (1);
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL))
		return (1);
	sa.sa_handler = sigint_handler;
	if (sigaction(SIGINT, &sa, NULL))
		return (1);
	return (0);
}
