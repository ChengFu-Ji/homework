# This is about inetd

##此為inetd hello 測試

`首先在 /home/pi創立testinetd.c`

`testinetd.c`

```c
#include<stdio.h>
#include<stdlib.h>

int main(){
        printf("hello");
        printf("\n");

        return 0;
}

```
`編譯`

`gcc -o testinetd testinetd.c`

---

##設定/etc/services、/etc/inetd.conf

`etc/servies是用來設定服務的相對應port`

`在最底下加上`

`testinetd 3333/tcp`

`儲存完並退出`

`etc/inetd.conf`
`設定需inetd引導到的服務`

```testinetd stream tcp nowait root /home/pi/testinetd testinetd

```
---
##重新開啟inetd服務


```
sudo /etc/init.d/inetutils-inetd start
```
---
##測試

```telnet localhost 3333
```
---

##測試結果

```
Trying ::1...Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
hello
Connection closed by foreign host.
```
