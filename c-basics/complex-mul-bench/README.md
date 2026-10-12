# complex-mul-bench：複數乘法 4 次 vs 3 次

> 2026-09-26～27。從「C 語言複數怎麼算」一路問到「3 次乘法的高斯技巧真的比較快嗎」，實際量出來的紀錄。

## 目錄

1. [問題](#問題)
2. [結論](#結論)
3. [怎麼跑](#怎麼跑)
4. [量測方法](#量測方法)
5. [踩過的坑](#踩過的坑)
6. [推薦讀物](#推薦讀物)

## 問題

兩個複數相乘 `(a + bi)(c + di) = (ac − bd) + (ad + bc)i`：

| 版本 | 做法 | 乘法 | 加減 |
|---|---|---|---|
| mul4 | 直接展開 | 4 | 2 |
| mul3 | k1 = ac、k2 = bd、k3 = (a+b)(c+d)；實部 = k1 − k2、虛部 = k3 − k1 − k2 | 3 | 5 |
| builtin | C99 `<complex.h>` 的 `z1 * z2` | （編譯器決定） | |

3 次乘法是理論下限（Winograd），Karatsuba 大數乘法用的是同一個想法。

## 結論

| 情境 | mul4 | mul3 | builtin |
|---|---|---|---|
| 延遲（一億次串成一條鏈） | **6.7 cycle／次** | 12.0 cycle／次 | 7.0 cycle／次 |
| 吞吐量（陣列、互不相依） | 0.570 ns／次 | 0.571 ns／次 | — |

- **4 次乘法不會比較慢，而且有時快很多。** 3 次乘法在這台機器（FW16）上沒有任何情境贏。
- mul3 慢在「等待」，不在指令數：它的關鍵路徑是加 → 乘 → 減 → 減，一步等一步，約 4 × 3 cycle。這個拆解是推算，還沒看組合語言驗證。
- builtin 指令最多（16 個，多了檢查 NaN 的分支），但分支可以平行，只慢一點點；`__muldc3` 只在結果是 NaN 時才會被呼叫。

原始數據：`perf_latency.txt`、`trace_43.csv`、`trace_34.csv`、`trace.png`。

## 怎麼跑

```bash
gcc -std=c11 -O2 -Wall -Wextra -Werror cmul_bench.c -o cmul_bench -lm
taskset -c 2 ./cmul_bench                      # 三個版本都跑
perf stat -e cycles:u,instructions:u taskset -c 2 ./cmul_bench 0.9999999 4   # 只跑 mul4（4 / 3 / b）

gcc -std=c11 -O2 -Wall -Wextra -Werror cmul_tput2.c -o cmul_tput2
taskset -c 2 ./cmul_tput2                      # 吞吐量，含暖機＋交錯

gcc -std=c11 -O2 -Wall -Wextra -Werror cmul_trace.c -o cmul_trace
taskset -c 2 ./cmul_trace 43 > trace_43.csv    # 逐輪記錄，看暖機爬升
taskset -c 2 ./cmul_trace 34 > trace_34.csv
~/Documents/llm-from-scratch/.venv/bin/python plot_trace.py trace_43.csv trace_34.csv   # → trace.png
```

## 量測方法

| 做法 | 為什麼 |
|---|---|
| `taskset -c 2` 綁核心 | 避免被排程器搬到別顆核心 |
| `-O2` | 跟實際使用一致 |
| 輸入從 `argv` 來 | 編譯器沒辦法事先算好答案 |
| 結果印出來或累加進 `sink` | 不然編譯器會把沒人用的計算整段刪掉（JMH 的 Blackhole 同理） |
| 三個版本結果互相比對 | 先確定算得一樣，再比快慢 |
| 延遲、吞吐量分開量 | 兩種情境的贏家不一樣 |
| 暖機一輪＋交錯跑多輪 | 見下面踩過的坑 |
| **用 `perf stat` 量 cycle，不只量秒數** | CPU 時脈會變，秒數會騙人 |

## 踩過的坑

1. **順序偏差**：第一版吞吐量測試，兩個版本的順序一對調，結論就反過來（誰排第二誰快）。加上暖機、交錯跑之後才穩定。
2. **時脈會變**：同一支程式，某次 mul4 是 1.33 ns／次，另一次 1.91 ns／次，但 cycle 數一樣（約 6.7）。換算出來一次跑在約 5 GHz（Turbo），一次 3.5 GHz。**用 cycle 比，不要用秒比。**
3. **收回的結論**：我一度報「吞吐量 mul3 快 8%」（0.28 vs 0.30 ns），後來逐輪量測是 0.570 vs 0.571，打平。那 8% 不可靠，已收回。
4. **暖機爬升很短**：`trace.png` 顯示只有最先跑的那個版本前 3 輪慢一倍，之後就平穩。所以第 1 點的順序偏差，光靠這 3 輪解釋不了，比較可能是時脈變化（第 2 點）。

## 推薦讀物

建議順序：① → ③ → ②。

| # | 讀物 | 為什麼 |
|---|---|---|
| ① | CS:APP 第 5 章〈Optimizing Program Performance〉 | CSAPP labs 主線本來就要讀；講延遲 vs 吞吐量、關鍵路徑，可拿本實驗對照 |
| ② | Denis Bakhvalov《Performance Analysis and Tuning on Modern CPUs》（免費，easyperf.net） | perf、硬體計數器、benchmark 陷阱的實戰版 |
| ③ | JMH Samples ＋ Aleksey Shipilëv 的文章 | 用 Java 的角度講同一套坑（暖機、Blackhole） |
| ④ | Chandler Carruth, CppCon 2015〈Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!〉 | 一小時現場示範用 perf 抓瓶頸 |
| ⑤ | Agner Fog 最佳化手冊／uops.info | 查每個指令幾個 cycle，可驗證「每步約 3 cycle」的推算 |
| ⑥ | Brendan Gregg《Systems Performance》 | 系統層級效能方法論，長期用 |

## 練習

- 自己寫一個 `mul3(a, b, c, d)`，跟內建的 `z1 * z2` 比對結果。
- 用 `objdump -d` 看 mul3 的組合語言，驗證「關鍵路徑 4 步」的推算。
- 把 `cmul_tput2.c` 整理成 kernel coding style（它是趕著寫的量測程式）。
