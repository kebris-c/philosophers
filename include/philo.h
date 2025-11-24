/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:49:39 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/24 19:36:11 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//	Headers
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <string.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>

//	Typedefs
typedef pthread_mutex_t	t_mutex;
typedef pthread_t		t_thread;
typedef struct timeval	t_timeval;

typedef struct s_table
{
	int				n_philos;
	int				must_eat;
	int				finished;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	t_mutex			*forks;
	t_mutex			print_lock;
	t_mutex			state_lock;
	struct s_philo	*philos;
}	t_table;

typedef struct s_philo
{
	int			id;
	int			meals;
	int			has_left;
	int			has_right;
	long		last_meal;
	t_mutex		*left_fork;
	t_mutex		*right_fork;
	t_table		*table;
	t_thread	thread;
}	t_philo;

typedef struct s_time
{
	t_timeval	timeval;
	long		abs_ms;
	long		rel_ms;
}	t_time;

//	Prototypes
//	  utils0 and utils1 files
int		exit_fail(char *flag, int threads, t_table *table);
int		ft_atoi(const char *str);
int		ft_strcmp(char *s1, char *s2);
void	free_table(t_table *table);
void	ft_putstr_fd(char *str, int std);
void	ft_usleep(long ms, t_table *table);
size_t	ft_strlen(const char *str);
t_time	get_time(t_table *table);
//	  philos_actions file
void	thinking_action(t_philo *philo);
void	pickup_forks(t_philo *philo);
void	eating_action(t_philo *philo);
void	sleeping_action(t_philo *philo);
void	drop_forks(t_philo *philo);
//	  independent files
void	*monitor_routine(void *arg);
void	*philos_routine(void *arg);
long	ft_atol(const char *str);

//	Inlines
static inline int	is_dead(t_philo *philo)
{
	int	finished;

	pthread_mutex_lock(&philo->table->state_lock);
	finished = philo->table->finished;
	pthread_mutex_unlock(&philo->table->state_lock);
	return (finished);
}

static inline void	take_a_fork(t_philo *philo, char *fork)
{
	if (ft_strcmp(fork, (char *)"right") == 0)
	{
		if (is_dead(philo))
			return ;
		pthread_mutex_lock(philo->right_fork);
		philo->has_right = 1;
	}
	else if (ft_strcmp(fork, (char *)"left") == 0)
	{
		if (is_dead(philo))
			return ;
		pthread_mutex_lock(philo->left_fork);
		philo->has_left = 1;
	}
}

static inline void	drop_a_fork(t_philo *philo, char *fork)
{
	if (ft_strcmp(fork, (char *)"right") == 0)
	{
		if (philo->has_right)
			pthread_mutex_unlock(philo->right_fork);
		philo->has_right = 0;
		pthread_mutex_lock(&philo->table->print_lock);
		if (!is_dead(philo))
			printf("%ld\t%d\tdropped a right fork\n", \
				get_time(philo->table).rel_ms, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
	else if (ft_strcmp(fork, (char *)"left") == 0)
	{
		if (philo->has_left)
			pthread_mutex_unlock(philo->left_fork);
		philo->has_left = 0;
		pthread_mutex_lock(&philo->table->print_lock);
		if (!is_dead(philo))
			printf("%ld\t%d\tdropped a left fork\n", \
				get_time(philo->table).rel_ms, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
}

#endif
