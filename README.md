# HOMEWORK

## Install Tool
```
$ sudo apt-get -y update
$ sudo apt-get -y install make gcc build-essential
$ sudo apt-get -y install git vim
```

## Homework list
### MySQL using C API
1. 先在pi 上面裝 MySQL
2. 建立資料庫 建立資料表 新增資料到資料表
3. 做到關聯式資料庫

### 使用 inetd 建構自己的 deamon
1. 研究 inetd
2. 使用 inetd 管理 pi 上面的網路服務
3. 使用Ｃ語言寫出可以跟 inetd 配合的服務
4. 在 github 撰寫 README.md 分享過程，紀錄使用的方法以及研究成果，請使用 markdown 語法將文章寫好

### Linked-list
1. 以 Linked List 實作以下command功能
    - ADD,data （增加 data 至 Linked List
    - DEL,data （從 Linked List 刪除 data
    - Save,FileName （將目前的Linked List存至檔名為 FileName的檔案中
    - Load,FileName （將檔名為FileName的檔案內容讀入 Linked List    
    - ShowList (將現在的 Linked List 印出來

2. Format -> CMD,data
3. data 為最大長度 101 bytes 的字串
4. 使用 Makefile 進行編譯
5. 使用 markdown撰寫報告

## Reference
1. [Git tutorial](https://backlog.com/git-tutorial/tw/)
2. [TCP/IP](http://www.pcnet.idv.tw/pcnet/network/network_ip_tcp.htm)
3. [TCP package spec](https://tools.ietf.org/html/rfc793)
4. [UDP package spec](https://tools.ietf.org/html/rfc768)
5. [What is Socket](https://beej-zhtw-gitbook.netdpi.net/)
6. [VI](http://www2.nsysu.edu.tw/csmlab/unix/vi_command.htm)
