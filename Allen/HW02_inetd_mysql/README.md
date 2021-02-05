## 安裝套件
1. apt install inetutils-inetd
2. apt install telnet

## 操作流程
1. etc/servies文件內 加入自己的服務名稱及Port號
2. etc/inetd.conf文件內 設定監聽服務
3. inetutils-inetd restart 重啟服務
4. telnet localhost 8787 啟動inetd監聽
5. Telnet連線，觸發inetd
6. 觸發起動MySQL字典程式
7. 輸入要查詢的國家編號
8. 輸出查詢國家名稱


## BUG
1. inetd觸發啟動程式，如果未先輸入數值，printf將會無法顯示後續結果
20210205已解決 - 預設不使用緩衝區 
* setvbuf(stdout, NULL, _IONBF, 0);


## Log(正常直接執行)
root@raspberrypi:/home/pi/HW# telnet localhost 8787<br />
Trying ::1...<br />
Trying 127.0.0.1...<br />
Connected to localhost.<br />
Escape character is '^]'.<br />
<br />
Welcome Dict! Enter 0 Exit!<br />
<br />
Mysql 連線成功<br />
請輸入國家編號：1<br />
查詢結果：Taiwan<br />
<br />
請輸入國家編號：0<br />
Mysql 連線關閉<br />
Bye~<br />
<br />
Connection closed by foreign host.<br />

## 製作靜態Library
1. gcc -c mysqllib.c (產生 mysqllib.a)
2. ar rcs mysqllib.a mysqllib.o (使用 ar 命令創建靜態 Library)
3. gcc mysqldict.c mysqllib.a -l mysqlclient (連結 mysqllib.a)

