# Problem #24 — ops struct + dispatcher

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 37 | **Set 3**

## 🎯 題目

實作 kernel 風格的「**ops struct**」——用 function pointer table 做 polymorphism（C 模擬 OOP virtual methods）：

```c
struct device_ops {
    int  (*init)(struct device *d);
    int  (*read)(struct device *d, char *buf, size_t n);
    int  (*write)(struct device *d, const char *buf, size_t n);
    void (*close)(struct device *d);
};

struct device {
    const struct device_ops *ops;
    void *private_data;
};

int device_init(struct device *d, const struct device_ops *ops);
int device_read(struct device *d, char *buf, size_t n);   /* 呼叫 d->ops->read */
int device_write(struct device *d, const char *buf, size_t n);
void device_close(struct device *d);
```

## 🛠️ 用法（注意：兩種「device」實作共用同一個介面）

```c
extern const struct device_ops null_ops;     /* /dev/null 行為 */
extern const struct device_ops echo_ops;     /* 回 echo input 的 mock */

struct device d1, d2;
device_init(&d1, &null_ops);
device_init(&d2, &echo_ops);

device_write(&d1, "hello", 5);   /* /dev/null：丟掉 */
device_write(&d2, "hello", 5);   /* echo：存起來 */
char buf[16];
device_read(&d2, buf, 5);         /* 拿到 "hello" */
```

## 🐧 對應

Linux 整個 driver 模型：
- `struct file_operations` (`include/linux/fs.h`)
- `struct net_proto_family`
- `struct super_operations`

每個 driver 提供自己的 ops 實例，VFS 層通過 ops 呼叫 driver。**這是 C 做 OOP 的標準範式**。

⚠️ 注意：每個 op handler 第一個參數通常是 `struct device *`（self pointer）——對應 C++/Java 的 `this`。
