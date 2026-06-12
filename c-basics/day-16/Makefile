# Makefile 註解用 '#' 開頭，到行尾結束（跟 shell 一樣）
# 變數賦值用 '=' 或 ':='，引用用 $(變數名)
#
# ⚠️ 重要陷阱：變數賦值行若寫 'CC = gcc   # 編譯器'，
#    那些空格和 '# 編譯器' 會被當成變數值的一部分！
#    所以註解最好獨立一行，或緊貼變數值後面不留空格。

# ===== 變數定義 =====

# 編譯器
CC = gcc

# 編譯旗標：打開警告 + debug symbol
CFLAGS = -Wall -Wextra -g

# 所有要連結的 object files
OBJS = main.o math.o

# 最終產出的執行檔名稱
TARGET = calc

# ===== 預設目標（第一個 target 會是 'make' 的預設）=====
# 語法：target: prerequisites
#       <TAB>recipe
# 注意：recipe 前面一定要是「真正的 Tab」，不能是空格
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# ===== 各檔案編譯規則 =====

# main.o 依賴 main.c 和 math.h：任一個改了就重編
main.o: main.c math.h
	$(CC) $(CFLAGS) -c main.c

math.o: math.c math.h
	$(CC) $(CFLAGS) -c math.c

# ===== 清理規則 =====
# 'make clean' 會刪掉所有產出檔
# 用 'rm -f' 而非 'rm -rf'：不小心改錯變數時災難小一點
clean:
	rm -f $(OBJS) $(TARGET)

# ===== Phony targets =====
# 告訴 make：clean 不是檔案名，不要跟實體檔案比對
# （否則如果你真的建了一個叫 clean 的檔案，'make clean' 就壞了）
.PHONY: clean
