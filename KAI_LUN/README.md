## 2021/1/21
    我利用mysql.h 向SQL連線，且可以根據使用的輸入去執行指令，然後會show出執行結果。
## 2021/2/1
1. 可透過 inetd 管理網路服務
2. 新增一個 port，透過 telnet 使用新增的port去執行服務程式

[This is code link](https://github.com/ChengFu-Ji/homework/blob/master/KAI_LUN/inetd/c_inetd.c)
## 2021/2/3
1. 透過inetd 去執行SQL程式。
2. SQL程式是做根據輸入英文 輸出中文。
3. 目前SQL 的程式有的問題是 我輸入英文會需要 額外輸入' '。
4. 還有用inetd 連線後程式跟連線資訊輸出順序有問題。
5. 用自己寫的function 去做字串連接。

[This is code link](https://github.com/ChengFu-Ji/homework/blob/master/KAI_LUN/inetd/trnasla.c)

## 2021/2/5
1. 修改了[transla.c](https://github.com/ChengFu-Ji/homework/blob/master/KAI_LUN/inetd/trnasla.c)

