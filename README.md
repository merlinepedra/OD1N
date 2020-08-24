## 0d1n
=====

![Alt text](https://github.com/CoolerVoid/0d1n/blob/master/doc/images/tables.png)
0d1n is a tool for automating customized attacks against web applications.


# Tool functions: 

> *Brute force login and passwords in auth forms

> *Directory disclosure ( use PATH list to brute, and find HTTP status code )

> *Test to find SQL Injection and XSS vulnerabilities 

> *Test to find SSRF

> *Test to find COmmand injection

> *Options to load ANTI-CSRF token each request

> *Options to use random proxy per request

> *other functions...

## to run and install follow this steps:

require libcurl-dev or libcurl-devel(on rpm linux based)

$ git clone https://github.com/CoolerVoid/0d1n/

 
You need libcurl to run, look the following to install::

```  
$ sudo apt-get install libcurl-dev

if rpm distro

$ sudo yum install libcurl-devel
```

To install follow this cmd:
```
$ cd 0d1n

$ make; sudo make install USER=name_your_user; 

$ cd 0d1n_view; make; sudo make install USER=name_your_user; 
```

Up the view server to look the reports online:

```
$ sudo 0d1n_view 

```

Now in other console you can run the tool:
```

$ 0d1n

```


## to uninstall follow this steps:


```
$ cd 0d1n; sudo make uninstall

$ cd 0d1n_view; sudo make uninstall

```



## Project Overview on cloc
```
cooler@gentoo:~/codes$ cloc 0d1n/
     937 text files.
     532 unique files.                                          
     451 files ignored.

-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
JavaScript                     361           9951          15621          52178
C                               51           4986           4967          26642
C/C++ Header                    30           1184           2858           4295
CSS                             10            434            369           2142
HTML                             7             59              0           1616
TeX                              2             52              4            206
Markdown                         3             81              0            137
make                             4             36              9            130
Bourne Shell                     2              0              0              4
-------------------------------------------------------------------------------
SUM:                           487          16835          23846          91213
-------------------------------------------------------------------------------

```


Read the docs...

Do you have any doubt about 0d1n? please create a issue in this repository, i can help you...



## To study  old versions look this following:

http://sourceforge.net/projects/odin-security-tool/files/?source=navbar

                             
