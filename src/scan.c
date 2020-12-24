#include "scan.h"

void scan(void)
{
	char *tmp_list = NULL, *delim = "\n", *ptr_line = NULL;
        long int total_requests = 0;	
	int threadss = 2, timeout = 3;

	if (param.threads!=NULL)
		threadss = (int)strtol(param.threads,(char **)NULL,10);

	if (param.timeout!=NULL)
		timeout = (int)strtol(param.timeout,(char **)NULL,10);

	if (timeout<1)
		timeout = 3;

	printf("Threads per request: %d\n Timeout seconds per threads: %d\n",threadss,timeout);
	puts("start...");


	tmp_list = xstrndup(param.buffer_payloads,strlen(param.buffer_payloads)); 
	ptr_line = strtok(tmp_list, delim);


	prepare_datatable();
	threadpool thpool = thpool_init(threadss);
	int inter = 0;

	while (ptr_line != NULL)
	{
		if (total_requests<LONG_MAX)
			total_requests++;

		void *tmp_str = (void *)ptr_line;
		thpool_add_work(thpool, spider, tmp_str);

		if(inter == threadss)
		{
			thpool_wait(thpool);			
			inter = 0;
		}

		ptr_line = strtok(NULL, delim);
		inter++;
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

	exit(0);
}
