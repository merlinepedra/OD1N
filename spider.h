#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>

#include "html_entities.h" 
#include "string_ops.h"
#include "file_ops.h"

// colors macro
#define RED "\033[22;31m"
#define YELLOW "\033[01;33m"
#define CYAN "\033[22;36m"
#define GREEN "\033[22;32m"
#define LAST "\033[0m"

 
struct MemoryStruct {
  char *memory;
  size_t size;
};

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);
void spider(void *arg,char *line,char * pathtable);
void scan(void *arg);
