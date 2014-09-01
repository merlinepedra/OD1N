#include "file_ops.h"
#include "mem_ops.h"

//read lines of file
char *readLine(char * NameFile)
{
	FILE * arq;
	arq = fopen(NameFile, "rx");

	if( arq == NULL )
	{
		DEBUG("error in to open() file"); 	 
		exit(1);
	}

	char *lineBuffer=xcalloc(1,1), line[512];

	while( fgets(line,sizeof line,arq) )  
	{
		lineBuffer=xrealloc(lineBuffer,strlen(lineBuffer)+strlen(line)+1);
		strncat(lineBuffer,line,strlen(lineBuffer)-1);
	}

 
	if( fclose(arq) == EOF )
	{
		DEBUG("Error in close() file %s",NameFile);
		exit(1);
	}
	arq=NULL;

/*	if(lineBuffer != NULL)
	{
		free(lineBuffer);
	}
*/

	return lineBuffer;
}


int 
WriteFile(char *file,char *str)
{
	FILE *arq;
 
	arq=fopen(file,"a"); 

	if ( arq == NULL ) 
	{
		DEBUG("error in WriteFile() %s",file); 
		exit(1);
	}

	fprintf(arq,"%s\n",str); 

	if( fclose(arq) == EOF )
	{
		DEBUG("error in Write() file %s",file);
		exit(1);
	}
	arq=NULL;
 

	return 1;
}

// return size of bytes on file , same to unix cmd "du -b file"
long FileSize(const char *file)
{
	long ret;
	FILE *arq; 

        arq = fopen(file, "rx");

	if ( arq == NULL )
	{
		DEBUG("error in file");
		return 0;
	}

	fseek(arq, 0, SEEK_END);
	ret = ftell(arq);

	if( fclose(arq) == EOF )
	{
  		DEBUG("error in close() file %s",file);
		exit(1);
	}
	arq=NULL;

	return ret;
}
