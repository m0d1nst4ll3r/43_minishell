/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:03:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 17:02:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

char			**build_env(char **envp);
void			setup_signal_handlers(void);
void			print_error(char *err_str);
t_command_table	parse(char *line);
int				execute(t_command_table cmd_table);
void			cleanup_line(char *line, t_command_table cmd_table);
void			cleanup_env(char **env);

#endif
