/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slathouw <slathouw@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 14:58:53 by slathouw          #+#    #+#             */
/*   Updated: 2021/12/28 12:35:52 by tamighi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_cmdline *cmdline)
{
	char	**env;
	size_t	i;
	t_cmds	*cmds;

	env = cmdline->env;
	i = -1;
	cmds = cmdline->cmds;
	if ((*(cmds + 1)).command != NULL && cmds->pipetype == 1)
		dup2(cmds->p[1], 1);
	while (env[++i])
		if (ft_strchr(env[i], '='))
			ft_putendl_fd(env[i], 1);
	ft_putendl_fd(BBLU "THIS IS MINE" RESET, 1);
	return (1);
}
