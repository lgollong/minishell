/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_here_doc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 20:02:22 by tfriedri          #+#    #+#             */
/*   Updated: 2022/12/14 11:21:27 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// pipe the result from here-doc to the command
int	pipe_here_doc(t_uni *uni, char *tmp, char *line, int check_meta)
{
	int	fd[2];
	int	cnt[2];
	int	i;

	cnt[0] = 0;
	cnt[1] = 0;
	i = -1;
	if (line)
		free(line);
	if (g_exitcode != 1)
	{
		while (check_meta == 1 && tmp && tmp[++i])
			tmp = check_for_metachars(uni, tmp, &i, cnt);
		if (pipe(fd) == 0)
		{
			write(fd[1], tmp, ft_strlen(tmp));
			free(tmp);
			close(fd[1]);
			return (fd[0]);
		}
	}
	if (tmp)
		free(tmp);
	return (0);
}

// here-doc
int	get_here_doc(t_uni *uni, char *limiter, int check_meta)
{
	char	*tmp;
	char	*line;

	line = NULL;
	tmp = NULL;
	g_exitcode = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
			print_error_msg("warning",
				"here-document delimited by end-of-file");
		if (!line || (ft_strncmp(line, limiter, ft_strlen
					(line)) == 0 && ft_strlen(line) == ft_strlen
				(limiter)) || g_exitcode == 1)
			break ;
		tmp = str_extend(&tmp, line);
		tmp = str_extend(&tmp, "\n");
		free(line);
	}
	return (pipe_here_doc(uni, tmp, line, check_meta));
}

// changes the signal-handler and starts here_doc
void	start_here_doc(t_uni *uni, t_cmmnds *cmd_strct, int i)
{
	char	*limiter;
	int		check_meta;
	int		j;

	check_meta = 1;
	j = 0;
	signal(SIGINT, &sig_handler_here_doc);
	limiter = uni->commands[i + 2];
	if (!limiter || limiter[0] == '<')
	{
		syntax_error(uni, limiter);
		return ;
	}
	if (ft_strchr(limiter, '\'') || ft_strchr(limiter, '\"'))
	{
		check_meta = 0;
		while (limiter[j] != '\"' && limiter[j] != '\'')
			j++;
		rem_quote_from_str(limiter, &j);
	}
	cmd_strct->inf = get_here_doc(uni, limiter, check_meta);
	if (g_exitcode == 1)
		cmd_strct->uni->stop = 1;
	signal(SIGINT, &sig_handler_after_rl);
}
