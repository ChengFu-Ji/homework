##安裝套件
1.apt install inetutils-inetd
2.apt install telnet

##操作流程
1.啟動inetd監聽
2.Telnet連線，觸發inetd
3.觸發起動MySQL字典程式
4.輸入要查詢的國家編號
5.輸出查詢國家名稱

##BUG
1.inetd觸發啟動程式，如果未先輸入數值，printf將會無法顯示後續結果

##Log(正常直接執行)
root@raspberrypi:/home/pi/HW# ./mysqldict.out
請輸入國家編號：1
Mysql Connection Success
查詢結果：Taiwan	
Mysql Connection Closed
root@raspberrypi:/home/pi/HW# 

##Log(異常inetd)
root@raspberrypi:/home/pi/HW# telnet localhost 8787
Trying ::1...
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
1
請輸入國家編號：Mysql Connection Success
查詢結果：Taiwan	
Mysql Connection Closed
Connection closed by foreign host.
root@raspberrypi:/home/pi/HW# 

