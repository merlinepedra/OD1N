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


Read the docs...

Do you have any doubt about 0d1n? please create a issue in this repository, i can help you...



## To study  old versions look this following:

http://sourceforge.net/projects/odin-security-tool/files/?source=navbar

                             
