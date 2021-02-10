## inetd

inetd 是個super deamon，它可以幫助我們去管理網路服務。

本次作業，讓我知道inetd它可以透過在inetd.conf的設定，使某些網路服務(ssh 、 telnet......)會先經過inetd後再去執行各個網路服務的deamon檔。

### services

```

transla   4444/tcp


```

### my_inetd

```

transla   stream  tcp nowait root /home/pi/C_sql/trnasla.o trnasla.o


```

## service 

    就我個人理解 service 是一個正在執行的 deamon 。

## deamon
   
   deamon 可以說是每個 service 的執行檔。
   
## result

![Minion](https://github.com/ChengFu-Ji/homework/blob/master/KAI_LUN/inetd/Transla.png)

