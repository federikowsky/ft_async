#include "../libft/libft.h"


#define NUM_ITEMS 32
#define BUFSIZE 8

static int buffer[BUFSIZE];
static int bufptr;
ft_async_sem_t full, empty;


static void
add_to_buffer(int item)
{
	printf(ANSI_RED "PROD --> Item %d added to buffer at place %d\n"ANSI_RESET, item, bufptr);
	buffer[bufptr] = item;
	bufptr = (bufptr + 1) % BUFSIZE;
}

static int
get_from_buffer(void)
{
	int item;
	item = buffer[bufptr];
	printf(ANSI_YELLOW "CONS --> Item %d retrieved from buffer at place %d\n" ANSI_RESET,
		item, bufptr);
	bufptr = (bufptr + 1) % BUFSIZE;
	return item;
}

static int
produce_item(void)
{
	static int item = 0;
	printf(ANSI_GREEN "PROD --> Item %d produced\n" ANSI_RESET, item);
	return item++;
}

static void
consume_item(int item)
{
	printf(ANSI_MAGENTA "CONS --> Item %d consumed\n" ANSI_RESET, item);
}

static async_status
producer(struct async *pt)
{
	static int produced;

	ft_async_start(pt);

	for (produced = 0; produced < NUM_ITEMS; ++produced) {

		ft_asem_await(&full);

		add_to_buffer(produce_item());

		ft_asem_signal(&empty);
	}

	ft_async_end;
}

static async_status
consumer(struct async *pt)
{
	static int consumed;

    ft_with_async(pt,
    
        for (consumed = 0; consumed < NUM_ITEMS; ++consumed) {

            ft_asem_await(&empty);

            consume_item(get_from_buffer());

            ft_asem_signal(&full);
        }
        
    );

}

int run(ft_async_t * main)
{
    ft_async_start(main);

    ft_async_t a = ft_async_new();
    ft_async_t b = ft_async_new();

	ft_asem_init(&empty, 0);
	ft_asem_init(&full, BUFSIZE);

    ft_await(producer(&a) & consumer(&b));

    printf("coro a terminata: %d\n", ft_async_exited(&a));
    printf("coro b terminata: %d\n", ft_async_exited(&b));

    ft_async_end;


}

int main(int argc, char **argv)
{   
    ft_async_t main = ft_async_new();

    while (!(ft_async_exited(&main)))
        run(&main);

    printf("coro main terminata: %d\n", ft_async_exited(&main));

    return 0;
}