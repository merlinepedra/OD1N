##0d1n
=====
![Alt text](http://4.bp.blogspot.com/-5wsk7TYyfPs/UMYXZ8JTT2I/AAAAAAAAAo0/HyQa5hK_rkA/s1600/Barbarians-VIKING.gif)

0d1n is a tool for automating customized attacks against web applications.


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

 
need libcurl to run

  
$ sudo apt-get install libcurl-dev

if rpm distro

$ sudo yum install libcurl-devel

$ make

$./0d1n



Examples:


Brute path:
$ ./0d1n --host 'http://host/find.php?search=!&type=!' --payloads payloads/xss-rsnake.txt -find_string_list response2find/find.txt  --log out_data22


Brute GET n Find string:

$ ./0d1n --host 'http://coolerlab.wordpress.com/!' --payloads test_list1 --find_string_list find_list_one --log thunder27


$ firefox tables/hammer_thunder27.html



