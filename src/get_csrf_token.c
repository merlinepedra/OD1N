#include "get_csrf_token.h"
#include "mem_ops.h"


char *
parse_token(char *str)
{
	int x = 0;
	char *parse = xmalloc(sizeof(char)*1024);	
    	memset(parse,0,1023); // i dont use last byte because crash my compiler, hardening use last byte to canary
	
		while ((*str!='"') && (*str!='\'') && (*str!='\n')&&(*str!='\0'))
		{
			if(x<=1023)
				parse[x]=*str;
			str++;
			x++;
		}

	if(parse)
		return parse;
	else
		return " ";

}

/*

THis function get token, to use custom request to bypass anti csrf 

*/
char *
get_anti_csrf_token(char *url,char *param, char *agent)
{
	CURL *curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;
	char *parse = xmalloc(sizeof(char)*1024);
	bool ValidToken = false;

	chunk.memory = xmalloc(1);  
  	chunk.size = 0;    

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, agent);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1 );
	curl_easy_setopt(curl_handle,CURLOPT_SSL_VERIFYHOST,0); 
	curl_easy_setopt(curl_handle,CURLOPT_SSL_VERIFYPEER,0);
 
	res = curl_easy_perform(curl_handle);

	if(res != CURLE_OK) 
	{
 		fprintf(stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));
 	} else {
    	
		memset(parse,0,1023);
		char *line = strtok(chunk.memory, "\n");

		while (line!=NULL)
		{
// read line per line
			if (strstr(line,param))
			{
				char *ptr = line;
				deadspace(ptr);	

					while (*ptr!='\n' && *ptr!='\0' && ValidToken==false)
					{
						if (ptr[0]=='v' && ptr[1]=='a' && ptr[2]=='l' && ptr[3]=='u' && ptr[4]=='e')
						{
							ptr += 7;
							ValidToken = true;
							parse=parse_token(ptr);
								
						}
						ptr++;
					
					}	
			}

			line = strtok(NULL, "\n");
		}	
	

  	}

  	curl_easy_cleanup(curl_handle);
  	free(chunk.memory);
  	curl_global_cleanup();

  	if (parse)
  		return parse;
 	else
		return "error";
}

