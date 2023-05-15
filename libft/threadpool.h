/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefilipp <fefilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 15:16:57 by fefilipp          #+#    #+#             */
/*   Updated: 2023/03/07 12:21:09 by fefilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file threadpool.h
 * @author fefilipp (efilipp@student.42.fr)
 * @brief
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "libft.h"

typedef struct task {
    async_status (*coro)(ft_async_t *as, void* arg);
    void* arg;
    TAILQ_ENTRY(task) entries;
    ft_async_t as;
} task_t;

/* Definizione del threadpool */
typedef struct threadpool {
    pthread_t* threads;
    size_t num_threads;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    volatile atomic_bool running;
    volatile atomic_uint_fast32_t active;
} threadpool_t;

/**
 * @brief function to call function in thread pool
 * if function returns 0 it will be called again, so don't be freed but add to queue again
 * 
 * @param arg 
 * @return void* 
 */
static void* ft_thpool_schedule(void* arg);

/**
 * @brief function to initialize thread pool 
 * 
 * @param pool 
 * @param num_threads 
 */
void ft_thpool_init(threadpool_t* pool, size_t num_threads);

/**
 * @brief function to submit task to thread pool 
 * 
 * @param pool 
 * @param function 
 * @param arg 
 */
void ft_thpool_submit(threadpool_t *pool, async_status (*coro)(ft_async_t *as, void* arg), void *arg);

/**
 * @brief function to shutdown thread pool.
 * When the queue is empty, the threads will exit and this function will be call automatically.
 * 
 * @param pool 
 */
void ft_thpool_shutdown(threadpool_t* pool);

#endif
