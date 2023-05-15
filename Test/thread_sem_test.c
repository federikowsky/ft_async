#include "../libft/libft.h"


int i;
ft_async_sem_t sem;

int print(ft_async_t *coro, int id)
{
    ft_async_start(coro);

    while (i < 5000)
    {
        ft_asem_await(&sem);
        i++;
        printf("coro %d = %p incremented i --> %d\n", id, coro, i);
        ft_asem_signal(&sem);
        usleep(1000);
        ft_yield;
    }

    ft_async_end;
}

void coro(int id)
{
    ft_async_run(print, id);
}

int main()
{
    pthread_t t1, t2, t3;

    ft_asem_init(&sem, 1);

    WALLTIME(
        pthread_create(&t1, NULL, coro, 1);
        pthread_create(&t2, NULL, coro, 2);
        pthread_create(&t2, NULL, coro, 3);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);
    );

    return 0;
}