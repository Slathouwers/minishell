/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slathouw <slathouw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 10:47:11 by tamighi           #+#    #+#             */
/*   Updated: 2022/01/03 15:04:16 by slathouw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>

void	sig_handler(int sig, siginfo_t *siginfo, void *uac)
{
	t_cmdline	*cl;

	(void) uac;
	cl = cl_ptr(NULL);
	if (sig == SIGINT)
	{
		ft_printf("\n");
		if (cl->shellpid == siginfo->si_pid)
		{
			cl->exit = 130;
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			cl->quit = 1;
		}
		else
			cl->exit = 1;
	}
	if (sig == SIGQUIT)
	{
		ft_printf("%s%s", cl->prompt, rl_line_buffer);
		rl_redisplay();
		cl->quit = 2;
	}
}

static void	setup_term(void)
{
	struct termios	t;

	tcgetattr(0, &t);
	t.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &t);
}

void	signal_management(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	setup_term();
}
