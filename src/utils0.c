/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:57:47 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/25 15:31:53 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	if (!str)
		return ;
	if (fd < 0)
		fd = 1;
	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

t_time	get_time(t_table *table)
{
	t_time	time;

	gettimeofday(&time.timeval, NULL);
	time.abs_ms = time.timeval.tv_sec * 1000 + time.timeval.tv_usec / 1000;
	time.rel_ms = time.abs_ms - table->start_time;
	return (time);
}

int	exit_fail(char *flag, int threads, t_table *table)
{
	if (threads > 0)
	{
		table->finished = 1;
		while (--threads >= 0)
			pthread_join(table->philos[threads].thread, NULL);
	}
	ft_putstr_fd("Error\n", 2);
	if (!flag)
		return (EXIT_FAILURE);
	else
	{
		ft_putstr_fd(flag, 2);
		ft_putstr_fd("\n", 2);
	}
	return (EXIT_FAILURE);
}

void	free_table(t_table *table)
{
	int	i;

	if (table->forks)
	{
		i = 0;
		while (i < table->n_philos)
			pthread_mutex_destroy(&table->forks[i++]);
		free(table->forks);
	}
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->state_lock);
	if (table->philos)
	{
		i = 0;
		while (i < table->n_philos)
			pthread_mutex_destroy(&table->philos[i++].lock);
		free(table->philos);
	}
}
