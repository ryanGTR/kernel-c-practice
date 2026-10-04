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
