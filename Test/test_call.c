#include "../libft/libft.h"


struct prova {
    int a;
    int b;
    ft_async_t coro;
};

static async_status
my_async_function(struct prova *p)
{
    ft_async_t *coro = &p->coro;
    static int i = 0;
    ft_with_async(coro,
        while (i < 10) {
            printf("%d %d  ", p->a, p->b);
            ft_yield;
            i++;
        }
    );
}


int main()
{
    struct prova p = {
        .a = 1,
        .b = 2,
        .coro = ft_async_new()
    };

    while (!ft_async_call(my_async_function, &(p.coro) ,(void *)&p))
    {
        printf("la funzione asincrona non è ancora completata\n");
    }
    printf("La funzione asincrona è completata");
}