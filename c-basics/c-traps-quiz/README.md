# C Traps and Pitfalls 題庫

題目出自 Andrew Koenig《C Traps and Pitfalls》（AT&T 技術報告版，29 頁）。
PDF 在 `~/Documents/linux_kernel/books/C.Traps.and.Pitfalls.Andrew.Koenig.pdf`。

## 玩法

- 在 TG 上一次出一題，用「預測輸出」或「找 bug」的形式，先猜再看答案。
- 每題答完就記進 `STATS.md`，邊做邊統計。
- 每題標一種類型：
  - **陷阱題**：正常人不會這樣寫。看得懂就好，錯了不代表不會寫 C。
  - **真功夫**：每天寫 code 都會碰到、避不開，要練到熟。
- 錯的題目再分兩種：**A＝提取 gap**（懂但想不起來）、**B＝模型 gap**（腦中的圖沒建好）。

## 檔案

| 檔案 | 用途 |
|---|---|
| `STATS.md` | 每題的成績與統計 |
| `ANSWERS.md` | 標準答案（都用 gcc 跑過驗證） |
| `verify/qNN.c` | 每題的驗證程式，`gcc -Wall -Wextra` 編譯 |
