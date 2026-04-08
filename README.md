# Kernel C Practice

用 Linux kernel 的 coding style 練習寫 C。每個 exercise 都模擬 kernel 裡的真實模式。

## 環境需求

- GCC (`sudo apt install gcc`)
- Make (`sudo apt install make`)
- Perl (`sudo apt install perl`) — for checkpatch

## 使用方式

```bash
# 編譯全部
make all

# 跑全部測試
make test

# 檢查 coding style
make check

# 編譯單一練習
make -C exercises/01-linked-list

# 清理
make clean
```

## 練習列表

| # | 主題 | 學到什麼 |
|---|------|----------|
| 01 | linked-list | `list_head`, `list_add`, `list_for_each_entry`, `container_of` |
| 02 | container-of | 結構體嵌入模式、從 member 回推 parent |
| 03 | error-handling | `goto` cleanup pattern、負數 errno 回傳 |

## Coding Style

遵循 Linux kernel coding style：
- Tab = 8 spaces
- 80 column line limit
- K&R brace style
- `scripts/checkpatch.pl` 檢查

## CI

GitHub Actions 自動跑：
1. `checkpatch.pl` coding style 檢查
2. `gcc -Wall -Werror` 編譯
3. 執行測試
