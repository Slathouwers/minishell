/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slathouw <slathouw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 13:32:18 by slathouw          #+#    #+#             */
/*   Updated: 2022/01/26 14:43:38 by slathouw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoinfree(char *dest, const char *str)
{
	char	*tmp;

	tmp = ft_strjoin(dest, str);
	ft_ptrdel(dest);
	dest = tmp;
	return (dest);
}

static char	*get_matches_in_dir(DIR *dir, char *pattern)
{
	struct dirent	*entry;
	char			*concat;
	int				count;

	count = 0;
	entry = readdir(dir);
	concat = ft_strdup("");
	while (entry)
	{
		if (is_match(entry->d_name, pattern)
			&& is_visible(entry->d_name, pattern))
		{
			if (count)
				concat = ft_strjoinfree(concat, " ");
			count++;
			concat = ft_strjoinfree(concat, entry->d_name);
		}
		entry = readdir(dir);
	}
	return (concat);
}

char	*expand_wildcard(char *pattern)
{
	char	*curr_dir_path;
	char	*ret;
	char	*ret2;
	DIR		*curr_dir;

	curr_dir_path = getcwd(NULL, 0);
	if (!curr_dir_path)
		return (pattern);
	curr_dir = opendir(curr_dir_path);
	ret = get_matches_in_dir(curr_dir, pattern);
	closedir(curr_dir);
	if (!*ret)
		ret2 = pattern;
	else
		ret2 = ms_strjoin(ret, "");
	free(ret);
	free(curr_dir_path);
	return (ret2);
}
