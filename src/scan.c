#include "scan.h"

void scan(void)
{
	FILE *fp = NULL;
        long int total_requests = 0;	
	int threadss = 2, timeout = 3;
	char *line = NULL;
	size_t len = 0;

	if (param.threads!=NULL)
		threadss = (int)strtol(param.threads,(char **)NULL,10);

	if (param.timeout!=NULL)
		timeout = (int)strtol(param.timeout,(char **)NULL,10);

	if (timeout<1)
		timeout = 3;

	printf("Threads per request: %d\n Timeout seconds per threads: %d\n",threadss,timeout);
	puts("start...");

	prepare_datatable();
	threadpool thpool = thpool_init(threadss);



	fp = fopen(param.payloads, "r");

 	if (!fp)
	{ 
		DEBUG("error to open Payload list"); 
		exit(0);
	}

	while ( getline(&line,&len,fp) != -1) 
	{
		if (total_requests<LONG_MAX)
			total_requests++;

		thpool_add_work(thpool, spider, (void *)line);

		thpool_wait(thpool);			

  	}

	thpool_destroy(thpool);
	threadss = 0;
	puts("Sleep timeout seconds");
	sleep(timeout);
	end_datatable(param.datatable);

	puts(RED);
	fprintf(stdout,"End scan \n look the file %s\n Total Requests %ld\n Path table: %s\n",param.path_output, total_requests, param.datatable);
	fprintf(stdout,"\nExecute 0d1n_view server \nFind the file html in https://127.0.0.1:40111/tables/hammer_%s.html\n", param.log);
	puts(LAST);

	XFREE(param.buffer_list);
	XFREE(param.buffer_payloads);
	XFREE(param.path_output);
	XFREE(param.datatable);

	if (fclose(fp) == EOF)
	{
		DEBUG("Error in close()");
		exit(0);
	}

	fp = NULL;

	exit(0);
}
