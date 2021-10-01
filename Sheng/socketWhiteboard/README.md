# socketWhiteboard #

> 簡介：建立一個 GUI 介面（以下簡稱白板），讓使用者可以在該介面上隨意塗鴉，並將繪圖者的筆跡表現在其他使用者的白板上，功能上與 Teams 中的白板相似。

### 目前功能 ###
1.  繪圖板功能


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