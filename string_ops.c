#include "string_ops.h"

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
  while (*str) 
  {
    if (*str == '\n' || *str == '\r') 
    {
      *str = 0;
      return;
    }
    str++;
  }
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
