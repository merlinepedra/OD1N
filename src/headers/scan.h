#ifndef SCAN_H__
#define SCAN_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <alloca.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <limits.h>
#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>

//#include "mem_ops.h"
#include "string_ops.h"
#include "file_ops.h"
#include "spider.h"
#include "opt_extract.h"
#include <pthread.h>
#include "thpool.h"

void scan(void);

#endif
