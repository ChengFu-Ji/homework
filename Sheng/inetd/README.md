<h3>inetd</h3>

網路上大多的說法是 在現今版本中的inetd 已經被 xinetd 取代 , 不過還是可以透過套件的方式取得 
但是這與原版的inetd有什麼區別 我不太了解

xinetd 與 inetd 的差別 自己使用上感覺是差不多 只是xinted 有較多參數可以設定

在設定一個自己的服務前 首先到/etc/services 新增一個port 例如:

<pre><code>mytest	1234/tcp</code></pre> 

設定好後便可到/etc/inetd.conf 設定一個自己所需要的服務 例如:

<pre><code>"service name" "sock type" "proto" "flags" "user" "server path" "args"
  mytest&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;stream  &emsp;&emsp;  tcp     nowait  root  /home/pi/Doc/inetd/myinetdtest </code></pre> 


設定好後重啟 inetd 服務 便可使用你所寫的服務了!

連線方式

<pre><code>telnet localhost 1234</code></pre>

<h3>myservice</h3>

目的是做到能夠透過mysql將英文單字翻譯成中文

目前的進度已經與預期的結果相符

<h4>fflush 的使用</h4>

為了將在buffer中儲存的值全部清空並輸出 而使用fflush

至於為何stdout的buffer會變為程式執行完後才全部輸出

我的猜想是 因為當我們藉由telnet連線到掛在inetd下我的服務時 我們的stdin是鍵盤, 輸入的對象則是inetd 

inetd 的stdout則會導向到我們執行檔的stdin, 執行檔的stdout 則維持不變為模擬終端介面

在預設情況下 stdout 的 buffered mode 採用 fully buffered, 

例外 (也就是 I/O 為 鍵盤/模擬終端介面) 時, stdout 的 buffered mode採用 line buffered

當我們用telnet 連線到我們的程式的時候, input則為inetd output則為模擬終端界面 

不符合例外狀況 所以會採用 fully buffered 

所以我使用fflush(stdout) 是為了將buffer中儲存的值全部列印出

<h5>tip : fully buffered & line buffered</h5>
<ul>
  <li>fully buffered 在 setvbuf() 中提到 表現為當程式結束或者buffer空間滿了才會進行傳輸</li>
  <li>line buffered 在 setvbuf() 中提到 表現為當遇到換行符號'\n'時便進行傳輸</li>
</ul> 
	
以上為猜想 目前沒有辦法 佐證我的想法 但有些參考資料 如有錯誤 還請指教 謝謝

<a href="https://www.gnu.org/software/libc/manual/html_node/Buffering-Concepts.html"> 參考資料 buffering-concept </a>

參考資料尚未補齊


