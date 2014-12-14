#include "file_ops.h"
#include "mem_ops.h"
#include "string_ops.h"

//read lines of file
char *readLine(char * NameFile)
{
	FILE * arq;

	arq = fopen(NameFile, "rx");
// todo think implement fcntl() ,toctou mitigation...
	if( arq == NULL )
	{
		DEBUG("error in to open() file"); 	 
		exit(1);
	}

	char *lineBuffer=xcalloc(1,1), line[4096];

	while( fgets(line,sizeof line,arq) )  
	{
		lineBuffer=xrealloc(lineBuffer,strlen(lineBuffer)+strlen(line)+1);
		strncat(lineBuffer,line,strlen(line)-1);
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

// write line in file
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



// returns random line from file
char *Random_linefile(char * namefile)
{
	FILE *f;
	int nLines = 0;
	static char line[1024];   // think recv space to nullbyte 1023
	int randLine=0,i=0;
 
	entropy_clock();  // i set entropy seed here

	memset(line,0x0,1023);

	f = fopen(namefile, "rx");

	if ( f == NULL )
	{
		DEBUG("error in file");
		exit(1);
	}

	while ( !feof(f) )
	{
		if(fgets(line, 1023, f)!=NULL) 
			nLines++;
	}

	randLine = rand() % nLines;

	fseek(f, 0, SEEK_SET);

	while (!feof(f) && i <= randLine)
		if(fgets(line, 1023, f)!=NULL)
			i++;
				
	if( fclose(f) == EOF )
	{
  		DEBUG("error in close() file %s",namefile);
		exit(1);
	}

	f=NULL;

    	return line;
}
