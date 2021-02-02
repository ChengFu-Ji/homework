## 2021/2/1
  1. 可透過 inetd 管理網路服務
  2. 新增一個 port，透過 telnet 使用新增的port去執行服務程式

## Program_Show

Indented code

  pi@larry:/etc $ telnet localhost 4321
  Trying ::1...

  Trying 127.0.0.1...

  Connected to localhost.

  Escape character is '^]'.

  This is KAI_sv

  Hello_world

  See you

  Connection closed by foreign host.i

## 2021/2/3
  1. 透過inetd 去執行SQL程式。
  2. SQL程式是做根據輸入英文 輸出中文。
  3. 目前SQL 的程式有的問題是 我輸入英文會需要 額外輸入' '。
  4. 還有用inetd 連線後程式跟連線資訊輸出順序有問題。
  5. 用自己寫的function 去做字串連接。

## Program_Success_Show

  Indented code

  pi@larry:/etc $ telnet localhost  4444
  Trying ::1...jnetd 管理網路服務
  2. 新增一個 port，透過 telnet 使用新增的port去執行服務程式
  
## Program_Show
  
Indented code
  
  pi@larry:/etc $ telnet localhost 4321
  Trying ::1...
  
  Trying 127.0.0.1...
  
  Connected to localhost.
  
  Escape character is '^]'.
  
  This is KAI_sv
  
  Hello_world
  
  See you
  
  Connection closed by foreign host.i
  
## 2021/2/3
  1. 透過inetd 去執行SQL程式。
  2. SQL程式是做根據輸入英文 輸出中文。
  3. 目前SQL 的程式有的問題是 我輸入英文會需要 額外輸入' '。
  4. 還有用inetd 連線後程式跟連線資訊輸出順序有問題。
  5. 用自己寫的function 去做字串連接。
  
## Program_Success_Show
 
Indented code
  
pi@larry:/etc $ telnet localhost  4444
Trying ::1...
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
'apple'
Connection success
input:
query made...
     蘋果
Connection closed by foreign host.

## Program_Fail_Show

Indented code

pi@larry:/etc $ telnet localhost  4444
Trying ::1...
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
apple
Connection success
input:
ERROR MAKING QUERY: Unknown column 'apple' in 'where clause'
Connection closed by foreign host.s
