/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefilipp <fefilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 14:03:24 by fefilipp          #+#    #+#             */
/*   Updated: 2023/05/15 20:52:44 by fefilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef LIBFT_H
#define LIBFT_H

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <stdatomic.h>

#include "async.h"
#include "async_sem.h"
#include "threadpool.h"

#define ANSI_ORANGE "\x1b[38;5;208m"
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_RESET "\x1b[0m"

#define setxy(x, y) printf("\033[%d;%dH", (x), (y))

#define WALLTIME(body)                                                                        \
    do                                                                                        \
    {                                                                                         \
        struct timeval start, end;                                                            \
        double time_taken;                                                                    \
        gettimeofday(&start, NULL);                                                           \
        body                                                                                  \
        gettimeofday(&end, NULL);                                                             \
        printf(ANSI_MAGENTA "%s took:\n" ANSI_RESET, #body);                                  \
        time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec - start.tv_usec / 1e6;     \
        printf(ANSI_GREEN "\t%3.10f seconds\n" ANSI_RESET, time_taken);                       \
        time_taken = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec); \
        printf(ANSI_GREEN "\t%3.10f micro seconds\n" ANSI_RESET, time_taken);                 \
    } while (0)

#endif