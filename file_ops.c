#include "file_ops.h"

char *readLine(char * NameFile)
{
 FILE * file;
 file = fopen(NameFile, "r");
 if(!file)
 {
  puts("error in template file"); 	 
  exit(0);
 }
 char *lineBuffer=calloc(1,1), line[128];

 if(!lineBuffer)
 {
  fprintf(stdout,"error in readLine() at %s",NameFile);
  exit(0);
 }

 while(fgets(line,sizeof line,file))  
 {
  lineBuffer=realloc(lineBuffer,strlen(lineBuffer)+strlen(line)+1);
  if(!lineBuffer)
  {
   fprintf(stdout,"error in readLine() at %s",NameFile);
   return NULL;
  }
  strcat(lineBuffer,line);
 }

 return lineBuffer;
}


int WriteFile(char *file,char *str)
{
 FILE *arq;
 
 arq=fopen(file,"a"); 

  if(!arq) 
  {
   fprintf(stdout,"error in WriteFile() %s",file); 
   exit(0);
  }

 fprintf(arq,"%s\n",str); 
 fclose(arq); 

return 1;
}
