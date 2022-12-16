/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:57:03 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/15 15:22:55 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_ident_len(char *s, int print_msg)
{
	int		i;
	int		err;
	char	*temp;

	i = 0;
	err = 0;
	while (s[i] != '=' && s[i] != '\0')
	{
		if ((!isalnum(s[i]) && s[i] != '_') || ft_isdigit(s[0]))
			err = 1;
		i++;
	}
	if (i == 0 || err == 1)
	{
		if (print_msg == 1)
		{
			temp = ft_strjoin("`", s);
			temp = str_extend(&temp, "'");
			print_long_error_msg("export", temp, "not a valid identifier");
			if (temp)
				free(temp);
		}
		i = 0;
	}
	return (i);
}

void	print_export_line(t_cmmnds *node, t_uni *u, int i)
{
	size_t	len;

	len = get_ident_len(u->envp[i], 0);
	ft_putstr_fd("declare -x ", node->outf);
	if (len > 0)
	{
		write (node->outf, u->envp[i], len);
		if (len != ft_strlen(u->envp[i]))
		{
			ft_putstr_fd("=\"", node->outf);
			ft_putstr_fd(u->envp[i] + (len + 1), node->outf);
			ft_putstr_fd("\"", node->outf);
		}
	}
	else
		ft_putstr_fd(u->envp[i], node->outf);
	ft_putstr_fd("\n", node->outf);
}

int	print_export(t_cmmnds *node)
{
	t_uni	*u;
	int		i;

	i = -1;
	u = node->uni;
	while (u->envp && u->envp[++i])
	{
		if (ft_strncmp(u->envp[i], "_=", 2) != 0)
			print_export_line(node, u, i);
	}
	return (0);
}

int	search_in_envp(t_uni *uni, char *str, int ident_len)
{
	int	i;
	int	envp_ident_len;

	i = 0;
	while (uni->envp[i])
	{
		envp_ident_len = get_ident_len(uni->envp[i], 0);
		if (envp_ident_len == ident_len
			&& ft_strncmp(uni->envp[i], str, ident_len) == 0)
		{
			if (ft_strchr(str, '=') != 0)
				change_in_env(ft_substr(uni->envp[i], 0, envp_ident_len),
					ft_strdup(str + envp_ident_len), uni->envp);
			break ;
		}
		else if (i == array_length(uni->envp) - 1)
		{
			uni->envp = add_str_to_array(uni->envp, str);
			break ;
		}
		i++;
	}
	return (0);
}

int	ft_export(t_cmmnds *node)
{
	t_uni	*uni;
	int		i;
	int		ident_len;
	int		ret_val;

	i = 1;
	ret_val = 0;
	uni = node->uni;
	if (array_length(node->cmd_array) == 1)
		return (print_export(node));
	if (!uni->envp && array_length(node->cmd_array) > 1
		&& get_ident_len(node->cmd_array[i], 0))
		uni->envp = add_str_to_array(uni->envp, node->cmd_array[i++]);
	while (node->cmd_array[i])
	{
		ident_len = get_ident_len(node->cmd_array[i], 1);
		if (ident_len != 0)
			search_in_envp(uni, node->cmd_array[i], ident_len);
		else
			ret_val = 1;
		i++;
	}
	return (ret_val);
}
