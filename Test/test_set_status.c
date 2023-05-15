#include "../libft/libft.h"


static int
test(ft_async_t *coro)
{
    static int i = 0;
    ft_with_async(coro,
        while (i < 10) {
            printf("i = %d\n", i);
            ft_yield_val(i++);
        }
    );
}


int main()
{
    int i;
    ft_async_t a = ft_async_new();
    do
    {
        i = test(&a);
        if (i == 5)
            ft_set_async_status(&a, ASYNC_DONE);
    } while (!(ft_async_exited(&a)));
}
