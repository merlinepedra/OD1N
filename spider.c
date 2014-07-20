#include "spider.h"
#define TABLE "tables/output_array.txt"
#define TEMPLATE "template.conf"
#define TEMPLATE2 "hammer1.conf"
#define TEMPLATE3 "hammer2.conf"


void spider(void *pack,char *line,char * pathtable)
{
	struct MemoryStruct chunk;
	FILE *fp=NULL;

	bool match_string=false;
	long status=0;
	int old=0,counter=0,POST=0,sum_size=0,mem_size=0; 
	char *make=NULL,*pathsource=NULL,*response_template=NULL;
	char **pack_ptr=(char **)pack,**arg = pack_ptr;
	char tabledata[6660],randname[16],log[5025],line2[1024],randname2[16];

	pathsource=(char *)xmalloc(sizeof(char)*64);
	memset(pathsource,0,sizeof(char)*63);


	CURL *curl;  
	curl_global_init(CURL_GLOBAL_ALL); 

	POST=(arg[4]==NULL)?0:1;
   
	counter=char_type_counter(POST?arg[4]:arg[0],'!');
	old=counter;  
	chomp(line);

	while ( old )
	{
		chunk.memory=NULL; 
		chunk.size = 0;  

		make=payload_injector( (POST?arg[4]:arg[0]),line,old);
		curl = curl_easy_init();
		curl_easy_setopt(curl,  CURLOPT_URL, POST?arg[0]:make);
 
		if ( POST )
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, make);
      
		curl_easy_setopt(curl,  CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl,  CURLOPT_WRITEDATA, (void *)&chunk);
    
		if ( arg[6]!=NULL )
		{
			curl_easy_setopt(curl,  CURLOPT_USERAGENT, arg[6]);
		} else {
			curl_easy_setopt(curl,  CURLOPT_USERAGENT, "Mozilla/5.0 (0d1n v0.1) ");
		}
 
		curl_easy_setopt(curl,  CURLOPT_ENCODING,"gzip,deflate");

		if ( arg[3] != NULL )
		{
			curl_easy_setopt(curl,CURLOPT_COOKIEFILE,arg[3]);
			curl_easy_setopt(curl,CURLOPT_COOKIEJAR,arg[3]);
		} else {
			curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"odin_cookiejar.txt");
		}

		curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);

		if ( arg[7] != NULL ) 
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
			curl_easy_setopt(curl, CURLOPT_CAINFO, arg[7]);
		} else {
			curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0); 
		}

		if ( arg[8] != NULL ) 
			curl_easy_setopt(curl,CURLOPT_TIMEOUT,atoi(arg[8])); 

		if ( arg[9] != NULL ) 
			curl_easy_setopt(curl,CURLOPT_SSLVERSION,atoi(arg[9])); 
   


        //       curl_easy_setopt(curl,CURLOPT_VERBOSE,1); 
 
		curl_easy_setopt(curl,CURLOPT_HEADER,1);  
		curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&status);
		curl_easy_cleanup(curl);

// arg[10]  list to find with regex , arg[2] list without regex
		if( (arg[2] || arg[10] ) && status != 0 )
		{


			pathsource=(char *)xmalloc(sizeof(char)*64);
			memset(pathsource,0,sizeof(char)*63);

			fp = fopen((arg[2]!=NULL)?arg[2]:arg[10], "r");
			if ( !fp )
			{ 
				puts("error to open response list"); 
				exit(0);
			}

			while ( fgets(line2,1023,fp) != NULL) 
			{
				chomp(line2);

				if ( arg[2] != NULL )
					match_string=strstr(chunk.memory,line2)?true:false;

				if ( arg[10] != NULL )
					match_string=strstr_regex(chunk.memory,line2)?true:false;

				if(chunk.memory && (match_string == true) ) 
				{
					fprintf(stdout,"%s [ %s %lu %s ] Payload: %s %s %s Grep: %s %s %s  Params: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,line2,YELLOW,make,LAST);


					pathsource=(char *)xmalloc(sizeof(char)*64);
					memset(pathsource,0,sizeof(char)*63);

			//		pathsource=realloc(pathsource,sizeof(char)*64);
			//		memset(pathsource,0,sizeof(char)*63);
					mem_size=64;
					mem_size+=18;

					pathsource=xrealloc(pathsource,sizeof(char)*mem_size);
					strncat(pathsource,"response_sources/",18);
					mem_size+=16;

					pathsource=xrealloc(pathsource,sizeof(char)*mem_size);
					strncat(pathsource,rand_str(randname2, sizeof randname2),16);
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
					snprintf(log,5023,"[%lu] Payload: %s  Grep: %s Params: %s \n Path Response Source: %s\n",status,line,line2,make,pathsource);
					WriteFile(arg[5],log);
					WriteFile(pathsource,readLine(TEMPLATE));
					WriteFile(pathsource,html_entities(chunk.memory));
					WriteFile(pathsource,"</pre></html>");

					snprintf(tabledata,6659,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%lu </a>\",\"%s\",\"%s\",\"%s\"],\n",pathsource,status,html_entities(make),
					html_entities(line2),html_entities(line));
      					WriteFile(pathtable,tabledata);
					memset(pathsource,0,strlen(pathsource)-1);
					if(pathsource)
						free(pathsource);
					pathsource=NULL;
				}
			}
 		fclose(fp);
	} else {
		fprintf(stdout,"%s [ %s %lu %s ] Payload: %s %s %s Params: %s %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,make,LAST);
		
		pathsource=(char *)xmalloc(sizeof(char)*64);
		memset(pathsource,0,sizeof(char)*63);
		sum_size=64;
		sum_size+=18;

		pathsource=xrealloc(pathsource,sizeof(char)*sum_size);

		strncat(pathsource,"response_sources/",17);
		sum_size+=16;

		pathsource=xrealloc(pathsource,sizeof(char)*sum_size);

		strncat(pathsource,rand_str(randname2, sizeof randname2),15);
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
		snprintf(log,5023,"[%lu] Payload: %s Params: %s \n Path Response Source: %s\n",status,line,make,pathsource);
		WriteFile(arg[5],log);
                response_template=(char *)xmalloc(sizeof(char)*FileSize(TEMPLATE)+200);
                response_template=readLine(TEMPLATE);
		WriteFile(pathsource,response_template);
                response_template=xcalloc(1,1);
           		xfree(response_template);
               // response_template=NULL;
		WriteFile(pathsource,html_entities(chunk.memory));
		WriteFile(pathsource,"</pre></html>");
		snprintf(tabledata,3659,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%lu </a>\",\"%s\",\"%s\",\"%s\"],\n",pathsource,status,html_entities(make),
		html_entities(line2),html_entities(line));
		WriteFile(pathtable,tabledata);
                memset(pathsource,0,strlen(pathsource)-1);
		xfree(pathsource);
                pathsource=NULL;
	}
	if( make )
		xfree(make);

	if( chunk.size < 10 ) 
		xfree(chunk.memory);

	if(pathsource)
		xfree(pathsource);
	old--;

	}


}


void scan(void *arguments)
{
	FILE *fp=NULL;
        int num1=0,num2=0;

	char **arg = (char **)arguments;
	char *pathtable=NULL,*pathhammer=NULL,*view=NULL;
	char line[2048]; 
        char *template2=NULL,*template3=NULL;
 
	pathtable=(char *)xmalloc(sizeof(char)*64);
	memset(pathtable,0, sizeof(char)*63);
	strncat(pathtable,"tables/",8);
	strncat(pathtable,arg[5],16);
	strncat(pathtable,".txt",5);
	fp = fopen(arg[1], "r");

 	if ( !fp )
	{ 
		puts("error to open Payload list"); 
		exit(1);
	}
        num1=FileSize(TEMPLATE2);
        num2=FileSize(TEMPLATE3); 

	view=(char *)xmalloc(sizeof(char)*(num1+num2+42));
	memset(view,0,sizeof(char)*(num1+num2+1));

        template2=(char *)xmalloc(sizeof(char)*num1+1);
        template2=readLine(TEMPLATE2);
	strncat(view,template2,num1+1);
	strncat(view,"\"sAjaxSource\": \"",23);
	strncat(view,arg[5],16);
	strncat(view,".txt\" \n",9);

        template3=(char *)xmalloc(sizeof(char)*num2+1);
        template3=readLine(TEMPLATE3);

	strncat(view,template3,num2);

	pathhammer=(char *)xmalloc(sizeof(char)*64);
	memset(pathhammer,0,sizeof(char)*63);
	strncat(pathhammer,"tables/hammer_",15);
	strncat(pathhammer,arg[5],16);
	strncat(pathhammer,".html",6);
	WriteFile(pathhammer,view);
	WriteFile(pathtable,"{ \"aaData\": [ \n");


	while ( fgets(line,2047,fp) != NULL ) 
		spider(arguments,line,pathtable);
  
	WriteFile(pathtable," [\"\",\"\",\"\",\"\"] \n ] }");

	puts(RED);
	fprintf(stdout,"end scan \n look the file %s\n \n",pathhammer);
	puts(LAST);

	

	memset(pathtable,0,sizeof(char)*strlen(pathtable));
	if(pathtable)
		xfree(pathtable);


	memset(pathhammer,0,sizeof(char)*strlen(pathhammer));
	if(pathhammer)
		xfree(pathhammer);

	memset(view,0,sizeof(char)*strlen(view));

	if(view)
		xfree(view);

	fclose(fp);

	exit(0);
}
