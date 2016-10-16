#include "get_csrf_token.h"
#include "mem_ops.h"

/*

THis function get token, to use custom request to bypass anti csrf 

*/
char *get_anti_csrf_token(char *url,char *param, char *agent)
{
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;
  char *parse=xmalloc(sizeof(char)*256);

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

  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    	char *line;
    	memset(parse,0,255);
    	line=strtok(chunk.memory, "\n");

	while(line!=NULL)
	{
		if(strstr(line,param))
		{
			char *ptr=deadspace(line);	
			int x=0;

			if(!strlen(parse))
				while(*ptr!='\0' && x != 255)
				{
					if(ptr[0]=='v' && ptr[1]=='a' && ptr[2]=='l' && ptr[3]=='u' && ptr[4]=='e')
					{
						ptr+=7;
	
						while(*ptr!='"' || *ptr!='\'')
						{
							
							parse[x]=*ptr;
							ptr++;
							x++;
						}
					}
					ptr++;
				
				}	
		}
		line=strtok(NULL, "\n");
	}	
	

  }



  curl_easy_cleanup(curl_handle);
  free(chunk.memory);
  curl_global_cleanup();
  
  return parse;
}

