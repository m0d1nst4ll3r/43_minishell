/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signal_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:09:43 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/20 17:33:11 by rapohlen         ###   ########.fr       */
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

static int	setup_sigint(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	if (sigemptyset(&sa.sa_mask)
		|| sigaction(SIGINT, &sa, NULL))
		return (1);
	return (0);
}

static int	setup_sigquit(void)
{
	struct sigaction	sa;

	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigemptyset(&sa.sa_mask)
		|| sigaction(SIGQUIT, &sa, NULL))
		return (1);
	return (0);
}

int	setup_signal_handlers(void)
{
	if (setup_sigquit() || setup_sigint())
		return (1);
	return (0);
}
