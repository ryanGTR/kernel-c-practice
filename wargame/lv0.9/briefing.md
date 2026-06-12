# LV0.9 補給站 — char device 前置知識簡報

> 你說「沒辦法繼續想下去」— 正常，LV0.9 是 wargame 第一次需要**kernel 自己的概念**（前面幾關靠 C 就能打）。
> 這份簡報補五個概念，每個都配 Java 類比 + 一題小測。讀完再回去看 chardev.c 的三個 TODO，它們會從「咒語」變成「廢話」。
> 搭配主幹教材：LKMPG **Character Device Drivers** 章（讀這份簡報 → 讀 LKMPG 該章 → 動手）。

---

## 概念 1：/dev 與 major/minor — 裝置檔只是「電話號碼」

`/dev/lv09` 不是檔案內容，是**轉接代碼**：

- 它身上記著兩個數字：**major**（找哪個 driver）、**minor**（同 driver 的第幾個裝置）
- userspace `open("/dev/lv09")` 時，VFS 看 major 查表 → 找到你註冊的 driver → 之後的 read/write 全部轉給你

**Java 類比**：major number 像 Spring 的 bean name — `mknod /dev/lv09 c 240 0` 等於寫一行設定檔：「名字 lv09 綁到 bean #240」。檔案系統只是 naming service。

> 小測 1：`mknod /dev/foo c 240 0` 和 `mknod /dev/bar c 240 1` — 這兩個節點會打到同一個 driver 嗎？driver 怎麼分辨？

---

## 概念 2：file_operations — kernel 的 interface 實作表

```c
static const struct file_operations fops = {
        .owner   = THIS_MODULE,
        .read    = dev_read,      /* userspace read()  → 這裡 */
        .open    = dev_open,      /* userspace open()  → 這裡 */
        .release = dev_release,   /* userspace close() → 這裡 */
};
```

這就是 C 的「實作 interface」：一張**函數指標表**（你 Day 33 qsort 的 comparator 放大版）。

**Java 類比**：

```java
class Lv09Device implements KernelFile {     // struct file_operations
    public int read(...)  { ... }           // .read  = dev_read
    public int open(...)  { ... }           // .open  = dev_open
    public int close(...) { ... }           // .release = dev_release
}
```

差別：Java 用 vtable 自動派發，C 手動填表。kernel 整個驅動模型都是這招 — Day 35 greybus 的 `.probe`/`.disconnect` 同款。

> 小測 2：你沒填 `.write` — userspace 對 /dev/lv09 呼叫 write() 會發生什麼？（猜完查：VFS 對 NULL 的預設行為，提示 `-EINVAL` 或 `-ENOSYS` 方向）

---

## 概念 3：`__user` 與 copy_to_user — 不可信指標（本關核心）

`dev_read` 的參數 `char __user *buf` — 這個指標**來自 userspace，kernel 不能直接 deref**：

1. 它可能根本是爛地址（惡意或 bug）— kernel 直接寫它 = oops 或安全漏洞
2. user page 可能不在記憶體（被 swap 出去）— 碰它會觸發 page fault，要走安全路徑

所以 kernel 規定：跨界搬運一律走 `copy_to_user(dst, src, n)`（出去）/ `copy_from_user`（進來）。它會驗證地址範圍、處理 page fault、**失敗不炸而是告訴你**。

**回傳值語意（跟直覺相反，必考）**：回傳「**沒搬成的 byte 數**」— `0` 才是全部成功。

**這就是 Day 35 greybus 四連擊的同一個世界觀**：外來的東西（那邊是 payload，這邊是指標）一律不信，過安檢門才能用。你的供應鏈 feed-sync 也是同一句話：不信宣告、驗實際。

**Java 類比**：`buf` 像一個外部系統給你的 URL — 你不會直接 `new File(url)` 開寫，你會走驗證過的 client。`__user` 標記 = `@Untrusted` annotation（sparse 靜態檢查會抓直接 deref 的人）。

> 小測 3：`copy_to_user` 回傳 5，代表什麼？這時 dev_read 該回傳什麼？（提示：部分成功在這關可以直接當失敗處理，回 `-EFAULT`）

---

## 概念 4：read 契約與 *off — 為什麼 cat 不會永遠讀下去

userspace 的 read() 契約（你在 my_cat 已經當過**呼叫方**，現在換你當**實作方**）：

- 回傳 > 0：給了這麼多 bytes，呼叫者會**再來要**
- 回傳 0：**EOF**，呼叫者停手
- 回傳 < 0：錯誤（-EFAULT、-EINVAL…，**負 errno**，Day 32 exit code 的 kernel 版）

`cat` 就是個 `while (read() > 0)` 迴圈。你若每次都回傳訊息長度，cat 永遠讀不完 — **無限迴圈就是這關埋的雷**。

`loff_t *off` 是 kernel 幫你記的「讀到哪了」（per-open 的游標，存在 `struct file` 裡）。所以 TODO 1 的邏輯：

```
第一次讀：*off == 0 → 給訊息、*off += n、回 n
第二次讀：*off > 0  → 回 0（EOF）→ cat 收工
```

**Java 類比**：`*off` 就是 `InputStream` 內部的 position；read() 回 -1（Java 的 EOF）≈ 這裡回 0。

> 小測 4：如果兩個終端機同時 `cat /dev/lv09`，它們會互相干擾彼此的 *off 嗎？（提示：每次 open 產生一個獨立的 `struct file`）

---

## 概念 5：register_chrdev 生命週期 — 又是對稱

```c
init:  major = register_chrdev(0, "lv09", &fops);   /* 0 = kernel 自選 major */
exit:  unregister_chrdev(major, "lv09");
```

- `register_chrdev(0, ...)` 回傳分到的 major（>0），失敗回負 errno → **init 要檢查，失敗要把錯誤傳回去**（insmod 才會失敗而不是半殘）
- exit 一定要 unregister — 不然 rmmod 後 VFS 表裡還掛著指向**已消失程式碼**的函數指標，下次有人 open 就是 use-after-free（kernel 版的，比 userspace 兇）

**對稱律**（這是第三次見了）：Day 35 greybus probe/disconnect、my_sort 的 malloc/free、這裡的 register/unregister — **取得跟釋放永遠成對、順序相反**。

> 小測 5：為什麼 `register_chrdev` 傳 0 讓 kernel 自選 major，而不是自己挑個喜歡的數字？（提示：誰知道哪些號碼已被占用？）

---

## 回到三個 TODO — 現在再看一次

| TODO | 用到的概念 | 現在應該的感覺 |
|------|-----------|----------------|
| TODO 1（dev_read）| 概念 3 + 4 | 「就是 off 判斷 + copy_to_user + 推進 off」 |
| TODO 2（init 註冊）| 概念 1 + 5 | 「註冊、印 major 給 mknod 用、檢查失敗」 |
| TODO 3（exit 反註冊）| 概念 5 | 「一行，跟 init 對稱」 |

動手順序建議：

1. 讀 LKMPG Character Device Drivers 章（現在讀，已經有地圖了，會快很多）
2. 五題小測寫進筆記（答案不確定的標 `?`，回報時我對）
3. 完成三個 TODO → 按 spec.md 的過關流程驗（insmod → dmesg 找 major → mknod → cat → 清場）
4. TG 回報：五題小測答案 + cat 輸出截圖/貼文字 + 「踩了什麼」

---

## 排程建議（跟 Week E 怎麼共存）

Week E（coreutils）是主線別斷 — 它練的 C 功夫正是 wargame 的彈藥（你看小測裡一直引用 Day 32/33/35）。

- **方案 A（推薦）**：本週末（6/14 或 6/15 的 coreutils 寫完後）加場 1 小時打掉 LV0.9 — briefing 已經把坡度鏟平了
- **方案 B**：Week E 結業（6/18 day-35 讀完 greybus — 它跟本關概念重疊度超高）後隔天打，概念互相增強

LV1 之後需要 QEMU 沙盒 + 同步知識（todo.md 的 Week H 才到），**LV0.9 打完先停**，不用焦慮往前衝 — 升級制的意義就是不欠進度。
