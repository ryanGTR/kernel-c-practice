# Problem #23 — goto cleanup 階梯

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 40 | **Set 3**

## 🎯 題目

寫一個 5 階段資源獲取函式，**任何階段失敗都正確清理**已獲取的資源。用 `goto` 階梯模式（kernel 標準寫法）。

```c
int do_setup(struct ctx *ctx) {
    /* 階段 1: 配 buffer1
       階段 2: 配 buffer2
       階段 3: 開檔
       階段 4: 配 buffer3
       階段 5: register callback

       任何階段失敗 → 反向釋放已配置的資源 → 回 -1
       全部成功 → 回 0
    */
}
```

## 🧪 模擬模式

```c
struct ctx {
    char *buf1;
    char *buf2;
    int   fd;
    char *buf3;
    int   registered;
};

int do_setup(struct ctx *ctx, int fail_at);  /* fail_at = 1..5 模擬第 N 階段失敗 */
void do_teardown(struct ctx *ctx);            /* 對稱清理 */
```

`fail_at = 0` 全部成功；`fail_at = 1` 第 1 階段（buf1 alloc）失敗；以此類推。
任何模擬失敗下，valgrind 都應該乾淨。

## 🛠️ Hints

kernel 標準 goto cleanup pattern：

```c
int do_setup(struct ctx *ctx, int fail_at) {
    ctx->buf1 = malloc(64);
    if (!ctx->buf1 || fail_at == 1) goto err_buf1;

    ctx->buf2 = malloc(64);
    if (!ctx->buf2 || fail_at == 2) goto err_buf2;

    ctx->fd = open_fake();   /* 模擬 fd */
    if (ctx->fd < 0 || fail_at == 3) goto err_fd;

    ctx->buf3 = malloc(64);
    if (!ctx->buf3 || fail_at == 4) goto err_buf3;

    if (fail_at == 5) goto err_register;
    ctx->registered = 1;
    return 0;

err_register:
    free(ctx->buf3);
err_buf3:
    close_fake(ctx->fd);
err_fd:
    free(ctx->buf2);
err_buf2:
    free(ctx->buf1);
err_buf1:
    memset(ctx, 0, sizeof(*ctx));
    return -1;
}
```

→ **每個 label 對應「失敗時要倒回去多遠」**。階梯式由淺入深。

## ⚠️ 為什麼用 goto 而非 if-else 巢狀？

巢狀寫法（noisy）：
```c
if (alloc1) {
    if (alloc2) {
        if (open) {
            ...
        } else { free(buf2); free(buf1); return -1; }
    } else { free(buf1); return -1; }
} else { return -1; }
```

→ 5 層深度沒救。kernel goto 寫法線性、可讀、易擴展。

**Linus 的話**：「goto 在 cleanup 是合理的，禁用 goto 是教條。」

## 🐧 對應

整個 kernel 充滿這個 pattern：
- `kernel/fork.c` `copy_process`
- `drivers/*/probe()` 函式
- `fs/` 各 mount/init 函式

打開 `drivers/staging/greybus/vibrator.c` 的 `gb_vibrator_probe()` 看 → **這就是 Day 41 你會讀的東西**。
