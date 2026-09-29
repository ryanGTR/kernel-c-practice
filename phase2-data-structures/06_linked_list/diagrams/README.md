# 06_linked_list 圖解

照你目前的 `linked_list.c` 一步一張圖。重新產生：`python3 gen.py`（需要 `rsvg-convert`）。

| 圖 | 內容 |
|---|---|
| 01–04 | `list_append`：new → cur 從 head 出發 → 走到尾 → 接上 |
| 05–06 | `list_prepend`：新節點指向舊 head → 回傳新頭 |
| 07–09 | `list_find(list, 20)`：cur 一格一格走 |
| 10–13 | `list_delete(list, 20)`：prev/cur 兩根手指 → 繞過 → free |
| 14 | 挑戰：`list_delete(list, 5)` 刪 head——先猜再跑 |

圖例：紅色箭頭＝這一步改的指標；黃底＝新 malloc 的節點；綠底＝比對命中；虛線灰底＝已 free。
