#include "file_ops.h"
#include "mem_ops.h"

//read lines of file
char *readLine(char * NameFile)
{
	FILE * file;
	file = fopen(NameFile, "r");

	if( !file )
	{
		puts("error in template file"); 	 
		exit(1);
	}

	char *lineBuffer=(char *)xmalloc(sizeof(char)*256), line[256];
	memset(lineBuffer,0,255);



	if( !lineBuffer )
	{
		fprintf(stdout,"error in readLine() at %s",NameFile);
		exit(1);
	}

	while( fgets(line,sizeof line,file) )  
	{
		lineBuffer=xrealloc(lineBuffer,strlen(lineBuffer)+strlen(line)+1);
		if( !lineBuffer )
		{
			fprintf(stdout,"error in readLine() at %s",NameFile);
			exit(1);
		}
		strncat(lineBuffer,line,strlen(lineBuffer)-1);
	}

	if(lineBuffer)
		xfree(lineBuffer);


 
	fclose(file);

	return lineBuffer;
}


int 
WriteFile(char *file,char *str)
{
	FILE *arq;
 
	arq=fopen(file,"a"); 

	if ( !arq ) 
	{
		fprintf(stdout,"error in WriteFile() %s",file); 
		exit(1);
	}

	fprintf(arq,"%s\n",str); 
	fclose(arq); 

	return 1;
}

// return size of bytes on file , same to unix cmd "du -b file"
long FileSize(const char *file)
{
	long ret;
	FILE *fh; 
        fh = fopen(file, "r");

	if ( !fh )
	{
		fprintf(stdout,"error in file");
		return 0;
	}

	fseek(fh, 0, SEEK_END);
	ret = ftell(fh);
	fclose(fh);

	return ret;
}
