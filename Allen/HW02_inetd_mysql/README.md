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


## Log(正常直接執行)
root@raspberrypi:/home/pi/HW# ./mysqldict.out<br />
請輸入國家編號：1<br />
Mysql Connection Success<br />
查詢結果：Taiwan<br />	
Mysql Connection Closed<br />
root@raspberrypi:/home/pi/HW#<br /> 


## Log(異常inetd)
root@raspberrypi:/home/pi/HW# telnet localhost 8787<br />
Trying ::1...<br />
Trying 127.0.0.1...<br />
Connected to localhost.<br />
Escape character is '^]'.<br />
1<br />
請輸入國家編號：Mysql Connection Success<br />
查詢結果：Taiwan<br />
Mysql Connection Closed<br />
Connection closed by foreign host.<br />
root@raspberrypi:/home/pi/HW#<br />