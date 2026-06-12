# 03_my_echo -- 重新實作 echo

## 目標

重新實作 GNU coreutils 的 `echo` 指令。這是最簡單的專案，作為暖身練習。

## 行為規格

| 情境 | 行為 |
|------|------|
| 無引數 | 印出一個空行（只有 `'\n'`） |
| 有引數 | 印出所有引數，以空格分隔，末尾加換行 |
| `-n` 旗標 | 不加末尾換行 |

### 範例

```
$ ./my_echo hello world
hello world
$ ./my_echo -n hello
hello$
$ ./my_echo
(空行)
```

注意：`-n` 只在第一個引數時生效。`echo -n` 不印換行，`echo foo -n` 會印出 `foo -n`。

## 練習技能

- `argv` 迭代
- 簡單輸出（`printf` 或 `fputs`）
- 旗標解析的邊界條件

## 工具

- `gcc -Wall -Wextra -Werror` 編譯
- `make` 建置
- `diff` 與系統 echo 比對

## 完成清單

- [ ] `make` 零警告編譯通過
- [ ] `./my_echo hello world` 與 `echo hello world` 一致
- [ ] `./my_echo -n hello` 與 `echo -n hello` 一致
- [ ] `./my_echo`（無引數）印出空行
- [ ] 原始碼少於 30 行

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. 從 `argv[1]` 開始迭代，檢查是否為 `-n`
2. 引數間印空格：第一個不印，之後每個前面印一個空格
3. 最後根據 `-n` 旗標決定是否印 `'\n'`
4. 就這麼簡單，不要想太複雜

</details>
