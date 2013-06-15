##0d1n
=====
![Alt text](http://4.bp.blogspot.com/-5wsk7TYyfPs/UMYXZ8JTT2I/AAAAAAAAAo0/HyQa5hK_rkA/s1600/Barbarians-VIKING.gif)

This is just another security tool, to tests inputs on Web Applications.

Works similar to Burp "Intruder"...


#You can do: 

> *brute force passwords in auth forms

> *diretory disclosure ( use PATH list to brute, and find HTTP status code )

> *test list on input to find SQL Injection and XSS vulnerabilities 

> other things...

I explain in portuguese language here:

http://coolerlab.wordpress.com/2012/09/02/0d1n/                                


## to run:

require libcurl-dev or libcurl-devel(on rpm linux based)

$ git clone https://github.com/CoolerVoid/0d1n/

$ cd 0d1n; make; ./0d1n


Example to use:

$ ./0d1n -h 'https://github.com/CoolerVoid?tab=!' -p payloads/xss.txt -f response2find/find.txt -o log14

$ firefox tables/hammer_log14.html


More about on portuguese language:

http://blog.conviso.com.br/2012/11/um-pouco-sobre-teste-de-parametros-em_19.html
