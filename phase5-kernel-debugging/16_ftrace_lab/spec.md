# 16_ftrace_lab -- 用 ftrace / trace-cmd 看 kernel 在幹嘛

## 目標

這個 lab **不寫 module**，純粹練 **ftrace** 的操作：直接操作 tracefs（`/sys/kernel/tracing`）
開 `function_graph` tracer、用 filter 鎖定函式、讀 `trace`；再用 **trace-cmd** 跑一輪
record / report。重點是學會「打開 kernel 的引擎蓋看它執行流」。

Java 類比：function_graph 就像把整個 call tree 攤出來的 method profiler / call tracer
（類似 async-profiler 或 `-XX:+TraceClassLoading` 那種觀測能力），只是觀測對象是 kernel 函式，
而且開關都是寫進 tracefs 的檔案。

## ⚠️ 安全

ftrace 本身是唯讀觀測、相對安全，主機上做也行；但既然你已有沙箱，**建議在 QEMU/VM 練手**，
養成在隔離環境玩 kernel 機制的習慣（`wargame/qemu-sandbox/`）。

## 需要的 CONFIG / 前置

- Kernel 需 **`CONFIG_FUNCTION_TRACER=y`**、**`CONFIG_FUNCTION_GRAPH_TRACER=y`**
  （多數發行版預設有開）。
- tracefs 通常掛在 `/sys/kernel/tracing`（舊路徑 `/sys/kernel/debug/tracing`）。
- 多數操作要 root。
- `trace-cmd`：`sudo pacman -S trace-cmd`（Arch）。

## 步驟

### A. 手動操作 tracefs

```
cd /sys/kernel/tracing

# 先看有哪些 tracer 可選
cat available_tracers

# 啟用 function_graph（會印出 call tree + 每個函式耗時）
echo function_graph > current_tracer

# 縮小範圍：只追跟某主題相關的函式（例：開檔相關）
echo 'do_sys_openat*' > set_ftrace_filter
cat set_ftrace_filter        # 確認 filter 生效

# 開始抓 -> 做點事 -> 停
echo 1 > tracing_on
ls /etc > /dev/null
echo 0 > tracing_on

# 看結果
cat trace | head -n 40

# 收尾：清掉 tracer 與 filter，避免一直在背景 trace
echo nop > current_tracer
echo > set_ftrace_filter
```

觀察重點：function_graph 的縮排 = call depth；右邊的時間 = 該函式耗時；
找到你 filter 的函式被誰呼叫、又呼叫了誰。

### B. 用 trace-cmd 跑一輪

```
# 錄一段 function_graph，鎖定同樣的 filter，期間跑一個指令
sudo trace-cmd record -p function_graph -g 'do_sys_openat*' ls /etc

# 解讀剛剛錄到的 trace.dat
trace-cmd report | head -n 40
```

比較 A（手動 echo）和 B（trace-cmd）兩種做法：trace-cmd 幫你包好 enable/run/disable/抓檔，
手動 echo 則讓你看清楚底層每個開關是哪個 tracefs 檔案。

## 完成 checklist

- [ ] `echo function_graph > current_tracer` 後 `cat trace` 看得到 call tree
- [ ] `set_ftrace_filter` 成功把追蹤範圍縮到指定函式
- [ ] 能指出 trace 輸出裡：縮排=call depth、右側=函式耗時
- [ ] 收尾把 `current_tracer` 設回 `nop`、清空 filter
- [ ] `trace-cmd record` + `trace-cmd report` 成功跑完一輪
- [ ] 能說出手動 tracefs 與 trace-cmd 各自的便利點

## 對應 wiki

- [[ftrace]] — `linux_kernel/wiki/ftrace.md`
