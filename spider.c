#include "spider.h"
#define TEMPLATE "template.conf"

void spider(void *pack,char *line,char * pathtable)
{
	struct MemoryStruct chunk;
	FILE *fp=NULL;
	bool match_string=false,save_response=false,test_tamper=false;
	long status=0,length=0;
	int old=0,res=0,counter=0,counter_cookie=0,counter_agent=0,POST=0,timeout=0,debug_host=3; 
	char *make=NULL,*make2=NULL,*make_cookie=NULL,*make_agent=NULL,*tamper=NULL,*responsetemplate=NULL,*tmp_response=NULL,*tmp_make=NULL,*tmp_make_cookie=NULL,*tmp_make_agent=NULL,*tmp_line=NULL,*tmp_line2=NULL,*token=NULL;
	char **pack_ptr=(char **)pack,**arg = pack_ptr;
	char randname[16],line2[1024],log[2048],tabledata[4086],pathsource[1024];

	if(arg[12]!=NULL)
		save_response=true;

	if(arg[8]!=NULL)
		timeout=(int)strtol(arg[8],(char **)NULL,10);

// if need get anti-csrf token
	if(arg[22]!=NULL && arg[23]!=NULL)
	{
		if(arg[6]!=NULL)
			token=get_anti_csrf_token(arg[22],arg[23],arg[6]);
		else
			token=get_anti_csrf_token(arg[22],arg[23],"Mozilla/5.0 (0d1n v0.1)");
	}



// payload tamper, get payload of line and make tamper 
	if(arg[20]!=NULL)
	{
		tamper=arg[20];
			
		if(strstr(tamper,"encode64"))
		{
			line=encode64(line,strlen(line)-1);
			test_tamper=true;
		}

		if(strstr(tamper,"randcase"))
		{
			line=rand_case(line);
			test_tamper=true;
		}


		if(strstr(tamper,"urlencode"))
		{
			line=urlencode(line);
			test_tamper=true;
		}

		if(strstr(tamper,"double_urlencode"))
		{
			line=double_urlencode(line);
			test_tamper=true;
		}

		if(strstr(tamper,"spaces2comment"))
		{
			line=spaces2comment(line);
			test_tamper=true;
		}

		if(strstr(tamper,"unmagicquote"))
		{
			line=unmagicquote(line);
			test_tamper=true;
		}


		if(strstr(tamper,"apostrophe2nullencode"))
		{
			line=apostrophe2nullencode(line);
			test_tamper=true;
		}

		if(strstr(tamper,"rand_comment"))
		{
			line=rand_comment(line);
			test_tamper=true;
		}



		if(strstr(tamper,"rand_space"))
		{
			line=rand_space(line);
			test_tamper=true;
		}



		if(strstr(tamper,"replace_keywords"))
		{
			line=replace_keywords(line);
			test_tamper=true;
		}



		if(test_tamper==false)
		{
			DEBUG("error at tamper argument\n");
			exit(0);
		}

		
	}

	memset(pathsource,0,sizeof(char)*1023);

	if(save_response==false)
		strlcat(pathsource,"0",1024);

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
		if(arg[21]==NULL)
		{
			make2=payload_injector( (POST?arg[4]:arg[0]),line,old);
			if(token!=NULL)
		 		make=replace(make2,"{token}",token); // if user pass token to bypass anti-csrf
			else
				make=strdup(make2);	

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


// arg[10]  list to find with regex , arg[2] list without regex
		if(  (arg[2]) || (arg[10])  )
		{
			if(save_response==true)
			{
				memset(pathsource,0,sizeof(char)*1023);
			}

			fp = fopen((arg[2]!=NULL)?arg[2]:arg[10], "r");

			if ( !fp )
			{ 
				DEBUG("error to open response list"); 
				exit(1);
			}

			while ( fgets(line2,1023,fp) != NULL) 
			{
				chomp(line2);

// find a string in response
				if(status != 0)
				{
					if ( arg[2] != NULL )
						match_string=strstr(chunk.memory,line2)?true:false;

					if ( arg[10] != NULL )
						match_string=strstr_regex(chunk.memory,line2)?true:false;
				}

				if(chunk.memory && (match_string == true) ) 
				{
					if(make_cookie!=NULL)
					{
						fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Grep: %s %s %s  Params: %s \nCookie: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,line2,YELLOW,make,make_cookie,LAST);
					} 

					if(make_agent!=NULL)
					{
						fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Grep: %s %s %s  Params: %s \nCookie: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,line2,YELLOW,make,make_agent,LAST);
					
					} else {

						fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Grep: %s %s %s  Params: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,line2,YELLOW,make,LAST);
					
					}

					if(save_response==true)
					{
// create responses path
						memset(pathsource,0,sizeof(char)*1023);
						strlcat(pathsource,"response_sources/",18);
						strlcat(pathsource,arg[5], 33);
						mkdir(pathsource,S_IRWXU|S_IRWXG|S_IRWXO);
						snprintf(pathsource,986,"response_sources/%s/%s.html",arg[5],rand_str(randname, sizeof randname));
					}
// write log file
					snprintf(log,2047,"[ %ld ] Payload: %s  Grep: %s Params: %s cookie: %s  UserAgent: %s \n Path Response Source: %s\n",status,line,line2,make,(make_cookie!=NULL)?make_cookie:" ",(make_agent!=NULL)?make_agent:" ",pathsource);
					WriteFile(arg[5],log);
					memset(log,0,2047);		

					if(save_response==true)
					{
// write highlights response
						responsetemplate=NULL;
                				responsetemplate=readLine(TEMPLATE);
						WriteFile(pathsource,responsetemplate);
						memset(responsetemplate,0,strlen(responsetemplate)-1);
						tmp_response=NULL;
						tmp_response=html_entities(chunk.memory);
						WriteFile(pathsource,tmp_response);
						memset(tmp_response,0,strlen(tmp_response)-1);
						WriteFile(pathsource,"</pre></html>");
					}
// create datatables	
				
					tmp_make=html_entities(make);
					tmp_line2=html_entities(line2);
					tmp_line=html_entities(line);

					if(make_cookie!=NULL)
					{
						tmp_make_cookie=html_entities(make_cookie);
						snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s cookie: %s\",\"%s\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_make_cookie,tmp_line2,tmp_line);
						memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
					}

					if(make_agent!=NULL)
					{
						tmp_make_agent=html_entities(make_agent);
						snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s UserAgent: %s\",\"%s\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_make_agent,tmp_line2,tmp_line);
						memset(tmp_make_agent,0,strlen(tmp_make_agent)-1);
					} else {
						snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s\",\"%s\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_line2,tmp_line);
      					}

					WriteFile(pathtable,tabledata);
				//	memset(tmp_make,0,strlen(tmp_make)-1);
				//	memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
				//	memset(tmp_make_agent,0,strlen(tmp_make_agent)-1);
					memset(tmp_line,0,strlen(tmp_line)-1);
					memset(tmp_line2,0,strlen(tmp_line2)-1);
					memset(tabledata,0,4085);
					memset(pathsource,0,strlen(pathsource)-1);
				}
			}
 
			
			if( fclose(fp) == EOF )
			{
				DEBUG("Error in close()");
				exit(1);
			}

			
			fp=NULL;

		} else {

			if(counter_cookie)
			{
				fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Params: %s %s\n Cookie: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,make,make_cookie,LAST);
			}
			if(counter_agent)
			{
				fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Params: %s %s\n UserAgent: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,make,make_agent,LAST);
			} else {
				fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Params: %s %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,make,LAST);
			}
	
			if(save_response==true)
			{		
			//	memset(pathsource,0,sizeof(char)*1023);
				strlcat(pathsource,"response_sources/",18);
				strlcat(pathsource,arg[5], 33);
				mkdir(pathsource,S_IRWXU|S_IRWXG|S_IRWXO);
				snprintf(pathsource,986,"response_sources/%s/%s.html",arg[5],rand_str(randname, sizeof randname));
			}
//write logs
			snprintf(log,2047,"[%ld Payload: %s Params: %s Cookie: %s UserAgent: %s \n Path Response Source: %s\n",status,line,make,(make_cookie!=NULL)?make_cookie:" ",(make_agent!=NULL)?make_agent:" ",pathsource);
			WriteFile(arg[5],log);
			memset(log,0,2047);

			if(save_response==true)
			{
// write response source with highlights
              	 		responsetemplate=readLine(TEMPLATE);
				WriteFile(pathsource,responsetemplate);
				//memset(responsetemplate,0,strlen(responsetemplate)-1);
				tmp_response=html_entities(chunk.memory);
				WriteFile(pathsource,tmp_response);
				//memset(tmp_response,0,strlen(tmp_response)-1);
	
				WriteFile(pathsource,"</pre></html>");
			}
// create datatables
			tmp_make=html_entities(make);
			tmp_line=html_entities(line);

			if(counter_cookie)
			{
				
				tmp_make_cookie=html_entities(make_cookie);
				snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s  cookie: %s\",\"\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_make_cookie,tmp_line);
			//	memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
			}

			if(counter_agent)
			{
				
				tmp_make_agent=html_entities(make_agent);
				snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s  UserAgent: %s\",\"\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_make_agent,tmp_line);

			} else {
				snprintf(tabledata,4047,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s\",\"\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_line);
			}
  			WriteFile(pathtable,tabledata);
			memset(tmp_make,0,strlen(tmp_make)-1);
			memset(tmp_line,0,strlen(tmp_line)-1);
			memset(tabledata,0,4085);
			memset(pathsource,0,strlen(pathsource)-1);

		}


		XFREE(chunk.memory);
	
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

// clear all
	XFREE(make_agent);
	XFREE(make_cookie);
	XFREE(make);
	XFREE(make2);
	XFREE(tmp_make);
	XFREE(tmp_make_cookie);
	XFREE(tmp_make_agent); 
	XFREE(tmp_line);
	XFREE(tmp_line2);
	XFREE(responsetemplate);
	XFREE(tmp_response);

	if(arg[20] != NULL)
		XFREE(line);

	if(arg[22] != NULL)
		XFREE(token);
 
}

