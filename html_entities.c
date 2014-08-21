#include "html_entities.h"
#include "mem_ops.h"
// implementation by IAK 

/**
 * Lista de caracteres que devem ser substituídos
 */
char* html_tbl = "<>\"\\";    /* áéíóú"; VEJA o TODO abaixo */

/**
 * Lista de caracteres para dar replace em html_tbl
 */
char* repl_tbl[] = {
	"&lt;",
	"&gt;",
	"&quot;",
        "\\\\"
/*	"&aacute;",  TODO: Para implementar isso tem que usar strings unicode (wchar_t)
	"&eacute;",
	"&iacute;",
	"&oacute;",
	"&uacute;"*/
	
};


int get_entity(char c) 
{
	char* ptr = html_tbl;
	unsigned idx = 0;

	while (*ptr) 
	{
		if (*ptr == c)
		return idx;
		ptr++;
		idx++;
	}	
	return -1;
}

char* html_entities(char* text) 
{
	char* ptr = text;
	unsigned retsize = 1;
	char* ret = NULL;
	
	ret = xrealloc(ret, retsize);
	
	memset(ret,0,retsize);
	
	while (*ptr) 
	{
		char c = *ptr;
		int repl_idx = 0;

		if((repl_idx = get_entity(c)) != -1) 
		{
			char* repl_str = repl_tbl[repl_idx];

			retsize += strlen(repl_str);
			ret = xrealloc(ret, retsize);
			strcat(ret, repl_str); // safe, repl_str não vem do usuário
		} else {
			retsize++;
			ret = xrealloc(ret, retsize);
			strcat(ret,&c); // safe, buffer foi aumentado
		}		
		ptr++;
	}
	
	return ret;
}

