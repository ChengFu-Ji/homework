This is about inetd.
--------
### 此為 inetd Helloworld 測試

另外需要 telnet 輔助完成測試

首先 在/home/pi touch test.c 

test.c 
```
#include<stdio.h>

int main()
{  
    printf("Hellowrld\n");

return 0;
}
```

編譯
```
gcc test.c -o test
```
---------
### 設定 /etc/services 、 /etc/inetd.conf

&ensp;

etc/services 這個檔案是用來設定port對應的服務

在最底下加上 
```
test    20001/tcp
```
儲存退出

&emsp;

etc/inetd.conf


設定需要inetd引導到的服務
```
test stream tcp nowait /home/pi/test
```
---------
### 重新開啟 inetd 服務

```
sudo killall inetd
sudo inetd
```
---------
### 測試
```
telnet localhost 20001
```
---------
### 測試結果
```
Trying ::1...
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Helloworld
Connection closed by foreign host.
```
