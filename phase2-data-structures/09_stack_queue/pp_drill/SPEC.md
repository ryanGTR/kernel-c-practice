# pp_drill：用指標的指標改 head（約 25 分鐘）

## 背景

10/16 的題目：`push_front(head, 5)` 是傳值呼叫，函式裡改的只是 head 的複本，
回到呼叫端後 head 沒有變。這次要寫的是能真的改到呼叫端 head 的版本。

Java 類比：`AtomicReference<Node> box`。`&head` 等於把盒子交給函式，
`*head` 是打開盒子拿到裡面的節點，`*head = n` 則是把盒子裡的東西換成 n。

## 資料結構（單向串列，已寫在 pp.h）

```c
struct node {
	int val;
	struct node *next;
};
```

## 要寫的三個函式

測試會**這樣呼叫**它們（10/17 依你要求補了框架：pp.h 已有宣告、pp.c 有空殼，可以編譯）：

| 呼叫方式 | 行為 | 回傳 |
|---|---|---|
| `push_front(&head, 5)` | 在最前面插入一個 val=5 的節點；head 要指向它 | 0＝成功；-1＝malloc 失敗 |
| `pop_front(&head, &out)` | 拿掉第一個節點、把它的 val 寫到 out、free 掉該節點 | 0＝成功；-1＝空串列（out 不動） |
| `list_free(&head)` | free 掉整條串列，結束時 head 必須是 NULL | 無（void） |

呼叫前 `struct node *head = NULL;`

## 規則

- 只改 `pp.c` 的三個函式本體。不要改 `test_pp.c`。
- 驗收：
  - `make test`：全部 ok。
  - `make asan`：沒有 sanitizer 報告。
  - `make style`：checkpatch 0 error。
- 寫之前先回答一題（寫在 pp.c 最上面的註解裡）：
  `push_front` 的第一個參數是什麼型別？為什麼不是 `struct node *`？
