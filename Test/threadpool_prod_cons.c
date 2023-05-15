#include "../libft/libft.h"


#define NUM_ITEMS 1000
#define BUFSIZE 8

#define MAX_THREADS 4

static ft_async_sem_t full, empty, prod, cons;

int buffer[BUFSIZE]; // buffer circolare
int count = 0;       // numero di elementi presenti nel buffer
int in = 0;          // indice di inserimento nel buffer
int out = 0;         // indice di estrazione dal buffer

volatile atomic_int counter = 0;

static void
add_to_buffer(int item)
{
    buffer[in] = item;
    in = (in + 1) % BUFSIZE;
    count++;
    // ft_print(1000, ANSI_RED "PROD[ID: %ld] --> Item %d added to buffer at place %d\n"ANSI_RESET, (long)pthread_self(), item, in);
}

static int
get_from_buffer()
{
    int item = buffer[out];
    out = (out + 1) % BUFSIZE;
    count--;
    // ft_print(1000, ANSI_YELLOW "CONS[ID: %ld] --> Item %d retrieved from buffer at place %d\n" ANSI_RESET, (long)pthread_self(), item, out);
    return item;
}

static int
produce_item(int *item)
{
    *item = atomic_fetch_add(&counter, 1);
    usleep(1000);
    printf(ANSI_GREEN "PROD[ID: %ld] --> Item %d produced\n" ANSI_RESET, (long)pthread_self(), *item);
    return 0;
}

static void
consume_item(int item)
{
    usleep(1000);
    print(ANSI_MAGENTA "CONS[ID: %ld] --> Item %d consumed\n" ANSI_RESET, (long)pthread_self(), item);
}

static async_status
producer(struct async *pt, void *data)
{
    static int produced;

    int item;
    ft_async_start(pt);

    for (produced = 0; produced < NUM_ITEMS; ++produced)
    {

        ft_asem_await(&full);

        ft_asem_await(&prod);
        produce_item(&item);
        add_to_buffer(item);
        ft_asem_signal(&prod);

        ft_asem_signal(&empty);
    }

    ft_async_end;
}

static async_status
consumer(struct async *pt, void *data)
{
    static int consumed;

    int item;
    ft_async_start(pt);

    for (consumed = 0; consumed < NUM_ITEMS; ++consumed)
    {

        ft_asem_await(&empty);

        ft_asem_await(&cons);
        item = get_from_buffer();
        consume_item(item);
        ft_asem_signal(&cons);

        ft_asem_signal(&full);
    }

    ft_async_end;
}

static int
run(threadpool_t *pool)
{
    int id = 0;
    while (id < MAX_THREADS)
    {
        if (id % 2 == 0)
        {
            ft_thpool_submit(pool, &producer, (void *)&id);
        }
        else
        {
            ft_thpool_submit(pool, &consumer, (void *)&id);
        }
        id++;
    }
    return 0;
}

int main()
{
    ft_asem_init(&full, BUFSIZE);
    ft_asem_init(&empty, 0);
    ft_asem_init(&prod, 1);
    ft_asem_init(&cons, 1);

    threadpool_t pool;
    ft_thpool_init(&pool, MAX_THREADS);

    run(&pool);

    WALLTIME(
        ft_thpool_shutdown(&pool););

    return 0;
}