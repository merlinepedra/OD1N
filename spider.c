#include "spider.h"
#define TABLE "tables/output_array.txt"
#define TEMPLATE "template.conf"

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) 
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);

  if(mem->memory) 
  {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

void spider(void *pack,char *line)
{
 struct MemoryStruct chunk;
 FILE *fp=NULL;

 long status=0;
 int old=0,counter=0,POST=0; 
 char *make=NULL,*pathsource=NULL;
 char **pack_ptr=(char **)pack,**arg = pack_ptr;
 char tabledata[6660],randname[16],log[5025],line2[1024],randname2[16];

 CURL *curl;  
 curl_global_init(CURL_GLOBAL_ALL); 

 POST=(arg[4]==NULL)?0:1;
   
 counter=char_type_counter(POST?arg[4]:arg[0],'!');
 old=counter;  
 chomp(line);

   while(old)
   {
    chunk.memory=NULL; 
    chunk.size = 0;  

    make=payload_injector( (POST?arg[4]:arg[0]),line,old);
    
    curl = curl_easy_init();
    curl_easy_setopt(curl,  CURLOPT_URL, POST?arg[0]:make);
 
     if(POST)
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, make);
      
    curl_easy_setopt(curl,  CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl,  CURLOPT_WRITEDATA, (void *)&chunk);
    
    if(arg[6]!=NULL)
    {
     curl_easy_setopt(curl,  CURLOPT_USERAGENT, arg[6]);
    } else {
     curl_easy_setopt(curl,  CURLOPT_USERAGENT, "Mozilla/5.0 (0d1n v0.1) ");
    }
 
    curl_easy_setopt(curl,  CURLOPT_ENCODING,"gzip,deflate");

    if(arg[3]!=NULL)
    {
     curl_easy_setopt(curl,CURLOPT_COOKIEFILE,arg[3]);
     curl_easy_setopt(curl,CURLOPT_COOKIEJAR,arg[3]);
    } else {
     curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"odin_cookiejar.txt");
    }

    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);

    if(arg[7]!=NULL) 
    {
     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
     curl_easy_setopt(curl, CURLOPT_CAINFO, arg[7]);
    } else {
     curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0); 
    }

    if(arg[8]!=NULL) 
     curl_easy_setopt(curl,CURLOPT_TIMEOUT,atoi(arg[8])); 
    
    curl_easy_setopt(curl,CURLOPT_HEADER,1);  
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&status);
    curl_easy_cleanup(curl);

    fp = fopen(arg[2], "r");
    if(!fp)
    { 
     puts("error to open response list"); 
     exit(0);
    }


    while(fgets(line2,1023,fp) != NULL) 
    {
     chomp(line2);

     if(chunk.memory && bitap_search(chunk.memory,line2)) 
     {
      fprintf(stdout,"%s [ %s %lu %s ] Payload: %s %s %s Grep: %s %s %s  Params: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,line2,YELLOW,make,LAST);

      pathsource=(char *)malloc(sizeof(char)*64);
      bzero(pathsource, sizeof(char)*64);
      strncat(pathsource,"response_sources/",63);
      strncat(pathsource,rand_str(randname2, sizeof randname2),63);
      mkdir(pathsource,S_IRWXU|S_IRWXG|S_IRWXO);
      strncat(pathsource,"/",63);
      strncat(pathsource,rand_str(randname, sizeof randname),63);
      strncat(pathsource,".html",63);
      snprintf(log,5023,"[%lu] Payload: %s  Grep: %s Params: %s \n Path Response Source: %s\n",status,line,line2,make,pathsource);
      WriteFile(arg[5],log);
      WriteFile(pathsource,readLine(TEMPLATE));
      WriteFile(pathsource,html_entities(chunk.memory));
      WriteFile(pathsource,"</pre></html>");

      snprintf(tabledata,6659,"[\"<a href=\\\"../%s\\\">%lu </a>\",\"%s\",\"%s\",\"%s\"],\n",pathsource,status,html_entities(make),
html_entities(line2),html_entities(line));
      WriteFile(TABLE,tabledata);
      
      free(pathsource);
     }
    }
 
    fclose(fp);

    if(make)
     free(make);

    if(chunk.size <10) 
     free(chunk.memory);

    old--;
   }


}


void scan(void *arguments)
{
 FILE *fp=NULL;

 char **arg = (char **)arguments;
 char line[2048]; 

 fp = fopen(arg[1], "r");

  if(!fp)
  { 
   puts("error to open Payload list"); 
   exit(1);
  }


  unlink(TABLE);
  WriteFile(TABLE,"{ \"aaData\": [ \n");
 
   while(fgets(line,2047,fp) != NULL) 
    spider(arguments,line);
  
  WriteFile(TABLE," [\"\",\"\",\"\",\"\"] \n ] }");

  fclose(fp);

  puts(RED);
  puts("end scan \n look the tables/hammer.html");
  puts(LAST);

}
