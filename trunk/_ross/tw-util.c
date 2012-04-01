#include <ross.h>

void
tw_printf(const char *file, int line, const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	fprintf(stdout, "%s:%i: ", file, line);
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	fflush(stdout);
	va_end(ap);
}

void
tw_error(const char *file, int line, const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	fprintf(stdout, "node: %d: error: %s:%i: ", g_tw_mynode, file, line);
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	fflush(stdout);
	fflush(stdout);
	va_end(ap);

	tw_net_abort();
}

void
tw_exit(int rv)
{
	tw_net_stop();
	exit(rv);
}

struct mem_pool
{
	struct mem_pool *next_pool;
	char *next_free;
	char *end_free;
}__attribute__((aligned(8)));

static struct mem_pool *main_pool;

//static const size_t pool_size = 512 * 1024 - sizeof(struct mem_pool);
static const size_t pool_size = (512 * 1024) - 32;
static const size_t pool_align = max(sizeof(double),sizeof(void*));
static size_t total_allocated;
static unsigned malloc_calls;
static void* my_malloc(size_t len);

void
tw_calloc_stats(
	size_t *bytes_alloc,
	size_t *bytes_wasted)
{
	struct mem_pool *p;

	*bytes_alloc = total_allocated;
	*bytes_wasted = malloc_calls * (sizeof(void*) + sizeof(size_t));

	for (p = main_pool; p; p = p->next_pool)
		*bytes_wasted += p->end_free - p->next_free;
}

static void*
pool_alloc(size_t len)
{
	struct mem_pool *p;
	void *r;

	for (p = main_pool; p; p = p->next_pool)
		if ((p->end_free - p->next_free >= len))
			break;

	if (!p) {
		if (len >= pool_size) {
			r = my_malloc(len);
			goto ret;
		}

		p = my_malloc(pool_size + 32);
		if (!p) {
			r = NULL;
			goto ret;
		}

		p->next_pool = main_pool;
		//p->next_free = (char*)(p + 1);
                p->next_free = (char *)((size_t)32 + (size_t)p);
		if( 7 & (size_t)(p->next_free) )
		    printf("pool_alloc: WARNING found pool start address (%p) NOT 8 byte aligned\n", p->next_free);
		p->end_free = p->next_free + pool_size;
		main_pool = p;
	}

	r = p->next_free;
	p->next_free += len;

	if( 7 & (size_t)r || 7 & (size_t)(p->next_free) )
	    printf("pool_alloc: WARNING found return ptr (%p) or next_free (%p) NOT 8 bytes aligned\n", r, p->next_free );

ret:
	if (r)
		total_allocated += len;
	return r;
}

void*
tw_calloc(
	const char *file,
	int line,
	const char *for_who,
	size_t e_sz,
	size_t n)
{
	void *r;

	if(e_sz & (pool_align - 1))
	{
	    e_sz += pool_align - (e_sz & (pool_align - 1));
	    // printf("%s:%d:%s: realigned size to %d \n", file, line, for_who, e_sz );
	}

	e_sz *= n;
	if (!e_sz)
		return NULL;

	r = pool_alloc(e_sz);
	if (!r)
		tw_error(
			file, line,
			"Cannot allocate %lu bytes for %u %s"
			" (need total of %lu KiB)",
			(unsigned long)e_sz,
			n,
			for_who,
			(unsigned long)((total_allocated + e_sz) / 1024));
	memset(r, 0, e_sz);
	return r;
}

#undef malloc
static void*
my_malloc(size_t len)
{
	malloc_calls++;
	return malloc(len);
}

#undef realloc
void*
tw_unsafe_realloc(
	const char *file,
	int line,
	const char *for_who,
	void *addr,
	size_t len)
{
	malloc_calls++;
	total_allocated += len;
	addr = realloc(addr, len);
	if (!addr)
		tw_error(
			file, line,
			"Cannot allocate %lu bytes for %s",
			(unsigned long)len,
			for_who);
	return addr;
}
