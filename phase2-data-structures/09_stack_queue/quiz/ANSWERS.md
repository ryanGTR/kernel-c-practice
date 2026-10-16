# ring buffer 間隔複習：標準答案（給批改用，Ryan 作答前別看）

## Day 1（10/05）
- Q1：第 i 個在 (3+i)%8，i=0..5 → 3,4,5,6,7,0；下一個 enqueue i=size=6 → (3+6)%8=1
- Q2：capacity 20、size 5

## Day 3（10/07）
- Q1：滿時繞回的是實體 0..head−1＝0,1,2 共 3 個 → 搬到 4,5,6（oldcap+j）；head 不變仍為 3；從 3 往右數 4 格＝3,4,5,6 ✓
- Q2：i=0 → head（排頭）。寫成 +1 會整體右移一格：排頭讀成第二個、最後一個讀到空格（off-by-one）

## Day 7（10/11）
- Q1：head==tail 同時代表空與滿（tail 繞一圈追上 head）。解法：①永遠空一格不用（滿＝(tail+1)%cap==head）②另存 size／count（本站做法）。kfifo：in/out 不取模一直加，size＝in−out，cap 為 2 的冪用 &(cap−1)
- Q2：ring buffer 用算的 (i+1)%cap（中間是記憶體隔壁、尾巴算回頭）；循環鏈表用 node->next。ring buffer 對 cache 友善：連續記憶體，一次載入一整段 cache line；鏈表節點散落，每步可能 cache miss

---

# lqueue 間隔複習：標準答案（給批改用，Ryan 作答前別看）

## Day 1（10/16）
- Q1：tail 要設成 NULL。漏了 → head 是 NULL 但 tail 指向已 free 的節點，下一次 enqueue 判斷「非空」走 else，執行 q->tail->next = node，寫進已釋放的記憶體 → use-after-free（10/15 ASan 實際抓到：queue.c:138，偏移 8 bytes＝next 欄位）。根本原因＝不變式 head==NULL ⇔ tail==NULL ⇔ size==0 被打破
- Q2：SIZE_MAX＝18446744073709551615（size_t 無號，-1 繞回最大值；-Wall -Wextra 不會警告）。Java 沒有 unsigned 整數，int -1 就是 -1

## Day 3（10/18）
- Q1：free(n) 之後 for 的第三段才讀 n->next → 讀已釋放的記憶體，ASan 報 heap-use-after-free。改：先存 next = cur->next，再 free(cur)，再 cur = next（同 lstack_destroy）
- Q2：stub 永遠回 0，剛好跟「清空時應該是 0」一樣，測試分不出對錯。要在答案「不是 0」的時間點問：enqueue(2)、enqueue(3) 之後 assert(lqueue_size(q) == 2)。驗證法：把 size 改回 stub 跑一次確認變紅，再改回來

- Q3（Day 1 回爐，10/16 作答兩題都半對）：Ryan 只答「head 改 NULL→segfault」沒點名 tail、沒說 use-after-free；Q2 答「size_t 是正整數」（應為非負）沒給值。標準：走 else，q->tail->next = node 寫進已 free 節點；(size_t)-1 = SIZE_MAX = 2^64−1
- Q4（10/16 push_front 加考）：void push_front(struct node **head, int v)；最後一行 *head = n;，* 在左邊＝寫入（右邊＝讀出）；& 才是取位址。10/16 他答錯序列：struct node → struct node * → &node → head=&n → 「開盒子=取值」→「查位址」
- Q5（10/16 r 題回爐）：*r＝pp（int **）、**r＝p（int *）、***r＝x（int）。10/16 他型別 3/3、格子把 **r 答成 pp（差一層＝第幾個 vs 走幾步老毛病）。int *q = *p：*p 是 int 值不是門牌（＝int *q = 1 陷阱）

## Day 7（10/22）
- Q1：第二行寫進「舊的尾巴」—— 這時 q->tail 還沒移動。[2] → enqueue(3) 後變成 [3] → [?]：舊節點的 2 被蓋成 3，新節點的 data 沒初始化（垃圾值）、next 也沒設。正解：先設 node->data = value、node->next = NULL，再接上去並移動 tail。（老弱點：寫到 q->tail／s->top 而不是當下節點）
- Q2：解讀成 (q->head == q->tail) == NULL。空佇列時 head == tail 為真 → 1，再比 1 == NULL（0）→ 假。所以空佇列反而進不了分支。-Werror=parentheses 會擋下來；Java 的 boolean == null 直接編譯不過
