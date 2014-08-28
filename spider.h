#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <alloca.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>


#include "mem_ops.h"
#include "html_entities.h" 
#include "string_ops.h"
#include "file_ops.h"

// colors macro
#define RED "\033[22;31m"
#define YELLOW "\033[01;33m"
#define CYAN "\033[22;36m"
#define GREEN "\033[22;32m"
#define LAST "\033[0m"
 
void spider(void *arg,char *line,char * pathtable);
void scan(void *arg);
