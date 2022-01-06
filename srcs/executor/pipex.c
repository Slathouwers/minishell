/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamighi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 10:20:52 by tamighi           #+#    #+#             */
/*   Updated: 2022/01/06 13:11:12 by tamighi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parent_process(t_cmdline *cmdline)
{
	int		p;

	if (!(cmdline->cmds + 1)->cmd)
		wait(&cmdline->cmds->exitstatus);
	if (close(cmdline->cmds->p[1]) == -1)
		exit(EXIT_FAILURE);
	p = cmdline->cmds->p[0];
	cmdline->cmds++;
	cmdline->cmds->fd_in = p;
}

void	fork_call(t_cmdline *cmdline)
{
	pid_t	pid;

	pipe(cmdline->cmds->p);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE); //correct exit
	else if (pid == 0)
	{
		if (close(cmdline->cmds->p[0]) == -1)
			exit(EXIT_FAILURE);
		if ((cmdline->cmds + 1)->command && cmdline->cmds->pipetype == 1)
			dup2(cmdline->cmds->p[1], 1);
		if (!check_if_builtin(cmdline, cmdline->builtins))
			redir_exec(cmdline);
		exit(0);
	}
	else
		parent_process(cmdline);
}

void	pipex(t_cmdline *cmdline)
{
	cmdline->cmds->fd_in = 0;
	expander(cmdline);
	if (miscarriage(cmdline))
		cmdline->cmds++;
	else
		fork_call(cmdline);
	while (cmdline->cmds->cmd && (cmdline->cmds - 1)->pipetype <= 1)
	{
		expander(cmdline);
		fork_call(cmdline);
	}
	check_exit_status(cmdline);
	if (cmdline->cmds->cmd)
		pipex(cmdline);
}
