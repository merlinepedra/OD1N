#include <stdio.h>	
#include <sys/types.h>
#include <string.h>		
#include <stdlib.h>
 
struct MemoryStruct {
  char *memory;
  size_t size;
};

static void *xmalloc_fatal(size_t size) 
{
	if ( size == 0 ) 
		return NULL;

	fprintf(stderr, "Out of memory.");

	exit(1);
}

void *xmalloc (size_t size) 
{
	void *ptr = malloc (size);

	if (ptr == NULL) 
		return xmalloc_fatal(size);

	return ptr;
}

void *xcalloc (size_t mem, size_t size) 
{
	void *ptr = calloc (mem, size);

	if (ptr == NULL) 
		return xmalloc_fatal(mem*size);

	return ptr;
}

void *xrealloc (void *ptr, size_t size) 
{
	void *p = realloc (ptr, size);

	if (p == NULL) 
		return xmalloc_fatal(size);

	return p;
}

void xfree(void *ptr) 
{
	return free(ptr);
}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) 
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	mem->memory = xrealloc(mem->memory, mem->size + realsize + 1);

	if( mem->memory ) 
	{
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
               
	}

	return realsize;
}

