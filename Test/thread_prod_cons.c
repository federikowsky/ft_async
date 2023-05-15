#include "../libft/libft.h"


#define NUM_ITEMS 1000
#define BUFSIZE 8

#define MAX_THREADS 4

static int buffer[BUFSIZE];
static int bufptr;
static ft_async_sem_t full, empty, prod, cons;



static void
add_to_buffer(int item, int id)
{
	ft_print(1000, ANSI_RED "PROD#%d --> Item %d added to buffer at place %d\n"ANSI_RESET, id, item, bufptr);
	buffer[bufptr] = item;
	bufptr = (bufptr + 1) % BUFSIZE;
}

static int
get_from_buffer(int id)
{
	int item;
	item = buffer[bufptr];
	ft_print(1000, ANSI_YELLOW "CONS#%d --> Item %d retrieved from buffer at place %d\n" ANSI_RESET, id, item, bufptr);
	bufptr = (bufptr + 1) % BUFSIZE;
	return item;
}

static int
produce_item(int id)
{
	static int item = 0;
	ft_print(1000, ANSI_GREEN "PROD#%d --> Item %d produced\n" ANSI_RESET, id, item);
	return item++;
}

static void
consume_item(int item, int id)
{
	ft_print(1000, ANSI_MAGENTA "CONS#%d --> Item %d consumed\n" ANSI_RESET, id, item);
}


static async_status
producer(struct async *pt, int id)
{
    static int produced;

    ft_async_start(pt);

    for (produced = 0; produced < NUM_ITEMS; ++produced) {

        ft_asem_await(&full);

        ft_asem_await(&prod);
        add_to_buffer(produce_item(id), id);
        ft_asem_signal(&prod);

        ft_asem_signal(&empty);
    }

    ft_async_end;
}

static async_status
consumer(struct async *pt, int id)
{
    static int consumed;

    ft_async_start(pt);

    for (consumed = 0; consumed < NUM_ITEMS; ++consumed) {

        ft_asem_await(&empty);

        consume_item(get_from_buffer(id), id);

        ft_asem_signal(&full);
    }

    ft_async_end;
}


static int
run(int id)
{
    if (id % 2 == 0)
    {
        ft_async_run(producer, id);
    }
    else
    {
        ft_async_run(consumer, id);
    }
    return 0;
}

int main()
{
    ft_asem_init(&full, BUFSIZE);
    ft_asem_init(&empty, 0);
    ft_asem_init(&prod, 1);
    ft_asem_init(&cons, 1);

    WALLTIME(
        pthread_t threads[MAX_THREADS];
        for (size_t i = 0; i < MAX_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, run, i);
        }

        for (size_t i = 0; i < MAX_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }
    );
    return 0;
}