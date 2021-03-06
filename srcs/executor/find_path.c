/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slathouw <slathouw@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 11:18:38 by tamighi           #+#    #+#             */
/*   Updated: 2022/01/29 09:30:53 by slathouw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_check(char s1, char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (s1 == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ms_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*res;
	char	*rres;

	i = 0;
	j = 0;
	while (s1[i])
		i++;
	while (s2[j])
		j++;
	res = ft_malloc(sizeof(char) * (i + j + 1), 0);
	rres = res;
	while (*s1)
		*res++ = *s1++;
	while (*s2)
		*res++ = *s2++;
	*res = '\0';
	return (rres);
}

char	*ms_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		j;
	int		k;
	int		l;

	i = 0;
	j = 0;
	while (s1[i] != '\0' && ft_check(s1[i], (char *)set))
		i++;
	j = ft_strlen((char *)s1);
	while (j > i && ft_check(s1[j - 1], (char *)set))
		j--;
	res = ft_malloc(sizeof(char) * (j - i + 1), 0);
	if (!res)
		return (res);
	k = i;
	l = 0;
	while (l < j - i)
	{
		res[l++] = s1[k];
		k++;
	}
	res[l] = '\0';
	return (res);
}

int	find_path2(char **tab, char *cmd, char **cmd1)
{
	int		i;
	char	*tmp;

	i = 0;
	while (tab[i])
	{
		*cmd1 = ms_strjoin(tab[i], "/");
		tmp = *cmd1;
		*cmd1 = ms_strjoin(*cmd1, cmd);
		ft_malloc(-1, tmp);
		if (access(*cmd1, X_OK) == 0)
			break ;
		ft_malloc(-1, *cmd1);
		i++;
	}
	if (tab[i] == NULL)
		return (0);
	return (1);
}

char	*find_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**tab;
	char	*cmd1;

	i = 0;
	if (!access(cmd, X_OK))
		return (cmd);
	while (ft_strlen(envp[i]) > 5 && ft_srch(envp[i], "PATH=") == 0)
		i++;
	if (!envp[i])
	{
		ft_fdprintf(2, "minishell: %s: No such file or directory\n", cmd);
		return (0);
	}
	path = ms_strtrim(envp[i], "PATH=");
	tab = ms_split(path, ':');
	ft_malloc(-1, path);
	if (find_path2(tab, cmd, &cmd1) == 0 && access(cmd, F_OK))
	{
		ft_fdprintf(2, "minishell: %s: command not found\n", cmd);
		return (0);
	}
	ft_malloc(-1, tab);
	return (cmd1);
}
