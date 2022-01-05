/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slathouw <slathouw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 13:30:27 by tamighi           #+#    #+#             */
/*   Updated: 2022/01/03 12:53:01 by slathouw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expander(t_cmdline *cmdline)
{
	void	*ptr;
	t_args	*tmp;

	ptr = cmdline->cmds;
	while (cmdline->cmds->command)
	{
		cmdline->cmds->cmd = expand(cmdline->cmds->cmd,
				cmdline->env, &cmdline->cmds->args, 1);
		tmp = cmdline->cmds->args;
		while (tmp)
		{
			tmp->content = expand(tmp->content, cmdline->env, &tmp, 0);
			tmp = tmp->next;
		}
		cmdline->cmds++;
	}
	cmdline->cmds = ptr;
}