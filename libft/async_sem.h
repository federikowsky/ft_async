/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   async_sem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefilipp <fefilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 00:37:21 by fefilipp          #+#    #+#             */
/*   Updated: 2023/05/15 20:47:39 by fefilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ft_async_sem.h
 * @author fefilipp (fefilipp@student.42.fr)
 * @brief
 * @version 0.1
 * @date 2023-03-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#ifndef ASYNC_SEM_H
#define ASYNC_SEM_H

#include "libft.h"

typedef struct async_sem
{
	atomic_int value;
} ft_async_sem_t;

/**
 * @brief Function for initialize a semaphore with a value
 *
 * @param sem
 * @param value
 *
 */

#define ft_asem_init(sem, v) atomic_init(&((sem)->value), v)

// (sem)->value = v

/**
 * Wait for a semaphore
 *
 * This macro carries out the "wait" operation on the semaphore. The
 * wait operation causes the protothread to block while the counter is
 * zero. When the counter reaches a value larger than zero, the
 * protothread will continue.
 *
 * @param sem
 */

#define ft_asem_await(sem)                                                                      \
	do                                                                                          \
	{                                                                                           \
		*async_status = __LINE__;                                                               \
		int value;                                                                              \
	case __LINE__:                                                                              \
		value = atomic_load(&((sem)->value));                                                   \
		if (!(value > 0 && atomic_compare_exchange_strong(&((sem)->value), &value, value - 1))) \
			return ASYNC_CONT;                                                                  \
	} while (0)

// ft_await((sem)->value > 0) --(sem)->value

/**
 * This macro carries out the "signal" operation on the semaphore. The
 * signal operation increments the counter inside the semaphore, which
 * eventually will cause waiting protothreads to continue executing.
 *
 * @param sem
 *
 */

#define ft_asem_signal(sem) atomic_fetch_add(&((sem)->value), 1)
// ++(sem)->value

/**
 * This macro destroys the semaphore. It is not necessary to call this
 * function, but it is good practice to do so.
 *
 * @param sem
 *
 */

#define ft_asem_destroy(sem) (sem)->value = 0

/**
 * This macro returns the current value of the semaphore.
 *
 * @param sem
 * @return int
 *
 */
#define ft_asem_value(sem) atomic_load(&((sem)->value))
//(sem)->value

/**
 * This macro is a convenience macro that combines the ft_asem_await()
 * and ft_asem_signal() macros into a single macro. It is useful when
 * you want to protect a critical section of code with a semaphore.
 *
 * @param sem
 * @param code
 *
 */
#define ft_with_asem(sem, code) \
	ft_asem_await(sem);         \
	code                        \
		ft_asem_signal(sem);

/**
 * @brief This macro is a convenience macro that combines the ft_asem_await()
 * and ft_asem_signal() macros into a single macro. It is useful when
 * you want to protect a critical section of code with a semaphore.
 * This macro is used to protect a critical section of code with a
 * semaphore. The critical section of code is executed in a loop until
 * the protothread exits.
 *
 */
#define ft_asem_loop(sem, coro, code)   \
	ft_async_start(coro);               \
	do                                  \
	{                                   \
		ft_with_asem(sem, code)         \
	} while (!(ft_async_exited(coro))); \
	ft_async_end

#endif
