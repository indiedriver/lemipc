/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/09 18:31:40 by amathias          #+#    #+#             */
/*   Updated: 2018/01/04 15:30:09 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_H
# define LEMIPC_H
# define BOARD_SIZE 16
# define SHARED_BOARD "/lemipc_board"
# define SEM_BOARD "/lemipc_board_sem"

# include <sys/stat.h>
# include <sys/msg.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/mman.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <assert.h>
# include <signal.h>
# include <math.h>
# include <time.h>
# include <poll.h>
# include "libft.h"

typedef struct	s_team
{
	char		*name;
}				t_team;

typedef struct	s_shared
{
	int			player_counter;
	int			player_ready;
	int			started;
	int			board[BOARD_SIZE][BOARD_SIZE];
}				t_shared;

typedef struct	s_pos
{
	int			x;
	int			y;
}				t_pos;

typedef	struct	s_env
{
	sem_t		*sem_board;
	t_shared	*shared;
	int			msq_target;
	int			msq_ready;
	int			team_id;
	t_pos		pos;
	t_pos		target;
}				t_env;

typedef	struct	s_msg_target
{
	long		mtype;
	t_pos		target;
}				t_msg_target;

typedef	struct	s_msg_ready
{
	long		mtype;
	int			ready;
}				t_msg_ready;

t_env			g_env;

typedef	struct	s_lpos
{
	t_pos		pos;
	int			counter;
	void		*next;
}				t_lpos;

void			disconnect_player(t_env *env);
void			disconnect_waiting_player(t_env *env);

void			init_shared_memory(t_env *env);
void			init_shared_memory_graph(t_env *env);
void			delete_shared_memory(t_env *env);

int				is_empty(t_env *env);
int				is_full(t_env *env);

void			game_display(t_env *env);
void			game_loop(t_env *env);

int				place_player(t_env *env);
int				is_pos_free(t_env *env, int x, int y);
int				inbound(int x, int y);
int				is_trapped(t_env *env);
int				num_of_player_around(t_env *env, int team_num);

void			init_msqs(t_env *env);
void			init_msqs_graph(t_env *env);
void			delete_msqs(t_env *env);

void			receive_loop(t_env *env);

void			broadcast_target(t_env *env, t_pos pos);
void			broadcast_ready(t_env *env);
void			update_target(t_env *env);

void			perr_exit(char *msg);
void			err_exit(char *msg);
void			sig_handler(int signum);

t_pos			find_path(t_env *env, t_pos	start, t_pos target);
void			push_back_remaining(t_lpos **main_list, t_lpos **adja_list);
t_lpos			*get_adjacent_pos_list(t_env *env, t_pos pos, int counter);

t_pos			get_pos(int x, int y);
int				pos_equal(t_pos	pos1, t_pos pos2);
float			get_distance(t_pos pos1, t_pos pos2);
int				are_adjacent(t_pos pos1, t_pos pos2);
void			append_pos(t_lpos **lpos, t_lpos *toadd);
void			push_back_pos(t_lpos **lpos, t_lpos *toadd);
t_lpos			*new_pos(t_pos pos, int counter);
void			remove_pos(t_lpos **lpos, t_pos pos);
void			clear_poslist(t_lpos **begin);

void			reset_graph(t_env *env);
#endif
