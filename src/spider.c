#include "spider.h"

void spider(void *pack,char *line,char * pathtable)
{
	struct MemoryStruct chunk;
	bool save_response=false;
	long status=0,length=0;
	int old=0,res=0,counter=0,counter_cookie=0,counter_agent=0,POST=0,timeout=0,debug_host=3; 
	char *make=NULL,*make2=NULL,*make_cookie=NULL,*make_agent=NULL,*tamper=NULL;
	char *token=NULL;
	char **pack_ptr=(char **)pack,**arg = pack_ptr;

	if(arg[12]!=NULL)
		save_response=true;

	if(arg[8]!=NULL)
		timeout=(int)strtol(arg[8],(char **)NULL,10);

// if need get anti-csrf token
	if(arg[0]!=NULL && arg[23]!=NULL)
	{
		token=xmalloc(sizeof(char)*1024);
		memset(token,0,1023);

		if(arg[6]!=NULL)
			token=get_anti_csrf_token(arg[0],arg[23],arg[6]);
		else
			token=get_anti_csrf_token(arg[0],arg[23],"Mozilla/5.0 (0d1n v2.7)");
	}



// payload tamper, get payload of line and make tamper 
	if(arg[20]!=NULL)
	{
		tamper=arg[20];
		line=tamper_choice(tamper,line);
	}

// brute POST/GET/COOKIES/UserAgent
	if(arg[21]==NULL)
	{
		POST=(arg[4]==NULL)?0:1;
		counter=char_type_counter(POST?arg[4]:arg[0],'^');
		counter_cookie=char_type_counter(arg[13]!=NULL?arg[13]:"",'^');
		counter_agent=char_type_counter(arg[19]!=NULL?arg[19]:"",'^');
		old=counter;  

	} else {

		char *file_request=readLine(arg[21]);
		counter=char_type_counter(file_request,'^');
		old=counter;
		XFREE(file_request);
	}
	chomp(line);

// goto to fix signal stop if user do ctrl+c
	try_again:

	while ( old > 0 || counter_cookie > 0  || counter_agent > 0 )
	{

		CURL *curl;  
//		curl_global_init(CURL_GLOBAL_ALL); 

		chunk.memory=NULL; 
		chunk.size = 0;  

		curl_socket_t sockfd; /* socket */
		long sockextr;
		size_t iolen;


		curl = curl_easy_init();
		
// add payload at inputs
		if(arg[21]==NULL) //if custom request  argv mode null
		{
			make2=payload_injector( (POST?arg[4]:arg[0]),line,old);
			if(token)
			{
//printf("OK\n");
		 		make=replace(make2,"{token}",token); // if user pass token to bypass anti-csrf
			} else
				make=strdup(make2);	
//printf("DEBUG PAYLOAD %s\n",make);

			if(arg[13]!=NULL)	
				make_cookie=payload_injector( arg[13],line,counter_cookie);	
	
			if(arg[19]!=NULL)
				make_agent=payload_injector( arg[19],line,counter_agent);

			curl_easy_setopt(curl,  CURLOPT_URL, POST?arg[0]:make);
		} else {
// if is custom request
			char *request_file=readLine(arg[21]);
			make2=payload_injector( request_file,line,old);	
			curl_easy_setopt(curl,  CURLOPT_URL, arg[0]);

			if(token!=NULL)
				make=replace(make2,"{token}",token); // if user pass token to bypass anti-csrf
			else
				make=strdup(make2);

			XFREE(request_file);
		}	
 
		if ( POST )
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, make);
      
		curl_easy_setopt(curl,  CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl,  CURLOPT_WRITEDATA, (void *)&chunk);

// load user agent     
		if ( arg[6]!=NULL )
		{
			curl_easy_setopt(curl,  CURLOPT_USERAGENT, arg[6]);
		} else {
			curl_easy_setopt(curl,  CURLOPT_USERAGENT, "Mozilla/5.0 (0d1n v0.1) ");
		}

// json headers to use JSON

		if(arg[14]!=NULL)
		{
			struct curl_slist *headers = NULL;
			curl_slist_append(headers, arg[14]);
			if(arg[16]!=NULL)
			{
				curl_slist_append(headers, "Accept: application/json");
				curl_slist_append(headers, "Content-Type: application/json");
			}
			curl_easy_setopt(curl,  CURLOPT_HTTPHEADER, headers);
			curl_slist_free_all(headers);
		} else {
			if(arg[16] != NULL)
			{
				struct curl_slist *headers = NULL;

				curl_slist_append(headers, "Accept: application/json");
				curl_slist_append(headers, "Content-Type: application/json");
				curl_easy_setopt(curl,  CURLOPT_HTTPHEADER, headers);
				curl_slist_free_all(headers);
			}
		}
	
//use custom method PUT,DELETE...
		if(arg[15]!=NULL)
		{
			curl_easy_setopt(curl,  CURLOPT_CUSTOMREQUEST, arg[15]);
		}
 
		curl_easy_setopt(curl,  CURLOPT_ENCODING,"gzip,deflate");

// load cookie jar
		if ( arg[3] != NULL )
		{
			curl_easy_setopt(curl,CURLOPT_COOKIEFILE,arg[3]);
			curl_easy_setopt(curl,CURLOPT_COOKIEJAR,arg[3]);
		} else {
			curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"odin_cookiejar.txt");
		}
// LOAD cookie fuzz

		if(arg[13]!=NULL)
		{
			curl_easy_setopt(curl,CURLOPT_COOKIE,make_cookie);
		}


// LOAD UserAgent FUZZ
		if(arg[19]!=NULL)
		{
			curl_easy_setopt(curl,CURLOPT_USERAGENT,make_agent);
		}


		curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
// Load cacert
		if ( arg[7] != NULL ) 
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
			curl_easy_setopt(curl, CURLOPT_CAINFO, arg[7]);
		} else {

			curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0L); 
			curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L); 
		}

		if(timeout) 
			curl_easy_setopt(curl,CURLOPT_TIMEOUT,timeout); 

// load single proxy
		if(arg[17] != NULL)
		{
			curl_easy_setopt(curl, CURLOPT_PROXY, arg[17]);
	//		curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);
		}

// load random proxy in list 
		if(arg[18] != NULL)
		{
			char *randproxy=Random_linefile(arg[18]);
			curl_easy_setopt(curl, CURLOPT_PROXY, randproxy);
	//		curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);
		}

// choice SSL version
		if ( arg[9] != NULL ) 
			curl_easy_setopt(curl,CURLOPT_SSLVERSION,(int)strtol(arg[9],(char **)NULL,10));

		curl_easy_setopt(curl,CURLOPT_VERBOSE,0); 
		curl_easy_setopt(curl,CURLOPT_HEADER,1);  
		
// if use custom request
		if(arg[21]!=NULL)
		{
			curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
		}

		res=curl_easy_perform(curl);
// get HTTP status code
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&status);

// custom http request
		if(arg[21]!=NULL)
		{
			curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sockextr); 
			sockfd = sockextr;

			if(!wait_on_socket(sockfd, 0, 60000L))
			{
				DEBUG("error in socket at custom http request");
			}
			res=curl_easy_send(curl, make, strlen(make), &iolen);
// recv data of custom request
			while(1)
			{
				wait_on_socket(sockfd, 1, 60000L);
				chunk.memory=xmallocarray(3024,sizeof(char)); 
				res = curl_easy_recv(curl, chunk.memory, 3023, &iolen); 
				chunk.size=strnlen(chunk.memory,3023);				

				if(strnlen(chunk.memory,3023) > 8)
					break;

			        if(CURLE_OK != res)
        				break;

			}

			
			status=(long)parse_http_status(chunk.memory);
//status=404;
		}

			

// length of response
		if(chunk.size<=0)
			length=0.0;
		else
			length=chunk.size;

// if have error at status
	
		if(status==0)
		{	
			debug_host--;
			DEBUG("Problem in Host: \n %s\n Host is down ?\n",chunk.memory);

			if(debug_host==0)
				exit(0);

			sleep(3);

			goto try_again;
			
		
		}


//		XFREE(chunk.memory);
	
	//	curl_easy_cleanup(curl);
       // 	curl_global_cleanup();

		if(old>0)
			old--;

		if(counter_cookie > 0)
			counter_cookie--;

		if(counter_agent > 0)
			counter_agent--;

		debug_host=3;

	
	
	}

	// Write results in log and htmnl+js in /opt/0d1n/view
	write_result(	pack,
			save_response,
			(char *)chunk.memory,
			pathtable,
			line,
			make,
			make_agent,
			make_cookie,
			counter_cookie,
			counter_agent,
			status,
			length
	);	


// clear all
	XFREE(chunk.memory);
	XFREE(make_agent);
	XFREE(make_cookie);
	XFREE(make);
	XFREE(make2);

	XFREE(chunk.memory);
	


	if(arg[20] != NULL)
		XFREE(line);

	if(arg[22] != NULL)
		XFREE(token);
 
}

