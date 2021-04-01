## Linked list 
### 將每一筆資料串接起來
-------
#### 每筆資料有下筆資料的位置
通常程式會
由 First 、 Current 、Previous 組合

--------
而資料通常是採用 struct 型態組成

--------
#### reference

Linked list: https://lakesd6531.pixnet.net/blog/post/329288496
https://www.cnblogs.com/oomusou/archive/2008/03/22/1117686.html
https://pisces1026.wordpress.com/2017/09/21/cc-linked-list/

struct:https://programming.im.ncnu.edu.tw/Chapter13.htm


--------
### show_n_line function.
其功能為顯示檔案第N筆資料.

我的方法分為兩種,現在共存於此 funcition 內,以是否有 .index_ 檔案作為判斷使用:


方法一:
從此程式 save function 成檔案後產生 .index_ 檔案,
藉由此檔案作為每個資料位置的索引,
使用fseek() 移動至指定位置取得檔案中的資料位置。


方法二:
每筆資料為\n 表示結束,
藉此從頭開始計算並記數至指定筆數。
