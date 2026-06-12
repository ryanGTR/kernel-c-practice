# Kernel-LeetCode：30 題 C 練習集

> 給 Ryan 的「kernel-flavored LeetCode」——每題 15-45 分鐘，從 K&R Ch 5 程度走到 kernel idiom 預習。
> 對應 todo.md 的 Day 19 → Day 49 整段，每天 1 題剛好填完。

---

## 🎯 30 題索引 + Day Mapping

### Set 1 — Pointer & Array 基本功（Day 19 → Day 28）

| # | 題目 | 難度 | 對應 Day | 重點 API |
|---|---|---|---|---|
| 01 | [my_strlen](set1-pointer-array/01_my_strlen/PROBLEM.md) | Easy | Day 20 | pointer 走訪 |
| 02 | [my_strcpy](set1-pointer-array/02_my_strcpy/PROBLEM.md) | Easy | Day 20 | NULL 結尾 |
| 03 | [my_strscpy](set1-pointer-array/03_my_strscpy/PROBLEM.md) | Medium | Day 20 | kernel 安全字串複製 |
| 04 | [my_memcpy](set1-pointer-array/04_my_memcpy/PROBLEM.md) | Easy | GDB Day 24 | byte-wise 複製 |
| 05 | [my_memmove](set1-pointer-array/05_my_memmove/PROBLEM.md) | Medium | GDB Day 24 | 重疊區域處理 |
| 06 | [my_strdup](set1-pointer-array/06_my_strdup/PROBLEM.md) | Easy | Day 29 | malloc + strcpy |
| 07 | [my_atoi](set1-pointer-array/07_my_atoi/PROBLEM.md) | Easy | Day 23 (my_wc) | 字串轉數字 |
| 08 | [reverse_string](set1-pointer-array/08_reverse_string/PROBLEM.md) | Easy | GDB Day 23 | in-place 操作 |
| 09 | [count_words](set1-pointer-array/09_count_words/PROBLEM.md) | Easy | Day 23 (my_wc) | state machine |
| 10 | [find_substring](set1-pointer-array/10_find_substring/PROBLEM.md) | Medium | Day 25 (my_grep) | 模式匹配 |

### Set 2 — 記憶體 ownership（Day 29 → Day 42）

| # | 題目 | 難度 | 對應 Day | 重點 |
|---|---|---|---|---|
| 11 | [dynamic_int_array](set2-memory-ownership/11_dynamic_int_array/PROBLEM.md) | Medium | Day 26 (my_sort), Day 31 | push/pop/realloc |
| 12 | **[strbuf](set2-memory-ownership/12_strbuf/PROBLEM.md)** ⭐ | Medium | **Day 19 主菜** | realloc + ownership |
| 13 | [ring_buffer](set2-memory-ownership/13_ring_buffer/PROBLEM.md) | Medium | Day 31 | wrap-around index |
| 14 | [stack](set2-memory-ownership/14_stack/PROBLEM.md) | Easy | Day 31 | LIFO |
| 15 | [queue](set2-memory-ownership/15_queue/PROBLEM.md) | Easy | Day 31 | FIFO |
| 16 | [doubly_linked_list](set2-memory-ownership/16_doubly_linked_list/PROBLEM.md) | Medium | Day 21, Day 29 | prev/next 維護 |
| 17 | [refcounted_object](set2-memory-ownership/17_refcounted_object/PROBLEM.md) | Medium | Day 36 | get/put pattern |
| 18 | [memory_pool](set2-memory-ownership/18_memory_pool/PROBLEM.md) | Hard | Day 32 | 預先 alloc 池 |
| 19 | [arena_allocator](set2-memory-ownership/19_arena_allocator/PROBLEM.md) | Medium | Day 32 | bump + reset |
| 20 | [bump_allocator](set2-memory-ownership/20_bump_allocator/PROBLEM.md) | Easy | Day 32 | 最簡 alloc |

### Set 3 — Kernel idiom 預習（Day 43 → Day 49）

| # | 題目 | 難度 | 對應 Day | 重點 |
|---|---|---|---|---|
| 21 | [container_of](set3-kernel-idiom/21_container_of/PROBLEM.md) | Medium | Day 36 | embedded struct 反推 |
| 22 | [err_ptr_pattern](set3-kernel-idiom/22_err_ptr_pattern/PROBLEM.md) | Medium | Day 38 | pointer 編碼 errno |
| 23 | [goto_cleanup](set3-kernel-idiom/23_goto_cleanup/PROBLEM.md) | Medium | Day 40 | 階梯式錯誤處理 |
| 24 | [ops_struct](set3-kernel-idiom/24_ops_struct/PROBLEM.md) | Medium | Day 37 | function pointer table |
| 25 | [list_for_each_safe](set3-kernel-idiom/25_list_for_each_safe/PROBLEM.md) | Medium | Day 39 | 走訪時刪除 |
| 26 | [min_max_macro](set3-kernel-idiom/26_min_max_macro/PROBLEM.md) | Medium | Day 36 | type-safe macro |
| 27 | [array_size_macro](set3-kernel-idiom/27_array_size_macro/PROBLEM.md) | Medium | Day 36 | 編譯期長度 |
| 28 | [build_bug_on](set3-kernel-idiom/28_build_bug_on/PROBLEM.md) | Hard | Day 36 | 編譯期斷言 |
| 29 | [kfifo](set3-kernel-idiom/29_kfifo/PROBLEM.md) | Hard | Day 49 | 無鎖 ring buffer |
| 30 | [atomic_counter](set3-kernel-idiom/30_atomic_counter/PROBLEM.md) | Medium | Day 49 | C11 _Atomic |

---

## 📱 用法

### 1. 手機看題目
所有 `PROBLEM.md` 在 GitHub mobile 上可讀，或下載到手機 markdown app。
**通勤時間 = 讀題 + 心算**。

### 2. 電腦寫程式
```bash
cd ~/Documents/kernel-c-practice/leetcode/set1-pointer-array/01_my_strlen/
make           # 編譯 starter.c
./01_my_strlen # 跑（會 fail，因為 starter 是骨架）

# 寫完後
make check     # valgrind + ASan 自動驗
make solution  # 看參考解
```

### 3. 通關標準（每題）
- [ ] valgrind 0 errors 0 leaks
- [ ] ASan 無錯誤
- [ ] 輸出符合 PROBLEM.md 預期
- [ ] **白紙能自己再寫一次**（戒律 #2）

---

## 🛠️ 共用工具

- **Makefile.common**：每題 Makefile include 此檔，避免重複
- **wiki/tools/c-api-cheatsheet.md**：手機 API 速查
- **wiki/tools/valgrind.md**：valgrind 教學（含 §3.5 實戰拆解）

---

## 📚 對應教材

每題 PROBLEM.md 都標：
- 對應 wiki 章節
- 對應 K&R 章節
- 對應 kernel source 檔案（之後會 navigate 到）

---

## 🎯 建議刷法

```
週一/三/五 → Phase 1/2 主項目（todo.md 排的 my_wc 等）+ 1 題搭配
週二/四    → 純刷題，挑 1-2 題
週六       → 自由：補刷 / 進階題 / 讀別人的 code
週日       → 反思 + 寫進度筆記

每天電腦時間：30-45 分鐘
每天手機時間：讀明天的題目（10-15 分鐘）+ cheatsheet 隨時查
```

---

## ✅ 完成記錄

完成一題在這勾起來：

- [ ] 01 my_strlen
- [ ] 02 my_strcpy
- [ ] 03 my_strscpy
- [ ] 04 my_memcpy
- [ ] 05 my_memmove
- [ ] 06 my_strdup
- [ ] 07 my_atoi
- [ ] 08 reverse_string
- [ ] 09 count_words
- [ ] 10 find_substring
- [ ] 11 dynamic_int_array
- [ ] 12 **strbuf** ⭐
- [ ] 13 ring_buffer
- [ ] 14 stack
- [ ] 15 queue
- [ ] 16 doubly_linked_list
- [ ] 17 refcounted_object
- [ ] 18 memory_pool
- [ ] 19 arena_allocator
- [ ] 20 bump_allocator
- [ ] 21 container_of
- [ ] 22 err_ptr_pattern
- [ ] 23 goto_cleanup
- [ ] 24 ops_struct
- [ ] 25 list_for_each_safe
- [ ] 26 min_max_macro
- [ ] 27 array_size_macro
- [ ] 28 build_bug_on
- [ ] 29 kfifo
- [ ] 30 atomic_counter

通關 = 30/30 + 整套 wiki 整合到 todo.md。
