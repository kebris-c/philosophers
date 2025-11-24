/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:13:04 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/24 19:15:58 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	ft_usleep(long ms, t_table *table)
{
	long	start;

	start = get_time(table).rel_ms;
	while (get_time(table).rel_ms - start < ms)
	{
		pthread_mutex_lock(&table->state_lock);
		if (table->finished)
		{
			pthread_mutex_unlock(&table->state_lock);
			return ;
		}
		pthread_mutex_unlock(&table->state_lock);
		usleep(50);
	}
}
