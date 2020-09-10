# 一日高能物理 Rolling with Rutherford

### Prerequisite
- ROOT 
  - 官方網站: https://root.cern/
  - "An open-source data analysis framework used by high energy physics and others."
  - [如何安裝](https://root.cern/install/)

### Usage
1. 將Rolling with Rutherford的數據存為.csv檔案格式 (csv的全名為comma-separated value)。要存取的變數請參考data路徑下的.csv檔案。檔案命名格式為 RWR_data_[日期].csv，如 RWR_data_20200905.csv。將.csv檔案移至data路徑底下
2. 將.csv檔案內容轉為[TTree結構](https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html)，並存至.root檔案格式。假設你要轉換的.csv檔案是在日期 20200905 時紀錄的，程式執行如下

    ```
    root -l -q -b RWR_csvtoroot.C\(20200905\)
    ```

    此程式預設會將data路徑下的.root檔案合併，合併後的檔名為 RWR_data_merge.root。若你不想做這件事，則程式執行如下

    ```
    root -l -q -b RWR_csvtoroot.C\(20200905,false\)
    ```
3. 將每10次滾動中的撞擊次數畫成直方圖 (histogram)。程式執行如下

    ```
    root -l -q -b RWR_plotdata.C
    ```

    此程式預設會讀取data路徑下的RWR_data_merge.root檔案。若你想使用合併前的單一.root檔案作圖 (以在20200905收集的數據.root檔案為例)，程式執行如下

    ```
    root -l -q -b RWR_plotdata.C\(\"./data/RWR_data_20200905.root\"\)
    ```

    程式執行結束後，直方圖會以.png及.pdf的檔案格式存取於plots路徑下。圖如下
    ![RWR_Ncoll_merge](/plots/RWR_Ncoll_merge.png)

4. 將每10次滾動中的撞擊次數畫成動態直方圖。程式執行如下

    ```
    root -l -q -b RWR_makegif.C
    ```

    此程式預設會讀取data路徑下的RWR_data_merge.root檔案。若你想使用合併前的單一.root檔案作圖 (以在20200905收集的數據.root檔案為例)，程式執行如下

    ```
    root -l -q -b RWR_makegif.C\(\"./data/RWR_data_20200905.root\"\)
    ```

    另外，程式預設為每1個事件更新一次直方圖。若你想更改此更新頻率為5 (更新頻率可為任意正整數，這裡以5為範例)，執行程式如下
    ```
    root -l -q -b RWR_makegif.C\(\"./data/RWR_data_merge.root,5\"\)
    ```

    程式執行結束後，直方圖會以.gif的檔案格式存取於plots路徑下。圖如下
    ![RWR_Ncoll_animation](/plots/RWR_animation.gif)

