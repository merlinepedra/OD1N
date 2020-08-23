## 0d1n
=====
![Alt text](https://github.com/CoolerVoid/0d1n/blob/master/doc/images/overview1.png)
0d1n is a tool for automating customized attacks against web applications.


# You can do: 

> *brute force login and passwords in auth forms

> *directory disclosure ( use PATH list to brute, and find HTTP status code )

> *test to find SQL Injection and XSS vulnerabilities 

> *Options to load ANTI-CSRF token each request

> *Options to use random proxy per request

> *other functions...


## Old Versions:

http://sourceforge.net/projects/odin-security-tool/files/?source=navbar

                             
![Alt text](https://github.com/CoolerVoid/0d1n/blob/master/doc/images/tables.png)

## to run:

require libcurl-dev or libcurl-devel(on rpm linux based)

$ git clone https://github.com/CoolerVoid/0d1n/

 
You need libcurl to run

```  
$ sudo apt-get install libcurl-dev

if rpm distro

$ sudo yum install libcurl-devel
```

To install follow this cmd:
```
$ cd 0d1n

$ sudo make install; 

$ cd 0d1n_view; sudo make install; sudo 
```

Up the view server to look the reports online:

```
$ sudo 0d1n_view &

```

Now you can run the tool:
```

$ 0d1n
```


Read the docs
