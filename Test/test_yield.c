#include "../libft/libft.h"


static async_status
test(ft_async_t *as, int x, char c)
{
    static int i = 0;

    ft_with_async(as,
        while (i < 20) {
            // printf("valore i = %d\n", i);
            printf("valore i = %d x = %d c = %c\n", i, x, c);
            ft_yield_val(i++);
        });
}


int main()
{
    // ft_async_run(test);
    ft_async_run(test, 7, 'a');
}
