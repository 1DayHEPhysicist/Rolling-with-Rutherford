# 一日高能物理 Rolling with Rutherford
---
## Table of Contents
- [Prerequisite](#prerequisite)
- [Usage](#usage)
- [Contributing](#contributing)
- [Issues](#issues)
- [Contact](#contact)
---
### Prerequisite
- ROOT 
  - 官方網站: https://root.cern/
  - "An open-source data analysis framework used by high energy physics and others."
  - [如何安裝](https://root.cern/install/)
- 先備知識
  - **直方圖 (Histogram)**: 我們會使用ROOT裡的TH1類別 (class)。若想更深入了解TH1，請參考 [User guide](https://root.cern.ch/root/htmldoc/guides/users-guide/Histograms.html)
  - **常態分佈** (Normal distribution, 又稱高斯分佈、鐘型分佈): 我們會使用ROOT裡的TF1類別裡的高斯函數擬合直方圖。若想更深入了解TF1，請參考 [Class reference](https://root.cern.ch/doc/master/classTF1.html) 及 [User guide: Fitting histograms](https://root.cern.ch/root/htmldoc/guides/users-guide/FittingHistograms.html)

---
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

--- 
### Contributing
若你認為這些程式可以有更好的寫法、或想增加新功能到這些程式裡，你可以在這個程式庫 (Repository) 創建另一個分支 (Branch)。步驟如下
1. Fork 這個專案 (Project)
2. 創建分支。若你想為你的分支取名為 Myfeature，則執行指令
   ``git checkout -b Myfeature``

   上面的Myfeature只是舉例，你可以使用任何你想用的名字
3. 用 add 命令增加更新檔案
   ``git add *``
   \* 符號代表增加所有更新。若你只想增加特定檔案，則將 \* 改為該檔案的名稱
4. 提交 (commit) 增加的更新檔案，並簡短說明此提交的內容。假設你想添加的說明是“Update README”，則執行指令
   ``git commit -m "Update README"``
   上面的"Update README"只是舉例，你可以使用任何文字說明你的提交內容
5. 推送 (push) 至你的Myfeature分支
   ``git push origin Myfeature``
6. 檢查推送過程中是否有錯誤。若有錯誤，請上網Google錯誤內容及相對應的解決方式 (通常你遇到的錯誤內容別人都已經遇過也問過了，所以一般來說你一定能找到解決方法。不過，這些網路上的討論絕大多數都是英文，你需要有一定的英文能力才能理解)

---
### Issues
若你遇到任何問題，你可以在Issues的分頁裡創建一個新的Issue，並請盡量清楚描述下列3點:  
a.你遇到的問題 
b.程式或指令執行時在終端機顯示的錯誤內容 
c.你在網路上找到的解決方法，或是在網路上你認為可能的解決方法

---
### Contact
Email: ncu.onedayhep@gmail.com
Facebook: https://www.facebook.com/1DayHEPhysicist

--- 
程式撰寫: 鄭皓仁
README 文件編修: 鄭皓仁

Latest update on 2020/09/10
