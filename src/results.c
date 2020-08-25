#include "results.h"
#include "mem_ops.h"
#define TEMPLATE_RES "/opt/0d1n/templates/template.conf"
#define RESPONSE_PATH "/opt/0d1n/view/response_sources"

void write_result(
 void *pack, 
 bool save_response,
 char * chunk,
 char *pathtable,
 char *line,
 char * make, 
 char * make_agent, 
 char * make_cookie,
 int counter_cookie,
 int counter_agent,
 long status,
 long length
)
{
	FILE *fp=NULL;
	bool match_string=false; 
	char **pack_ptr=(char **)pack,**arg = pack_ptr;
	char randname[16],line2[1024],log[3048],tabledata[4086],pathsource[1024],pathurl[1024];

	// arg[10]  list to find with regex , arg[2] list without regex
		if(  (arg[2]) || (arg[10])  )
		{

			fp = fopen((arg[2]!=NULL)?arg[2]:arg[10], "r");

			if ( !fp )
			{ 
				DEBUG("Find List error\nPlease use correct argv\nerror to open response list : %s",strerror(errno)); 
				exit(1);
			}

			while ( fgets(line2,1023,fp) != NULL) 
			{
				chomp(line2);
				char *tmp_make_cookie=NULL,*tmp_make_agent=NULL;
				char *tmp_make=NULL,*tmp_line=NULL,*tmp_line2=NULL,*tmp_response=NULL,*responsetemplate=NULL;

				// find a string in response
				if(status != 0)
				{
					if ( arg[2] != NULL )
						match_string=strstr(chunk,line2)?true:false;

					if ( arg[10] != NULL )
						match_string=strstr_regex(chunk,line2)?true:false;
				}
				if(chunk && (match_string == true) ) 
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
						strlcat(pathsource,RESPONSE_PATH,45);
						strlcat(pathsource,"/",47);
						strlcat(pathsource,arg[5], 78);
						mkdir(pathsource,S_IRWXU|S_IRWXG|S_IRWXO);
						char *str_tmp_rng=rand_str(randname, sizeof randname);
						snprintf(pathsource,986,"%s/%s/%s.html",RESPONSE_PATH,arg[5],str_tmp_rng);
						snprintf(pathurl,986,"response_sources/%s/%s.html",arg[5],str_tmp_rng);
					}

					// write log file
					snprintf(log,3047,"[ %ld ] Payload: %s  Grep: %s Params: %s cookie: %s  UserAgent: %s \n Path Response Source: %s\n",status,line,line2,make,(make_cookie!=NULL)?make_cookie:" ",(make_agent!=NULL)?make_agent:" ",pathsource);
					WriteFile(arg[5],log);
					memset(log,0,3047);		

					if(save_response==true)
					{
// write highlights response
						responsetemplate=NULL;
                				responsetemplate=readLine(TEMPLATE_RES);
						WriteFile(pathsource,responsetemplate);
						memset(responsetemplate,0,strlen(responsetemplate)-1);
						tmp_response=NULL;
						tmp_response=html_entities(chunk);
						WriteFile(pathsource,tmp_response);
				//		memset(tmp_response,0,strlen(tmp_response)-1);
						WriteFile(pathsource,"</pre></html>");
					}


// create datatables	
				
					tmp_make=html_entities(make);
					tmp_line2=html_entities(line2);
					tmp_line=html_entities(line);

					if(make_cookie!=NULL)
					{
						tmp_make_cookie=html_entities(make_cookie);
						snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s cookie: %s\",\"%s\",\"%s\"],\n",pathurl,status,length,tmp_make,tmp_make_cookie,tmp_line2,tmp_line);
						memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
					}

					if(make_agent!=NULL)
					{
						tmp_make_agent=html_entities(make_agent);
						snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s UserAgent: %s\",\"%s\",\"%s\"],\n",pathurl,status,length,tmp_make,tmp_make_agent,tmp_line2,tmp_line);
						memset(tmp_make_agent,0,strlen(tmp_make_agent)-1);
					} else {
						snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s\",\"%s\",\"%s\"],\n",pathurl,status,length,tmp_make,tmp_line2,tmp_line);
      					}
					WriteFile(pathtable,tabledata);
			// for debug purpose
			//		memset(tmp_make,0,strlen(tmp_make)-1);
			//		memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
			//		memset(tmp_make_agent,0,strlen(tmp_make_agent)-1);
			//		memset(tmp_line,0,strlen(tmp_line)-1);
			//		memset(tmp_line2,0,strlen(tmp_line2)-1);
			//		memset(tabledata,0,4085);
			//		memset(pathsource,0,strlen(pathsource)-1);
					XFREE(tmp_line);
					XFREE(tmp_make);
					XFREE(tmp_line2);
					XFREE(responsetemplate);
					XFREE(tmp_response);	
					XFREE(tmp_make_cookie);
					XFREE(tmp_make_agent);
	
	
				}

			}
 
			
			if( fclose(fp) == EOF )
			{
				DEBUG("Error in close()");
				exit(1);
			}

			
			fp=NULL;

		} else {
			char *tmp_line=NULL,*tmp_make=NULL,*tmp_response=NULL,*responsetemplate=NULL;
			char *tmp_make_cookie=NULL,*tmp_make_agent=NULL;

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
				memset(pathsource,0,sizeof(char)*1023);
				strlcat(pathsource,RESPONSE_PATH,45);
				strlcat(pathsource,"/",46);
				strlcat(pathsource,arg[5], 60);
				mkdir(pathsource,S_IRWXU|S_IRWXG|S_IRWXO);
				snprintf(pathsource,986,"%s/%s/%s.html",RESPONSE_PATH,arg[5],rand_str(randname, sizeof randname));
			}

//write logs
			snprintf(log,3047,"[%ld Payload: %s Params: %s Cookie: %s UserAgent: %s \n Path Response Source: %s\n",status,line,make,(make_cookie!=NULL)?make_cookie:" ",(make_agent!=NULL)?make_agent:" ",pathsource);
			WriteFile(arg[5],log);
			memset(log,0,3047);

			if(save_response==true)
			{
// write response source with highlights
              	 		responsetemplate=readLine(TEMPLATE_RES);
				WriteFile(pathsource,responsetemplate);
				//memset(responsetemplate,0,strlen(responsetemplate)-1);
				tmp_response=html_entities(chunk);
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
				snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s  cookie: %s\",\"\",\"%s\"],\n",pathurl,status,length,tmp_make,tmp_make_cookie,tmp_line);
			//	memset(tmp_make_cookie,0,strlen(tmp_make_cookie)-1);
			}

			if(counter_agent)
			{
				
				tmp_make_agent=html_entities(make_agent);
				snprintf(tabledata,4085,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s  UserAgent: %s\",\"\",\"%s\"],\n",pathurl,status,length,tmp_make,tmp_make_agent,tmp_line);

			} else {
				snprintf(tabledata,4047,"[\"<a class=\\\"fancybox fancybox.iframe\\\" href=\\\"../%s\\\">%ld </a>\",\"%ld\",\"%s\",\"\",\"%s\"],\n",pathurl,status,length,tmp_make,tmp_line);
			}
  			WriteFile(pathtable,tabledata);

			XFREE(tmp_line);
			XFREE(tmp_make);
			XFREE(tmp_response);
			XFREE(responsetemplate);
			XFREE(tmp_make_cookie);
			XFREE(tmp_make_agent);
	
		}
 
}

