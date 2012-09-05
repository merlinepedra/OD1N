#include "spider.h"
#define TABLE "tables/output_array.txt"


char *rand_str(char *dst, int size)
{
 static const char text[] = "abcdefghijklmnopqrstuvwxyz"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

 int i, len = rand() % (size - 1);
 if(!len) 
  len=8;

  for(i=0; i<len; ++i)
   dst[i] = text[rand() % (sizeof text - 1)];
   
 dst[i] = '\0';

 return dst;
}

int char_type_counter(char *string,char type)
{
 int counter=0;
 
 while(*string != '\0')
 {
  if(*string==type) 
   counter++;
  string++;
 }
 return counter;
}


void chomp(char * str)
{
  while (*str) {
    if (*str == '\n' || *str == '\r') {
      *str = 0;
      return;
    }
    str++;
  }
}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) 
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

char *payload_injector(char * ptr,char * payload,int counter)
{
 char *new=(char *)malloc((strlen(ptr)+strlen(payload)+2)*sizeof(char));
 short i=0,x=1;
 bzero(new, sizeof(char)*(strlen(ptr)+strlen(payload)+1));

 while(*ptr != '\0')
 {
  if(*ptr == '!')
  {
   if(counter==x)
   {
    strncat(new,payload,strlen(payload));
    i+=strlen(payload);
   }
   x++;
  } 
  else 
  { 
   *(new+i)=*ptr;
   i++;
  }
  ptr++;
 }

 return new;
}


int WriteFile(char *file,char *str)
{
 FILE *arq;
 
 arq=fopen(file,"a"); 
  if(!arq) 
  {
   fprintf(stdout,"error in WriteFile() %s",file); 
   return 0;
  }
 fprintf(arq,"%s\n",str); 
 fclose(arq); 
 return 1;
}

void spider(void *pack,char *line)
{

 struct MemoryStruct chunk;
 FILE *fp=NULL;

// CURL *curl;  
 CURL *curl;  
 curl_global_init(CURL_GLOBAL_ALL); 

 int old=0,counter=0,POST=0,status=0;
 char line2[1024];
 char *make=NULL;

 // Unpack args , and follow arg[0]...
 char **pack_ptr=(char **)pack;
 char **arg = pack_ptr;
 char tabledata[6660],randname[16],log[1024];
 char *pathsource=NULL;   

 
//if be POST
  if(arg[4]==NULL)
  {
    POST=0;
  } else POST=1;
  
  counter=char_type_counter(POST?arg[4]:arg[0],'!');
  old=counter;  
  chomp(line);

   while(old)
   {
    chunk.memory=NULL; 
    chunk.size = 0;  
//    make=(char *)malloc(sizeof(char)*( strlen(arg[4])+5023));
//payload with arguments ok
    make=payload_injector( (POST?arg[4]:arg[0]),line,old);
// send to POST on curl
    curl = curl_easy_init();
    curl_easy_setopt(curl,  CURLOPT_URL, POST?arg[0]:make);
    if(POST)
     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, make);
    curl_easy_setopt(curl,  CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl,  CURLOPT_WRITEDATA, (void *)&chunk);
    if(arg[6]!=NULL)
     curl_easy_setopt(curl,  CURLOPT_USERAGENT, arg[6]);
    else
     curl_easy_setopt(curl,  CURLOPT_USERAGENT, "Mozilla/5.0 (0d1n v0.1) ");
    curl_easy_setopt(curl,  CURLOPT_ENCODING,"gzip,deflate");
//    curl_easy_setopt(curl, CURLOPT_SETIMEOUT,3);
    if(arg[3]!=NULL)
    {
     curl_easy_setopt(curl,CURLOPT_COOKIEFILE,arg[3]);
     curl_easy_setopt(curl,CURLOPT_COOKIEJAR,arg[3]);
    } else 
     curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"odin_cookiejar.txt");
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0); 
    curl_easy_setopt(curl,CURLOPT_HEADER,1);
  
  
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&status);
    curl_easy_cleanup(curl);

//open response list 2 grep
    fp = fopen(arg[2], "r");
    if(!fp)
    { 
     puts("error to open response list"); 
     exit(0);
    }

      line=html_entities(line);
      make=html_entities(make);

    while(fgets(line2,1023,fp) != NULL) 
    {
// grep response
     chomp(line2);
     if(chunk.memory && strstr(chunk.memory,line2)!=NULL) 
     {
      fprintf(stdout,"%s [ %s%d%s ] Payload: %s %s %s Grep: %s %s %s  Params: %s %s\n",YELLOW,CYAN,status,YELLOW,GREEN,line,YELLOW,CYAN,line2,YELLOW,make,LAST);
      sprintf(log,"[%d] Payload: %s  Grep: %s Params: %s",status,line,line2,make);
      WriteFile(arg[5],log);
      pathsource=malloc(sizeof(char)*64);
      bzero(pathsource, sizeof(char)*64);
      strcat(pathsource,"response_sources/");
      strcat(pathsource,rand_str(randname, sizeof randname));
      strcat(pathsource,".html");
      WriteFile(pathsource,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\\
<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\\
<head>\\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\\
	<title>SyntaxHighlighter</title>\\
	<script type=\"text/javascript\" src=\"../tables/media/js/shCore.js\"></script>\\
	<script type=\"text/javascript\" src=\"../tables/media/js/shBrushXml.js\"></script>\\
	<link type=\"text/css\" rel=\"Stylesheet\" href=\"../tables/media/css/shCoreRDark.css\"/>\\
	<script type=\"text/javascript\">SyntaxHighlighter.all();</script>\\
</head>\\
<body style=\"background: black; font-family: Helvetica\">\\
<pre class='brush: xml'>");
      WriteFile(pathsource,html_entities(chunk.memory));
      WriteFile(pathsource,"</pre></html>");

// make the table lines
      sprintf(tabledata,"[\"<a href=\\\"../%s\\\">%d </a>\",\"%s\",\"%s\",\"%s\"],\n",pathsource,status,make,html_entities(line2),line);
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

 // Unpack args , and follow arg[0]...
 char **arg = (char **)arguments;
 char line[2024]; 

// open payload list and get lines
  fp = fopen(arg[1], "r");
  if(!fp)
  { 
   puts("error to open Payload list"); 
   exit(0);
  }
 
// delete out table
  unlink(TABLE);
//start array to use in datatables
  WriteFile(TABLE,"{ \"aaData\": [ \n");
 
  while(fgets(line,2023,fp) != NULL) 
   spider(arguments,line);
  
// end datables array
  WriteFile(TABLE," [\"\",\"\",\"\",\"\"]");
  WriteFile(TABLE,"] }");

  fclose(fp);

 puts(RED);
 puts("end scan \n look the tables/hammer.html");
 puts(LAST);


}
