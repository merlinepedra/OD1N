#include "spider.h"
#define TABLE "tables/output_array.txt"
#define TEMPLATE "template.conf"
#define TEMPLATE2 "hammer1.conf"
#define TEMPLATE3 "hammer2.conf"

int threadss=4;

void spider(void *pack,char *line,char * pathtable)
{
	struct MemoryStruct chunk;
	FILE *fp=NULL;
	bool match_string=false,save_response=false;
	long status=0,length=0;
	int old=0,counter=0,counter_cookie=0,POST=0,sum_size=0,mem_size=0,size_log=0,timeout=0,debug_host=3; 
	char *make=NULL,*make_cookie=NULL,*pathsource=NULL,*responsetemplate=NULL,*log=NULL,*tabledata=NULL,*tmp_response=NULL,*tmp_make=NULL,*tmp_make_cookie=NULL,*tmp_line=NULL,*tmp_line2=NULL;
	char **pack_ptr=(char **)pack,**arg = pack_ptr;
	char randname[16],line2[1024];

	if(arg[12]!=NULL)
		save_response=true;

	if(arg[8]!=NULL)
		timeout=atoi(arg[8]);

	pathsource=xmalloc(sizeof(char)*64);
	memset(pathsource,0,sizeof(char)*63);

	if(save_response==false)
	{
		pathsource="0";
	}

// brute POST/GET/COOKIES
	POST=(arg[4]==NULL)?0:1;
	counter=char_type_counter(POST?arg[4]:arg[0],'^');
	counter_cookie=char_type_counter(arg[13]!=NULL?arg[13]:"",'^');

	old=counter;  
	chomp(line);

// goto to fix signal stop if user do ctrl+c
	try_again:

	while ( old > 0 || counter_cookie > 0)
	{
		CURL *curl;  
		curl_global_init(CURL_GLOBAL_ALL); 

		chunk.memory=NULL; 
		chunk.size = 0;  

	
		make=payload_injector( (POST?arg[4]:arg[0]),line,old);
		 		
		if(arg[13]!=NULL)
			make_cookie=payload_injector( arg[13],line,counter_cookie);	
	

		curl = curl_easy_init();
		curl_easy_setopt(curl,  CURLOPT_URL, POST?arg[0]:make);
 
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
// LOAD cookie

		if(arg[13]!=NULL)
		{
			curl_easy_setopt(curl,CURLOPT_COOKIE,make_cookie);
		}


		curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
// Load cacert
		if ( arg[7] != NULL ) 
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
			curl_easy_setopt(curl, CURLOPT_CAINFO, arg[7]);
		} else {
			curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0); 
		}

		if(timeout) 
			curl_easy_setopt(curl,CURLOPT_TIMEOUT,timeout); 

// load single proxy
		if(arg[17] != NULL)
		{
			curl_easy_setopt(curl, CURLOPT_PROXY, arg[17]);
	//		curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);
		}

// load proxy list 
		if(arg[18] != NULL)
		{
			char *randproxy=Random_linefile(arg[18]);
	//		printf("PROXY LOAD: %s\n",randproxy);
			curl_easy_setopt(curl, CURLOPT_PROXY, randproxy);
	//		curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);
		}


		if ( arg[9] != NULL ) 
			curl_easy_setopt(curl,CURLOPT_SSLVERSION,(long)atoi(arg[9]));

                curl_easy_setopt(curl,CURLOPT_VERBOSE,0); 
		curl_easy_setopt(curl,CURLOPT_HEADER,1);  
		curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&status);

// length of response
		if(chunk.size<=0)
			length=0.0;
		else
			length=chunk.size;

//		curl_easy_cleanup(curl);

		if(status==0)
		{	debug_host--;
			DEBUG("Problem in Host");
			if(debug_host<0)
				exit(0);
			
			goto try_again;
			
		}
// arg[10]  list to find with regex , arg[2] list without regex
		if(  (arg[2]) || (arg[10])  )
		{
			if(save_response==true)
			{
		//		pathsource=xmalloc(sizeof(char)*64);
				memset(pathsource,0,sizeof(char)*63);
			}

			fp = fopen((arg[2]!=NULL)?arg[2]:arg[10], "r");
			if ( !fp )
			{ 
				puts("error to open response list"); 
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
					} else {

						fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Grep: %s %s %s  Params: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,line2,YELLOW,make,LAST);
					
					}
					if(save_response==true)
					{
// create responses path
				//		pathsource=xmalloc(sizeof(char)*64);
						memset(pathsource,0,sizeof(char)*63);
						mem_size=64;
						mem_size+=18;
						pathsource=xrealloc(pathsource,sizeof(char)*mem_size);
						strncat(pathsource,"response_sources/",18);
						mem_size+=16;
						pathsource=xrealloc(pathsource,sizeof(char)*mem_size);
						strncat(pathsource,arg[5], 15);
						mkdir(pathsource,S_IRWXU|S_IRWXG|S_IRWXO);
						mem_size+=2;
						pathsource=xrealloc(pathsource,sizeof(char)*mem_size);
						strncat(pathsource,"/",2);
						mem_size+=17;
						pathsource=xrealloc(pathsource,sizeof(char)*mem_size);
						strncat(pathsource,rand_str(randname, sizeof randname),16);
						mem_size+=7;
						pathsource=xrealloc(pathsource,sizeof(char)*mem_size);
						strncat(pathsource,".html",6);
					}
// write log file
					size_log=strlen(line)+strlen(line2)+strlen(make)+strlen(pathsource)+256;
				 //	xfree((void **)&log);
					log=xmalloc(sizeof(char)*size_log);
					snprintf(log,size_log-1,"[ %ld ] Payload: %s  Grep: %s Params: %s cookie: %s \n Path Response Source: %s\n",status,line,line2,make,(make_cookie!=NULL)?make_cookie:" ",pathsource);
					WriteFile(arg[5],log);
					memset(log,0,strlen(log)-1);		
//					xfree((void **)&log);
			
					if(save_response==true)
					{
// write highlights response
						responsetemplate=NULL;
                		//		responsetemplate=xmalloc(sizeof(char)*FileSize(TEMPLATE)*8);
                				responsetemplate=readLine(TEMPLATE);
						WriteFile(pathsource,responsetemplate);
  //        					xfree((void **)&responsetemplate);
						memset(responsetemplate,0,strlen(responsetemplate)-1);
						tmp_response=NULL;
				//		tmp_response=xmalloc(sizeof(char)*(strlen(chunk.memory)+1));
						tmp_response=html_entities(chunk.memory);
						WriteFile(pathsource,tmp_response);
						memset(tmp_response,0,strlen(tmp_response)-1);
//						xfree((void **)&tmp_response);
						WriteFile(pathsource,"</pre></html>");
					}
// create datatables	
					tabledata=xmalloc(sizeof(char)*4548);
 			//		tmp_make=xmalloc((strlen(make)*sizeof(char))+1);
			//		tmp_line2=xmalloc(1050);
			//		tmp_line=xmalloc(2090);
					tmp_make=html_entities(make);
					tmp_line2=html_entities(line2);
					tmp_line=html_entities(line);

					if(make_cookie!=NULL)
					{
			//			tmp_make_cookie=xmalloc((strlen(make)*sizeof(char))+1);
						tmp_make_cookie=html_entities(make_cookie);
						snprintf(tabledata,4547,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s cookie: %s\",\"%s\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_make_cookie,tmp_line2,tmp_line);
//						xfree((void **)&tmp_make_cookie);
						memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
					} else {
						snprintf(tabledata,4547,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s\",\"%s\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_line2,tmp_line);
      					}

					WriteFile(pathtable,tabledata);
/*					xfree((void **)&tmp_make);
					xfree((void **)&tmp_make_cookie);
					xfree((void **)&tmp_line);
					xfree((void **)&tmp_line2);
					xfree((void **)&tabledata);
					xfree((void **)&pathsource);
*/
					memset(tmp_make,0,strlen(tmp_make)-1);
					memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
					memset(tmp_line,0,strlen(tmp_line)-1);
					memset(tmp_line2,0,strlen(tmp_line2)-1);
					memset(tabledata,0,strlen(tabledata)-1);
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
		} else {
			fprintf(stdout,"%s [ %s %ld %s ] Payload: %s %s %s Params: %s %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,make,LAST);
		}

		if(save_response==true)
		{		
		//	pathsource=xmalloc(sizeof(char)*64);
			memset(pathsource,0,sizeof(char)*63);
			sum_size=64;
			sum_size+=18;
			pathsource=xrealloc(pathsource,sizeof(char)*sum_size);
			strncat(pathsource,"response_sources/",17);
			sum_size+=16;
			pathsource=xrealloc(pathsource,sizeof(char)*sum_size);
			strncat(pathsource,arg[5], 15);
			mkdir(pathsource,S_IRWXU|S_IRWXG|S_IRWXO);
			sum_size+=2;
			pathsource=xrealloc(pathsource,sizeof(char)*sum_size);
			strncat(pathsource,"/",2);
			sum_size+=16;
			pathsource=xrealloc(pathsource,sizeof(char)*sum_size);
			strncat(pathsource,rand_str(randname, sizeof randname),16);
			sum_size+=6;
			pathsource=xrealloc(pathsource,sizeof(char)*sum_size);
			strncat(pathsource,".html",6);
		}
//write logs
		size_log=strlen(line)+strlen(make)+strlen(pathsource)+128;
		log=NULL;
		log=xmalloc(sizeof(char)*size_log);
		snprintf(log,size_log-1,"[%ld Payload: %s Params: %s Cookie: %s\n Path Response Source: %s\n",status,line,make,(make_cookie!=NULL)?make_cookie:" ",pathsource);
		WriteFile(arg[5],log);
		memset(log,0,strlen(log)-1);
//		xfree((void **)&log);

		if(save_response==true)
		{
// write response source with highlights
             // 	  	responsetemplate=xmalloc(sizeof(char)*FileSize(TEMPLATE)*8);
               		responsetemplate=readLine(TEMPLATE);
			WriteFile(pathsource,responsetemplate);
			memset(responsetemplate,0,strlen(responsetemplate)-1);

  //        		xfree((void **)&responsetemplate);
		//	tmp_response=xmalloc(sizeof(char)*(strlen(chunk.memory)+1));
			tmp_response=html_entities(chunk.memory);
			WriteFile(pathsource,tmp_response);
//			xfree((void **)&tmp_response);
			memset(tmp_response,0,strlen(tmp_response)-1);

			WriteFile(pathsource,"</pre></html>");
		}
// create datatables
		tabledata=xmalloc(sizeof(char)*4048);
 //		tmp_make=xmalloc((strlen(make)*sizeof(char))+1);
//		tmp_line=xmalloc(2090);
		tmp_make=html_entities(make);
		tmp_line=html_entities(line);

		if(counter_cookie)
		{
				
 		//	tmp_make_cookie=xmalloc((strlen(make_cookie)*sizeof(char))+1);
			tmp_make_cookie=html_entities(make_cookie);
			snprintf(tabledata,4047,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s  cookie: %s\",\"\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_make_cookie,tmp_line);
//			xfree((void **)&tmp_make_cookie);
			memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
		} else {
			snprintf(tabledata,4047,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s\",\"\",\"%s\"],\n",pathsource,status,length,tmp_make,tmp_line);
		}
      		WriteFile(pathtable,tabledata);
		memset(tmp_make,0,strlen(tmp_make)-1);
		memset(tmp_line,0,strlen(tmp_line)-1);
		memset(tabledata,0,strlen(tabledata)-1);
		memset(pathsource,0,strlen(pathsource)-1);
/*
		xfree((void **)&tmp_make);
		xfree((void **)&tmp_line);
		xfree((void **)&tabledata);
		xfree((void **)&pathsource);
*/
	}

	memset(make,0,strlen(make)-1);
//	xfree((void **)&make);

	memset(make_cookie,0,strlen(make_cookie)-1);
//	if(make_cookie!=NULL)
//		xfree((void **)&make_cookie);

	memset(pathsource,0,strlen(pathsource)-1);
	xfree((void **)&chunk.memory);
//	xfree((void **)&pathsource);

	old--;
	counter_cookie--;
	debug_host=3;

	curl_easy_cleanup(curl);
        curl_global_cleanup();

	}

	xfree((void **)&make);
	xfree((void **)&tmp_make);
	xfree((void **)&tmp_make_cookie);
	xfree((void **)&tmp_line);
	xfree((void **)&tmp_line2);
	xfree((void **)&tabledata);
	xfree((void **)&pathsource);
	xfree((void **)&responsetemplate);
	xfree((void **)&log);
	xfree((void **)&tmp_response);	
}


void scan(void *arguments)
{
	FILE *fp=NULL;
        int num1=0,num2=0;

	char **arg = (char **)arguments;
	char *pathtable=NULL,*pathhammer=NULL,*view=NULL,*template2=NULL,*template3=NULL;
	char line[2048]; 

	pid_t pid;

	if(arg[11]!=NULL)
		threadss=atoi(arg[11]);

	int old_thread=threadss;
	int status=-1;

 	int timeout=3;

	if(arg[8]!=NULL)
		timeout=atoi(arg[8]);
 
	pathtable=xmalloc(sizeof(char)*64);
	memset(pathtable,0, sizeof(char)*63);
	strncat(pathtable,"tables/",8);
	strncat(pathtable,arg[5],16);
	strncat(pathtable,".txt",5);
	fp = fopen(arg[1], "r");

 	if ( !fp )
	{ 
		DEBUG("error to open Payload list"); 
		exit(1);
	}

        num1=FileSize(TEMPLATE2);
        num2=FileSize(TEMPLATE3); 

	view=xmalloc(sizeof(char)*(num1+num2+42));
	memset(view,0,sizeof(char)*(num1+num2+1));

      //  template2=xmalloc(sizeof(char)*num1+1);
        template2=readLine(TEMPLATE2);
	strncat(view,template2,num1+1);
	strncat(view,"\"sAjaxSource\": \"",23);
	strncat(view,arg[5],16);
	strncat(view,".txt\" \n",9);

      //  template3=xmalloc(sizeof(char)*num2+1);
        template3=readLine(TEMPLATE3);

	strncat(view,template3,num2);

	pathhammer=xmalloc(sizeof(char)*64);
	memset(pathhammer,0,sizeof(char)*63);
	strncat(pathhammer,"tables/hammer_",15);
	strncat(pathhammer,arg[5],16);
	strncat(pathhammer,".html",6);
	WriteFile(pathhammer,view);
	WriteFile(pathtable,"{ \"aaData\": [ \n");

	puts("start...");

	while ( fgets(line,2047,fp) != NULL ) 
	{

		pid=fork();
 
		if(pid==-1)
		{
			DEBUG("error in fork()");
			exit(1);
		}
		if(!pid)
		{
			threadss--;
			spider(arguments,line,pathtable);
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

	sleep(timeout);

	WriteFile(pathtable," [\"\",\"\",\"\",\"\",\"\"] \n ] }");

	puts(RED);
	fprintf(stdout,"end scan \n look the file %s\n \n",pathhammer);
	puts(LAST);

	memset(pathtable,0,sizeof(char)*strlen(pathtable)-1);
	xfree((void **)&pathtable);
	memset(pathhammer,0,sizeof(char)*strlen(pathhammer)-1);
	xfree((void **)&pathhammer);
	memset(view,0,sizeof(char)*strlen(view)-1);
	xfree((void **)&view);

	if( fclose(fp) == EOF )
	{
		DEBUG("Error in close()");
		exit(1);
	}

	fp=NULL;

	exit(0);
}
