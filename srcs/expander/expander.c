/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slathouw <slathouw@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 13:30:27 by tamighi           #+#    #+#             */
/*   Updated: 2022/01/19 09:11:50 by tamighi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_to_nb(char *new, char *var, int j)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != 34 && var[i] != '$')
		new[j++] = var[i++];
	return (new);
}

char	*expand_to_env(char *var, char **env, int ix, int i)
{
	char	*new;
	int		j;
	int		k;

	new = ft_malloc(ft_strlen(var) + ft_strlen(env[ix]) + 1, 0);
	j = 0;
	k = 0;
	while (var[i] != '$' && var[i])
		new[k++] = var[i++];
	if (var[i + 1] >= '0' && var[i + 1] <= '9')
		return (expand_to_nb(new, &var[i + 2], k));
	while (env[ix] && env[ix][j] != '=')
		j++;
	j++;
	while (env[ix] && env[ix][j])
		new[k++] = env[ix][j++];
	i++;
	while (var[i] && var[i] != ' ' && var[i] != 34
		&& var[i] != 39 && var[i] != '$' && var[i] != '*')
		i++;
	while (var[i])
		new[k++] = var[i++];
	new[k] = '\0';
	ft_malloc(-1, var);
	return (new);
}

int	env_index(char **env, char *var)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env[i])
	{
		while (env[i][j] == var[j])
		{
			j++;
			if (env[i][j] == '=' && (var[j] == ' ' || var[j] == 34 || !var[j]
					|| var[j] == '$' || var[j] == 39 || var[j] == '*'))
				return (i);
		}
		i++;
		j = 0;
	}
	return (i);
}

void	expander(t_cmdline *cmdline)
{
	t_args	*tmp;

	tmp = cmdline->cmds->args;
	cmdline->cmds->cmd = expand(cmdline->cmds->cmd, cmdline, 1);
	while (tmp)
	{
		tmp->content = expand(tmp->content, cmdline, 0);
		tmp = tmp->next;
	}
}
