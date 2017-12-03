#include "scan.h"
#define TABLE "tables/output_array.txt"
#define TEMPLATE "template.conf"
#define TEMPLATE2 "hammer1.conf"
#define TEMPLATE3 "hammer2.conf"

void scan(void *arguments)
{
	FILE *fp=NULL;

	char **arg = (char **)arguments;
	char *pathtable=NULL,*pathhammer=NULL,*view=NULL,*template2=NULL,*template3=NULL;
	char line[2048]; 

	pid_t pid;

	if(arg[11]!=NULL)
		threadss=(int)strtol(arg[11],(char **)NULL,10);
	int old_thread=threadss,status=-1,timeout=0,num1=0,num2=0;
	long int total_requests=0;

	if(arg[8]!=NULL)
		timeout=(int)strtol(arg[8],(char **)NULL,10);

	if(timeout<1)
		timeout=3;


	printf("Threads per request: %d\n Timeout seconds per threads: %d\n",threadss,timeout);

 // write tables rows at datatables file to load
	pathtable=xmalloc(64*sizeof(char));
	memset(pathtable,0, 63*sizeof(char));
	strlcat(pathtable,"tables/",8);
	strlcat(pathtable,arg[5],28);
	strlcat(pathtable,".txt",33);
	fp = fopen(arg[1], "r");

 	if ( !fp )
	{ 
		DEBUG("error to open Payload list"); 
		exit(1);
	}

        num1=FileSize(TEMPLATE2);
        num2=FileSize(TEMPLATE3); 

	size_t total=num1+num2+512;

	view=xmallocarray(total,sizeof(char));
	memset(view,0,(total-1)*sizeof(char));

      //  template2=xmalloc(sizeof(char)*num1+1);
        template2=readLine(TEMPLATE2);

	strlcat(view,template2,num1+1);
	strlcat(view,"\"sAjaxSource\": \"",num1+24);
	strlcat(view,arg[5],num1+64);
	strlcat(view,".txt\" \n",num1+112);

      //  template3=xmalloc(sizeof(char)*num2+1);
        template3=readLine(TEMPLATE3);

	strlcat(view,template3,num2+num1+112);

	pathhammer=xmalloc(64*sizeof(char));
	memset(pathhammer,0,63*sizeof(char));

	strlcat(pathhammer,"tables/hammer_",15);
	strlcat(pathhammer,arg[5],32);
	strlcat(pathhammer,".html",38);

	WriteFile(pathhammer,view);
	WriteFile(pathtable,"{ \"aaData\": [ \n");

	puts("start...");

// TODO add my thread pool library
	while ( fgets(line,2047,fp) != NULL ) 
	{
		threadss--;
		curl_global_cleanup();
		pid=fork();
		curl_global_init(CURL_GLOBAL_ALL);


		if(total_requests<LONG_MAX)
			total_requests++;


		if(pid==-1)
		{
			DEBUG("error in fork()");
			exit(1);
		}

		if(pid==0)
		{

//			curl_global_init(CURL_GLOBAL_ALL);
			threadss--;
			spider(arguments,line,pathtable);


//			curl_global_cleanup();
			exit(0);
		}
		
		if(threadss<=0)
		{
				while(1)
				{
					pid=wait(&status);	

						if (errno == ECHILD)
						{
							break;
						}
						
				}
				sleep(timeout);
				threadss=old_thread;							
		}

  	}

	threadss=0;


	while(1)
	{
		pid=wait(&status);	

		if (errno == ECHILD)
		{
			break;
		}
	}				

	sleep(timeout+5);
	
// end of json file
	WriteFile(pathtable," [\"\",\"\",\"\",\"\",\"\"] \n ] }");

	puts(RED);
	fprintf(stdout,"End scan \n look the file %s\n Total Requests %ld\n Path table: %s\n",pathhammer, total_requests,pathtable);
	puts(LAST);

// clear all
	memset(pathtable,0,(strnlen(pathtable,64)-1)*sizeof(char));
	memset(pathhammer,0,(strnlen(pathhammer,64)-1)*sizeof(char));
	memset(view,0,(strnlen(view,total-1)-1)*sizeof(char));

	XFREE(pathtable);
	XFREE(pathhammer);
	XFREE(view);
	XFREE(template2);
	XFREE(template3);

	if( fclose(fp) == EOF )
	{
		DEBUG("Error in close()");
		exit(1);
	}

	fp=NULL;

	exit(0);
}
