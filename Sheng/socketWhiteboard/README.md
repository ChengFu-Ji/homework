# socketWhiteboard #

> 簡介：建立一個 GUI 介面（以下簡稱白板），讓使用者可以在該介面上隨意塗鴉，並將繪圖者的筆跡表現在其他使用者的白板上，功能上與 Teams 中的白板相似。


※ 1/12 更新 master ， 將貝茲曲線函式獨立成一個檔案，並更動 linkedlist struct 名稱，以及 linkedlist 、 server 及 whiteboard 中相對應的修改。
※ 1/26 更新 master ， 實作按鈕觸發、新增頁面功能以及翻頁功能。

### 目前功能 ###
1.  繪圖板功能
2.  連線功能
3.  server
4.  同時繪圖功能


### openCV 及 cmake ### 

本次作品中的白板使用 openCV 搭配 cmake 產生 Makefile 製作執行檔。

※ 不同作業環境下方法不一

在 ubuntu 安裝 openCV 函式庫:

<pre><code> sudo apt-get install libopencv-dev </code></pre> 

在 ubuntu 中安裝 cmake :

<pre><code> sudo apt-get install cmake </code></pre>

#### 使用方法 ####
在需編譯之 cpp 檔的同目錄下，建立一個名為 CMakeLists.txt 之檔案並寫入參數及設定 (CMakeLists.txt 的內容可參考本目錄下之同名檔案)，完成後輸入:

<pre><code> cmake [需編譯檔案之目錄] </code></pre>

隨後便會產生 CMakeFiles 資料夾，以及 CMakeCache.txt、cmake-install.cmake 和 Makefile 檔案。
CMakeFiles 資料夾內有建立檔案所需的函式庫位置以及一些目前我尚未看懂的檔案，CMakeCache.txt 及 cmake-install.camke 檔案的功能目前我目前也不能確定，這個部分後面如果有看懂會再更新。

得到 Makefile 檔案後，便可以產生出執行檔。

### 速度測試 ###

> 簡介：為了解 每個點傳送 和 一筆畫畫完再傳 兩者的速度誰比較快，所以才開始實作

測試檔案皆放置在 timeTest 資料夾中，當中只放置有更改之檔案，其中 dotTp 資料夾中放置邊畫邊傳版本的 client 。

#### 實驗設計 ####

~~原先是打算將傳送前後都搜集，再比對結果，但因為如果使用該方法 clock_gettime 取得值時會有一些值差距非常大，後面想說也用不到，所以放棄此方法。~~

10/18 更新: 在重新暸解 struct timespec 時，發現到 timespec 的 tv_nsec 在每次到達一秒後會歸 0 ，所以才會有差距非常大的現象發生， tv_sec 則會加 1 ，目前計算時間的作法已經改進。
下次更新時會補上。

目前方式是分別在 一筆畫完傳完後 及 每個點傳送的最後一個點傳送完後 用 clock_gettime 列印出目前的時間 (單位: nanosecond )

11/1 更新: 實作方法和上次相同，只差在於時間計算的方式更改

#### 實驗步驟Ver.1 ####

1.  先將 server 建立起來
2.  開啟 client (一筆畫完版本)
3.  畫一筆畫
4.  紀錄時間
5.  關閉 client
6.  開啟 client (邊畫邊傳版本)
7.  重複步驟 3 ~ 5

以上步驟重複四遍，即為本次測試完整流程

#### 實驗步驟Ver.2 ####

1.  建立 server 
2.  開啟多個 client (目前為十個，一筆畫完版本和邊畫邊傳版本，各五個)
3.  自動畫一百筆畫
4.  將時間記錄成檔案

以上動作皆由 shell script 完成

#### 實驗結果Ver.1 ####

目前的結果是，邊畫邊傳 的速度是最快的

<table>
<tr>
   <td> </td>
   <td> 一筆再傳 (單位:s) </td>
   <td> 邊畫邊傳 (單位:s) </td>
</tr>
<tr>
   <td> 第一次 (筆畫較長) </td>
   <td> 0.00059 </td>
   <td> 0.000147 </td>
</tr>
<tr>
   <td> 第二次 </td>
   <td> 0.000239 </td>
   <td> 0.000141 </td>
</tr>
<tr>
   <td> 第三次 </td>
   <td> 0.000127 </td>
   <td> 0.000095 </td>
</tr>
<tr>
   <td> 第四次 </td>
   <td> 0.000185 </td>
   <td> 0.00008 </td>
</tr>
</table>

#### 實驗結果Ver.2 ####

目前資料儲存於 timeTest/logs 之資料夾中。

目前稍微看過，邊畫邊傳 的速度依舊為最快，但資料太多，單純看過沒有辦法確認，所以下次更新會改為全部紀錄的平均值。

11/5 更新: 已經將紀錄更新為所有資料的平均值。 整理時間及計算平均之程式已放入 timeTest/logs 資料夾中。

### 速度測試衍伸 ###

在執行速度測試時所遇到之問題。

#### blocking ####

測試程式在 timeTest/blocking 中

當程式執行需要系統呼叫之 function 時，在等待資料時的狀態稱之 blocking 。
write() 在呼叫時，如果 buffer 空間已滿，也會進入 blocking 。

##### 實驗假設 #####

執行 write() 給 client 是有先寫入 buffer 的，卡住原因是 buffer 滿了，導致 server 進入 block 狀態。 

##### 實驗方法 #####

1. 先測試 server 是否能寫入成功
2. 在 server 收到一定資料量後 client 開始使用 read()，查看總資料量有多少
3. 在 server 進入 block 狀態後，client 開始使用 read()，觀察是否能離開 block 的狀態

##### 結論 #####

因為 server 執行 write() 是成功的，而且收取到的資料量和計算出來的相同，所以代表是有 buffer 的。

而在 server 進入 block 狀態後，的確能離開 block 狀態，代表是因為 buffer 滿了才進入 block 狀態的。
