/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 20:51:44 by kebris-c          #+#    #+#             */
/*   Updated: 2025/12/02 20:03:19 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	dead_checker(t_table *table)
{
	int		i;
	long	last_meal;
	long	now;

	i = 0;
	while (i < table->n_philos)
	{
		last_meal = table->philos[i].last_meal;
		now = get_time(table).rel_ms;
		if (now - last_meal > table->time_to_die)
		{
			pthread_mutex_lock(&table->state_lock);
			table->finished = 1;
			pthread_mutex_unlock(&table->state_lock);
			print_force(table, table->philos[i].id, "died");
			return (true);
		}
		i++;
	}
	return (false);
}

//			print_dbg(&table->philos[i]);
static int	food_is_gone(t_table *table)
{
	int			i;
	long		now;

	i = 0;
	while (i < table->n_philos)
		if (table->philos[i++].meals < table->must_eat)
			return (false);
	pthread_mutex_lock(&table->state_lock);
	table->finished = 1;
	pthread_mutex_unlock(&table->state_lock);
	now = get_time(table).rel_ms;
	pthread_mutex_lock(&table->print_lock);
	printf("%ld Simulation ended: all philosophers ate %d times\n", \
			now, table->must_eat);
	return (pthread_mutex_unlock(&table->print_lock), true);
}

void	*monitor_routine(void *arg)
{
	t_table		*table;

	table = (t_table *)arg;
	while (1)
	{
		pthread_mutex_lock(&table->state_lock);
		if (table->finished)
		{
			pthread_mutex_unlock(&table->state_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&table->state_lock);
		if (dead_checker(table))
			return (NULL);
		if (table->must_eat != -1 && food_is_gone(table))
			return (NULL);
		ft_usleep(1, table);
	}
	return (NULL);
}
