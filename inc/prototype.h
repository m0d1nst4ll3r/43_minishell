/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:03:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/16 11:03:32 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

char		**build_env(char **envp);
int			setup_signal_handlers(void);
void		print_error(char *err_str);
t_command	*parse(char *line);
int			execute(t_command *cmd_list);
void		cleanup_line(char *line, t_command *cmd_list);
void		cleanup_env(char **env);

#endif
