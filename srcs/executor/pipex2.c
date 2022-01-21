/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slathouw <slathouw@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 10:55:09 by slathouw          #+#    #+#             */
/*   Updated: 2022/01/21 13:41:32 by slathouw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_builtins(t_cmdline *cmdline)
{
	t_builtins	builtins[8];
	builtins_init(cmdline);
	cmdline->builtins = builtins;
}

void	set_up_pipes(int *fds)
{
	pipe(fds);
	pipe(fds + 2);
}

void	switch_pipes_close_files(int *fds, t_cmds *cmd)
{
	close(fds[0]);
	close(fds[1]);
	fds[0] = fds[2];
	fds[1] = fds[3];
	pipe(fds + 2);
	while (cmd->outfiles && cmd->outfiles->fd > 2)
	{
		close(cmd->outfiles->fd);
		cmd->outfiles = cmd->outfiles->next;
	}
	while (cmd->infiles && cmd->infiles->fd > 2)
	{
		close(cmd->infiles->fd);
		cmd->infiles = cmd->infiles->next;
	}
}

void	wait_if_conditional(t_cmdline *cmdline, t_cmds * current)
{
	cmdline->cmds = current;
	if (current->pipetype != 1)
	{
		waitpid(cmdline->is_forked, &cmdline->cmds->exitstatus, 0);
		while (wait(0) != -1)
			;
		check_exit_status(cmdline);
	}
}

void	pipex(t_cmdline *cmdline, int *fds, int flag_in_out[2], t_cmds *current)
{
		int	i;
		int	fd_in;
		int fd_out;

		cmdline->cmds = current;
		if(miscarriage(cmdline))
		{
			cmdline->cmds->exitok = 1;
			check_exit_status(cmdline);
			return ;
		}
		cmdline->is_forked = fork();
		if(cmdline->is_forked == 0)
		{
			
		}
}

void	executor(t_cmdline *cmdline)
{
	init_builtins(cmdline);
	
	int			fds[4];
	const int	std_in = dup(0);
	int			flags_in_out[2];
	t_cmds		*current;


	flags_in_out[0] = 0;
	flags_in_out[1] = 0;
	current = cmdline->cmds;
	set_up_pipes(fds);
	while (current->cmd)
	{
		if (current->pipetype == 1)
			flags_in_out[1] = 1;
		pipex(cmdline, fds, flags_in_out, current);
		check_for_minishell(current->cmd);
		switch_pipes_close_files(fds, current);
		wait_if_conditional(cmdline, current);
		flags_in_out[0] = flags_in_out[1];
		flags_in_out[1] = 0;
		current++;
	}
}
