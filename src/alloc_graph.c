/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_graph.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/03 14:00:21 by amathias          #+#    #+#             */
/*   Updated: 2018/01/04 14:58:34 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	init_semaphores_graph(t_env *env)
{
	while ((env->sem_board = sem_open(SEM_BOARD, 0)) == SEM_FAILED)
	{
		system("clear");
		ft_putendl("Semaphore unavailable");
		sleep(1);
	}
}

void	init_shared_memory_graph(t_env *env)
{
	int			shm_fd;
	t_shared	*shared;

	init_semaphores_graph(env);
	while ((shm_fd = shm_open(SHARED_BOARD, O_RDWR)) == -1)
	{
		system("clear");
		ft_putendl("Shared memory unavailable");
		sleep(1);
	}
	ftruncate(shm_fd, sizeof(t_shared));
	if ((shared = mmap(NULL, sizeof(t_shared), PROT_READ,
			MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
		perr_exit("mmap");
	env->shared = shared;
}

void	init_msqs_graph(t_env *env)
{
	key_t	key;

	if (access("/tmp/lempipc", F_OK) != 0)
		creat("/tmp/lemipc", 0666);
	key = ftok("/tmp/lemipc", 'R');
	while ((env->msq_ready = msgget(key, 0666)) == -1)
	{
		system("clear");
		ft_putendl("Shared memory unavailable");
		sleep(1);
	}
}
