/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefilipp <fefilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:16:55 by fefilipp          #+#    #+#             */
/*   Updated: 2023/05/15 20:55:28 by fefilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file threadpool.c
 * @author fefilipp (fefilipp@student.42.fr)
 * @brief A very simple thread pool implementation using pthreads.
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2023
 * 
 */

#include "libft/libft.h"

/* Definizione della coda */
TAILQ_HEAD(task_queue, task);
struct task_queue queue = TAILQ_HEAD_INITIALIZER(queue);

/**
 * @brief function to submit task to thread pool
 *
 * @param pool
 * @param function
 * @param arg
 */
void ft_thpool_submit(threadpool_t *pool, async_status (*coro)(ft_async_t *as, void *arg), void *arg)
{
    task_t *task = malloc(sizeof(task_t));
    task->coro = coro;
    task->arg = arg;
    task->as = ft_async_new();
    pthread_mutex_lock(&pool->mutex);
    TAILQ_INSERT_TAIL(&queue, task, entries);
    pthread_mutex_unlock(&pool->mutex);
    pthread_cond_signal(&pool->condition);
}

/**
 * @brief function to shutdown thread pool.
 *
 * @param pool
 */
void ft_thpool_shutdown(threadpool_t *pool)
{
    while(atomic_load(&pool->active))
        ;
    atomic_store(&pool->running, false);
    pthread_cond_broadcast(&pool->condition);
    for (size_t i = 0; i < pool->num_threads; ++i)
    {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->condition);
}

/**
 * @brief function to run task from thread pool when data is available
 *
 * @param arg
 * @return void*
 */
static void *ft_thpool_schedule(void *arg)
{
    threadpool_t *pool = arg;
    while (atomic_load(&pool->running) && atomic_load(&pool->active))
    {
        pthread_mutex_lock(&pool->mutex);
        while (TAILQ_EMPTY(&queue) && atomic_load(&pool->running))
            pthread_cond_wait(&pool->condition, &pool->mutex);
        if (!TAILQ_EMPTY(&queue))
        {
            task_t *task = TAILQ_FIRST(&queue);
            TAILQ_REMOVE(&queue, task, entries);
            pthread_mutex_unlock(&pool->mutex);
            task->coro(&task->as, task->arg);
            if (!(ft_async_exited(&task->as)))
            {
                pthread_mutex_lock(&pool->mutex);
                TAILQ_INSERT_TAIL(&queue, task, entries);
                pthread_mutex_unlock(&pool->mutex);
            }
            else
            {
                free(task);
                atomic_fetch_sub(&pool->active, 1);
            }
        }
        else
            pthread_mutex_unlock(&pool->mutex);
    }
    return NULL;
}

/**
 * @brief function to initialize thread pool
 *
 * @param pool
 * @param num_threads
 * 
 */
void ft_thpool_init(threadpool_t *pool, size_t num_threads)
{
    assert(num_threads > 0);
    atomic_init(&pool->running, true);
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->condition, NULL);
    pool->threads = calloc(num_threads, sizeof(pthread_t));
    pool->num_threads = 0;
    atomic_init(&pool->active, num_threads);
    for (size_t i = 0; i < num_threads; ++i)
    {
        pthread_create(&pool->threads[i], NULL, ft_thpool_schedule, pool);
        pthread_detach(pool->threads[i]);
    }
}
