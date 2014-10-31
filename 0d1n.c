/*
Author:Antonio Costa aka " Cooler_ "
contact: c00f3r[at]gmail[dot]com


    Copyright (C) 2012 0d1n authors,
    
    This file is part of 0d1n
    
    0d1n is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    0d1n is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.



What this IT ?
this is another Web POST,GET test tool lol

need libcurl to run

$ sudo apt-get install libcurl-dev
if rpm distro
$ sudo yum install libcurl-devel
$ make
$./0d1n


*/
#include <stdio.h> 
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/resource.h>
#include "spider.h"
#include "validate.h"

void init_banner_odin()
{
 puts(
 CYAN
   "       ~.             ~             \n"                      
   "         01...___|__..10.              \n" 
   "          1010   101   101             \n" 
   "           0101  :Bug  :Sec   `.oo'   \n"
   "           :101  |010  |101  ( (`-'   \n"
   " .---.    1010  ;110  ;010   `.`.     \n"
   "/ .-._)  111-\"\"\"|\"\"\"'-000      `.`.   \n"
   "( (`._) .-.  .-. |.-.  .-.  .-. ) ) \n"
   " \\ `---( 1 )( 0 )( 1 )( 1 )( 0 )-' /  \n"
   " `.    `-'  `-'  `-'  `-'  `-'  .'   \n"
   "   `---------------------------'     \n"
 YELLOW
 "0d1n Web Hacking Tool 1.9 RELEASE\n"
 LAST
 "--host :	host to scan or GET method  site.com/page.jsp?var=^&var2=^\n"
 "--post :	POST method params  ex: 'var=^&x=^...'\n"
 "--cookie :      COOKIE  params  ex: 'var=^&var2=^...'\n"
 "--method :      Custom http method like DELETE, PUT, TRACE, CONNECT... \n"
 "--header :      Add line on http header \n"
 "--payloads :	payload list to inject\n"
 "--find_string_list :	strings list to find on response\n"
 "--find_regex_list :	regex list to find on response(this regex is posix)\n"
 "--cookie_jar :	 cookie jar file to load\n"
 "--log :	output of result\n"
 "--UserAgent :	custom UserAgent\n"
 "--CA_certificate :	Load CA certificate to work with SSL\n"
 "--SSL_version :	choice SSL version  \n	1 = TLSv1\n	2 = SSLv2\n	3 = SSLv3\n"
 "--theads : Number of threads to use, default is 4\n"
 "--timeout :	timeout to wait Response\n"
 "--proxy :   proxy_address:port to use single proxy tunnel\n	example: format [protocol://][user:password@]machine[:port]\n"
YELLOW
 "\nEnable-options-args:\n"
LAST
 "--save_response :   Enable save response highlights view when you click at http status code in datatables \n"
 "--json_headers :   Enable add JSON headers in Request \n\n"
 YELLOW
 "example 1 to find SQL-injection:\n"
LAST
"./odin --host 'http://site.com/view/1^/product/^/' --payloads payloads/sqli_list.txt --find_string_list sqli_str2find_list.txt --log log1337 --threads 5 --timeout 3 --save_response\n"
"\n"
YELLOW
 "example 2 to Bruteforce in simple auth:\n"
LAST
"./odin --host 'http://site.com/auth.py' --post 'user=admin&password=^' --payloads payloads/wordlist.txt --log log007 --threads 10 --timeout 3\n"
"\n"
YELLOW
"Notes:\n"
LAST
"Look the character '^', is lexical char to change to payload list lines...\n"
CYAN
 "Coded by Cooler_\n c00f3r[at]gmail[dot]com\n "
 );
 puts(LAST);
}

static struct option long_options[] =
{
	{"host", required_argument, NULL, 'h'},
	{"payloads", required_argument, NULL, 'p'},
	{"find_string_list", required_argument, NULL, 'f'},
	{"find_regex_list", required_argument, NULL, 'z'},
	{"cookie_jar", required_argument, NULL, 'c'},
	{"cookie", required_argument, NULL, 'i'},
	{"post", required_argument, NULL, 'P'},
	{"method", required_argument, NULL, 'b'},
	{"header", required_argument, NULL, 'd'},
	{"log", required_argument, NULL, 'o'},
	{"UserAgent", required_argument, NULL, 'u'},
	{"CA_certificate", required_argument, NULL, 's'},
	{"SSL_version", required_argument, NULL, 'V'},
	{"threads", required_argument, NULL, 't'},
 	{"timeout", required_argument, NULL, 'T'}, 
 	{"proxy", required_argument, NULL, '1'},
	{"save_response", no_argument, 0, 'k'},	
	{"json_headers", no_argument, 0, 'j'},
	{NULL, 0, NULL, 0}
};


int 
main(int argc, char ** argv)
{
 char c;
 char *pack[18]; 
 short y=17;

 	no_write_coredump ();
 	load_signal_alarm ();


	if(argc < 7) 
	{
		init_banner_odin();
		DEBUG(" Need more arguments.\n");
		exit(0);
	}
 

 	while(y)
 	{
  		pack[y]=NULL;
  		y--;
 	}

 	opterr = 0;

 	while((c = getopt_long(argc, argv, "h:p:f:z:c:P:b:d:o:u:s:t:T:1:k:j:V",long_options,NULL)) != -1)
  		switch(c) 
  		{
// Host
   			case 'h':
    				
				if ( strnlen(optarg,256)<= 128 )
				{
    					pack[0] = optarg;
					validate_hostname(pack[0]);
    					printf("Host: %s \n",pack[0]);
    					
				} else {
					DEBUG("Error \nArgument Host very large \n");
					exit(1);
				}
				break;
// payload list
			case 'p':
				if ( strnlen(optarg,256)<= 128 )
				{
    					pack[1] = optarg;
    					printf("Payloads: %s \n",optarg);
    					
				} else {
					DEBUG("Error \nArgument Payloads very large  \n");
					exit(1);
				}
				break;

   			case 'f':    				
				if ( strnlen(optarg,256)<= 128 )
				{
    					pack[2] = optarg;
    					printf("Find list: %s \n",optarg);
    				} else {
			
					DEBUG("Error \nArgument Find list file is very large  \n");
					exit(1);
				}
				break;

   			case 'z':			
				if ( strnlen(optarg,256)<= 128 )
				{
    					pack[10] = optarg;
    					printf("Regex list: %s \n",optarg);
    				} else {
					DEBUG("Error \nArgument Regex file is very large  \n");
					exit(1);
				}
				break;

   			case 'c':
				if ( strnlen(optarg,256)<= 64 )
				{
    					pack[3] = optarg;
				} else {

					DEBUG("Error \nArgument cookie jar file is very large  \n");
					exit(1);
				}
    				break;



   			case 'i':
				if ( strnlen(optarg,256)<= 64 )
				{
    					pack[13] = optarg;
				} else {

					DEBUG("Error \nArgument cookie is very large  \n");
					exit(1);
				}
    				break;

   			case 'P':
				if ( strnlen(optarg,512)<= 256 )
				{
    					pack[4] = optarg;
				} else {
					DEBUG("Error \nArgument POST very large  \n");
					exit(1);
				}
    				break;

   			case 'b':
				if ( strnlen(optarg,14)<= 12 )
				{
    					pack[15] = optarg;
				} else {
					DEBUG("Error \nArgument method is very large  \n");
					exit(1);
				}
    				break;

   			case 'd':
				if ( strnlen(optarg,129)<= 128 )
				{
    					pack[14] = optarg;
				} else {
					DEBUG("Error \nArgument header is very large  \n");
					exit(1);
				}
    				break;

   			case 'o':
				if ( strnlen(optarg,256)<= 128 )
				{
    					pack[5] = optarg;
    					printf("Log file: %s \n",optarg);
    				} else {
					DEBUG("Error \nArgument Log file very large \n");
					exit(1);
				}
				break;
   
   			case 'u':
				if ( strnlen(optarg,256)<= 128 )
				{
    					pack[6] = optarg;
    				} else {	
					DEBUG("Error \nArgument user agent is very large  \n");
					exit(1);
				}
				break;
 
   			case 's':
				if ( strnlen(optarg,256)<= 128 )
				{
    					pack[7] = optarg;
				} else {	
					DEBUG("Error \nArgument ca cert file name is very large  \n");
					exit(1);
				}
				break;

 
   			case 't':
				if ( strnlen(optarg,4)<= 2 )
				{
    					pack[11] = optarg;
				} else {	
					DEBUG("Error \nArgument threads is very large  \n");
					exit(1);
				}
				break;
 
   			case 'T':
				if ( strnlen(optarg,4)<= 3 )
				{	
    					pack[8] = optarg;
				} else {	
					DEBUG("Error \nArgument timeout is very large need 3 digit  \n");
					exit(1);
				}
				break;

// proxy single
   			case '1':
				if ( strnlen(optarg,47)<= 48 )
				{	
    					pack[17] = optarg;
				} else {	
					DEBUG("Error \nArgument proxy is very large \n");
					exit(1);
				}
				break;
 // save response
   			case 'k':
    				pack[12] = "1";
				break;
 	
		
   			case 'j':
    				pack[16] = "1";
				break;
 			
 				
   			case 'V':
				if ( strnlen(optarg,3)<= 1 )
				{	
    					pack[9] = optarg;
				} else {	
					DEBUG("Error \nArgument SSL version one digit example 1,2 or 3 : \n1 is TLSv1\n2 is SSLv2\n3 is SSLv3\n 0 is default\n");
					exit(1);
				}
				break;

   			case '?':
    				if(optopt == 'h' || optopt == 'p' || optopt == 'f' || optopt == 'c' || optopt == 'P' || optopt == 'o' || optopt=='s') 
    				{
     					init_banner_odin();
     					puts(RED);
     					DEBUG("Option -%c requires an argument.\n", optopt); 
     					puts(LAST);
     					exit(1);
    				}
				break;
  		}

 

	if(strlen(pack[0]))
		scan((void *)pack);

 	exit(0);
}

