/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   async.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefilipp <fefilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 23:31:57 by fefilipp          #+#    #+#             */
/*   Updated: 2023/05/15 20:47:34 by fefilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef ASYNC_H
#define ASYNC_H

#include "libft.h"

/**
 * @file ft_async.h
 * @author fefilipp (fefilipp@student.42.fr)
 * @brief
 * @version 0.1
 * @date 2023-03-02
 *
 * @copyright Copyright (c) 2023
 *
 */

typedef enum ASYNC_STATUS
{
	ASYNC_INIT = 0,
	ASYNC_CONT = 0,
	ASYNC_DONE = 1,
	ASYNC_ERR = -1
} async_status;

/**
 * @brief Declare the async state
 *
 */

typedef struct async
{
	int _async_status;
} ft_async_t;

/**
 * @brief Get the number of arguments passed to the macro (GCC, Clang)
 *
 */
#if defined(__GNUC__) || defined(__clang__)
// Supports 0-10 arguments
#define VA_NARGS_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
// ## deletes preceding comma if _VA_ARGS__ is empty (GCC, Clang)
#define VA_NARGS(...) VA_NARGS_IMPL(_, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#else
// Supports 1-10 arguments
#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#endif

/**
 * Initialize the async coro state
 *
 * @return async
 */

#define ft_async_new() ((ft_async_t){ \
	._async_status = ASYNC_INIT})

/**
 * Get the async status
 *
 * @param async
 * @return ASYNC_STATUS
 */
#define ft_get_async_status(a) ((a)->_async_status)

/**
 * Set the async status
 *
 * @param async
 * @param status
 */
#define ft_set_async_status(a, s) (a)->_async_status = s

/**
 * Check if the async is done
 *
 * @param async
 * @return true or false
 */
#define ft_async_exited(a) (a)->_async_status == ASYNC_DONE

/**
 * Mark the start of an async subroutine
 *
 * @param async
 */
#define ft_async_start(a)                    \
	int *async_status = &(a)->_async_status; \
	switch (*async_status)                   \
	{                                        \
	case ASYNC_INIT:

/**
 * Wait for a condition to be true before continuing execution of the async subroutine
 *
 * @param cond
 */
#define ft_await(cond)        \
	*async_status = __LINE__; \
	case __LINE__:            \
		if (!(cond))          \
		return ASYNC_CONT

/**
 * Yield the async subroutine execution to the caller
 *
 * @param x param to return
 */
#define ft_yield_val(value)   \
	*async_status = __LINE__; \
	return value;             \
	case __LINE__:

/**
 * Yield the async subroutine execution to the caller without returning a value
 *
 */
#define ft_yield              \
	*async_status = __LINE__; \
	return ASYNC_CONT;        \
	case __LINE__:

#define ft_breakpoint(pos) \
	*async_status = pos;   \
	case pos:

/**
 * Mark the end of an async subroutine
 *
 * @param async
 */
#define ft_async_end            \
	*async_status = ASYNC_DONE; \
	case ASYNC_DONE:            \
		return ASYNC_DONE;      \
		}                       \
		return 0

/**
 * Resume a running async computation and check for completion
 *
 * Calling the function itself will return true if the async call is complete,
 * or false if it's still in progress.
 * @param f The async procedure
 * @param state The async procedure state
 * @return status of the async
 */
#define ft_async_call(f, state, ...) (ft_async_exited(state) || (f)(__VA_ARGS__))

/**
 * This macro is a convenience macro that combines the ft_async_start()
 * and ft_async_end macros into a single macro. It is useful when
 * you want to write more clear code.
 *
 * @param async
 * @param code
 *
 */
#define ft_with_async(a, body) \
	ft_async_start(a)          \
	body	                   \
	ft_async_end

/**
 * @brief Run an async subroutine passing it the given arguments and wait for it to complete
 * it will enclose the async subroutine in a ft_with_async statement and call function f with the given arguments
 * until the async subroutine is done or an error occurs
 *
 * @param f the async function to execute
 * @param ... the arguments to pass to the async function, even 0
 *
 * the call f(&a, ##__VA_ARGS__);
 * it's euqivalent to: f(&a __VA_OPT__(,) __VA_ARGS__);
 * both delete the comma if __VA_ARGS__ is empty.
 * second one introduced in C++20
 */
#define ft_async_run(f, ...)       \
	ft_async_t a = ft_async_new(); \
	do                             \
	{                              \
		f(&a, ##__VA_ARGS__);      \
	} while (!(ft_async_exited(&a)))

#endif
