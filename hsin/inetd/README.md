# This is about inetd.

### 此為 inetd Helloworld 測試

另外需要 telnet 輔助完成測試

首先 在/home/pi touch test.c 

test.c 
```
#include<stdio.h>

int main()
{  
    printf("Helloworld\n");

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
----------
## 測試scanf

測試程式為簡易 scanf 程式

><mark>testscanf.c</mark>

---------

### 測試結果

```
Trying ::1...
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
you
me
ate
_exit
what the you
what the me
what the ate
Connection closed by foreign host.
```

---------

### 發現問題

### `文字並不會直接印出，而是在離開之後才會印出。`

---------

## 測試scanf with fflush

><mark>test2scanf.c</mark>

---------

### 測試結果

```
pi@hsin:~/code $ telnet localhost 20001
Trying ::1...
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
What's wrong with you ?
If you don't have any question. keyin _exit to left.
>>e
What is the e ?
>>yeee
What is the yeee ?
>>w
What is the w ?
>>weeee
What is the weeee ?
>>LOLOLOLOLOL
What is the LOLOLOLOLOL ?
>>_exit
Connection closed by foreign host.
```
--------

### 階段性結論

__*已知是buffer問題造成*__

使用 `fflush` 這個函數清空 buffer 後即可解決


