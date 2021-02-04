<h3>inetd</h3>

網路上大多的說法是 在現今版本中的inetd 已經被 xinetd 取代 , 不過還是可以透過套件的方式取得 
但是這與原版的inetd有什麼區別 我不太了解

在設定一個自己的服務前 首先到/etc/services 新增一個port 例如:

<pre><code>mytest	1234/tcp</code></pre> 

設定好後便可到/etc/inetd.conf 設定一個自己所需要的服務 例如:

<pre><code>
"service name" "sock type" "proto" "flags" "user" "server path" "args"
mytest	stream	tcp nowait root /home/pi/Doc/inetd/myinetdtest </code></pre> 


設定好後重啟 inetd 服務 便可使用你所寫的服務了!

連線方式

<pre><code>telnet localhost 1234</code></pre>


