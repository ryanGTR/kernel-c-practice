# Problem #17 — refcounted_object

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 36 | **Set 2**

## 🎯 題目

實作 reference counting：

```c
struct rc_obj {
    int ref;
    char *data;
};

struct rc_obj *rc_new(const char *s);   /* 配置，ref=1 */
void rc_get(struct rc_obj *o);           /* ref++ */
void rc_put(struct rc_obj *o);           /* ref--，0 時 free */
```

多人共享一個 obj，誰用 `rc_get` 加引用，用完 `rc_put` 減；歸 0 時自動釋放。

## 🛠️ Hints

```c
struct rc_obj *rc_new(const char *s) {
    struct rc_obj *o = malloc(sizeof(*o));
    if (!o) return NULL;
    o->data = strdup(s);
    if (!o->data) { free(o); return NULL; }
    o->ref = 1;
    return o;
}

void rc_get(struct rc_obj *o) { o->ref++; }
void rc_put(struct rc_obj *o) {
    if (--o->ref == 0) {
        free(o->data);
        free(o);
    }
}
```

## ⚠️ Edge cases
- [ ] rc_new fail（malloc 失敗）→ 全部清乾淨
- [ ] ref 漏 put → leak
- [ ] ref 多 put（< 0）→ UAF

## 🐧 對應

Linux 隨處可見：
```c
struct kref ref;
kref_init(&ref);
kref_get(&ref);
kref_put(&ref, release_func);
```

`fs/inode.c` 的 `iget`/`iput`、network 的 `dst_hold`/`dst_release` 都是這套。

⚠️ **真實 kernel 用 atomic refcount**（多 thread）。本題是單 thread 簡化版。Set 3 #30 atomic_counter 會處理多 thread 版。
