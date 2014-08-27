#include "file_ops.h"
#include "mem_ops.h"

//read lines of file
char *readLine(char * NameFile)
{
	FILE * file;
	file = fopen(NameFile, "rx");

	if( file == NULL )
	{
		DEBUG("error in to open() file"); 	 
		exit(1);
	}

	char *lineBuffer=xcalloc(1,1), line[256];
//	memset(lineBuffer,0,1);

	while( fgets(line,sizeof line,file) )  
	{
		lineBuffer=xrealloc(lineBuffer,strlen(lineBuffer)+strlen(line)+1);
		strncat(lineBuffer,line,strlen(lineBuffer)-1);
	}

	if( lineBuffer != NULL )
		free(lineBuffer);


 
	if( fclose(file) == EOF )
	{
		DEBUG("Error in close() file %s",NameFile);
		exit(1);
	}

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
 

	return 1;
}

// return size of bytes on file , same to unix cmd "du -b file"
long FileSize(const char *file)
{
	long ret;
	FILE *fh; 
        fh = fopen(file, "rx");

	if ( fh == NULL )
	{
		DEBUG("error in file");
		return 0;
	}

	fseek(fh, 0, SEEK_END);
	ret = ftell(fh);

	if( fclose(fh) == EOF )
	{
  		DEBUG("error in close() file %s",file);
		exit(1);
	}

	return ret;
}
