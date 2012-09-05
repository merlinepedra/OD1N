#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>

#include "html_entities.h" 

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

char *rand_str(char *dst, int size);
int char_type_counter(char *string,char type);
void chomp(char * str);
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);
char *payload_injector(char * ptr,char * payload,int counter);
int WriteFile(char *file,char *str);
void spider(void *arg,char *line);
void scan(void *arg);
