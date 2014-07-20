#include <stdio.h>	
#include <sys/types.h>
#include <string.h>		
#include <stdlib.h>
 
struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct MemoryStruct MemoryStruct;

void *xmalloc (size_t size);
void *xcalloc (size_t mem, size_t size);
void *xrealloc (void *ptr, size_t size);
void xfree(void *ptr);
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);

