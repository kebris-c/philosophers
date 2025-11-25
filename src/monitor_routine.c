/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 20:51:44 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/25 18:54:56 by kebris-c         ###   ########.fr       */
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
		now = get_time(table).rel_ms;
		pthread_mutex_lock(&table->philos[i].lock);
		last_meal = table->philos[i].last_meal;
		pthread_mutex_unlock(&table->philos[i].lock);
		if (now - last_meal > table->time_to_die)
		{
			pthread_mutex_lock(&table->state_lock);
			table->finished = 1;
			pthread_mutex_unlock(&table->state_lock);
			pthread_mutex_lock(&table->print_lock);
			printf("%ld\t%d\tdied\n", now, table->philos[i].id);
			pthread_mutex_unlock(&table->print_lock);
			return (true);
		}
		if ((++i & 0xF) == 0)
			now = get_time(table).rel_ms;
	}
	return (false);
}

static int	food_is_gone(t_table *table)
{
	int			i;
	int			meals;
	long		now;

	i = 0;
	while (i < table->n_philos)
	{
		pthread_mutex_lock(&table->philos[i].lock);
		meals = table->philos[i].meals;
		pthread_mutex_unlock(&table->philos[i].lock);
		if (meals < table->must_eat)
			return (false);
		i++;
	}
	now = get_time(table).rel_ms;
	pthread_mutex_lock(&table->print_lock);
	printf("%ld Simulation ended: all philosophers ate %d times\n", \
			now, table->must_eat);
	pthread_mutex_unlock(&table->print_lock);
	pthread_mutex_lock(&table->state_lock);
	table->finished = 1;
	pthread_mutex_unlock(&table->state_lock);
	return (true);
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
		usleep(200);
	}
	return (NULL);
}
