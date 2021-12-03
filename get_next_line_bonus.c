/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:52:46 by dasanter          #+#    #+#             */
/*   Updated: 2019/12/08 15:52:48 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int		isbsn(char *ligne)
{
	int i;

	if (ligne == NULL)
		return (0);
	i = -1;
	while (ligne[++i])
		if (ligne[i] == '\n')
			return (i + 1);
	return (0);
}

t_list	*get_fdlst(int fd, t_list **lst)
{
	t_list *tmp;
	t_list *new;

	tmp = *lst;
	while (tmp != NULL)
	{
		if (tmp->fd == fd)
			return (tmp);
		tmp = tmp->next;
	}
	if (!(new = malloc(sizeof(t_list))))
		return (NULL);
	new->fd = fd;
	new->next = *lst;
	new->lgn = NULL;
	*lst = new;
	return (*lst);
}

void	freememory(t_list **lst, int fd)
{
	t_list *tmp;
	t_list *prec;
	t_list *next;

	tmp = *lst;
	if ((*lst)->fd == fd && *lst != NULL)
	{
		tmp = (*lst)->next;
		free((*lst)->lgn);
		free(*lst);
		*lst = tmp;
		return ;
	}
	while (tmp != NULL)
	{
		prec = tmp;
		if (tmp->fd == fd)
		{
			next = tmp->next;
			prec->next = next;
			free(tmp->lgn);
			free(tmp);
			return ;
		}
	}
}

int		return_gestion(t_list **flist, int fd, char **line, int endof)
{
	t_list	*list;
	char	*buf;

	list = get_fdlst(fd, flist);
	if (endof == 0 && !isbsn(list->lgn))
	{
		*line = ft_substr(list->lgn, 0, ft_strlen(list->lgn));
		freememory(flist, fd);
		return (0);
	}
	else
	{
		*line = ft_substr(list->lgn, 0, isbsn(list->lgn) - 1);
		buf = list->lgn;
		list->lgn = ft_substr(list->lgn, isbsn(list->lgn),
				ft_strlen(list->lgn));
		free(buf);
	}
	return (1);
}

int		get_next_line(int fd, char **line)
{
	static t_list	*flist = NULL;
	t_list			*list;
	char			*buf;
	int				endof;

	if (fd < 0 || !BUFFER_SIZE || !line)
		return (-1);
	if (!(buf = malloc((BUFFER_SIZE + 1) * sizeof(char))))
		return (-1);
	list = get_fdlst(fd, &flist);
	if (list == NULL || read(fd, buf, 0) == -1)
	{
		free(buf);
		return (-1);
	}
	while (!isbsn(list->lgn) && (endof = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[endof] = 0;
		list->lgn = ft_strjoin(list->lgn, buf);
	}
	free(buf);
	return (return_gestion(&flist, fd, line, endof));
}
