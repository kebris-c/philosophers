/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 20:51:44 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/03 21:06:58 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	dead_checker(t_table *table, int i)
{
	long		ts;
	long		last_meal;

	pthread_mutex_lock(&table->state_lock);
	ts = get_timestamp(table);
	last_meal = table->philos[i].last_meal;
	if (ts - last_meal > table->time_to_die)
	{
		pthread_mutex_lock(&table->print_lock);
		printf("%ld %d died\n", ts, table->philos[i].id);
		pthread_mutex_unlock(&table->print_lock);
		pthread_mutex_lock(&table->print_lock);
		table->finished = 1;
		pthread_mutex_unlock(&table->print_lock);
		return (true);
	}
	pthread_mutex_unlock(&table->state_lock);
	return (false);
}

static bool	food_is_gone(t_table *table)
{
	int			i;
	int			meals;
	long		ts;

	i = 0;
	while (i++ < table->n_philos)
	{
		pthread_mutex_lock(&table->state_lock);
		meals = table->philos[i - 1].meals;
		pthread_mutex_unlock(&table->state_lock);
		if (meals < table->must_eat)
			return (false);
	}
	ts = get_timestamp(table);
	pthread_mutex_lock(&table->print_lock);
	printf("%ld Simulation ended: all philosophers ate %d times\n", \
			ts, table->must_eat);
	pthread_mutex_unlock(&table->print_lock);
	pthread_mutex_lock(&table->print_lock);
	table->finished = 1;
	pthread_mutex_unlock(&table->print_lock);
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_table		*table;
	int			i;

	table = (t_table *)arg;
	while (!table->finished)
	{
		i = 0;
		while (i < table->n_philos)
			if (dead_checker(table, i++))
				return (NULL);
		if (table->must_eat != -1 && food_is_gone(table))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
