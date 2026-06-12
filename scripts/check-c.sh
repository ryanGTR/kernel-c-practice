#!/usr/bin/env bash
#
# check-c.sh — 深度檢查單一 C 檔案
#
# 會跑 5 層檢查，每一層都把原始 stdout/stderr 印出來讓你「親眼看」，
# 最後給一個 pass/fail 的 scorecard。
#
# 用法：
#   ./scripts/check-c.sh <file.c>                # 檢查單一檔案
#   ./scripts/check-c.sh <dir>                   # 檢查目錄下所有 *.c
#
# 範例：
#   ./scripts/check-c.sh c-basics/day-18/basics.c
#   ./scripts/check-c.sh c-basics/day-18
#

set -u  # 不 set -e，我們要讓每一層都跑完再下結論

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CHECKPATCH="$SCRIPT_DIR/checkpatch.pl"

# ANSI colors
RED=$'\e[31m'
GRN=$'\e[32m'
YLW=$'\e[33m'
CYN=$'\e[36m'
MGN=$'\e[35m'
BLD=$'\e[1m'
RST=$'\e[0m'

usage() {
	cat <<EOF
用法: $0 <file.c | directory>

檢查層級:
  1. 基本編譯   — gcc -Wall -Wextra -Werror
  2. 嚴格編譯   — 極致 warning flag (-Wconversion/-Wshadow/-Wpedantic 等)
  3. Sanitizer  — -fsanitize=address,undefined 編譯 + 執行
  4. Valgrind   — memcheck (leak + invalid access)
  5. checkpatch — kernel coding style
EOF
}

if [ $# -ne 1 ]; then
	usage
	exit 2
fi

divider() { printf '%s\n' "────────────────────────────────────────────────────────────"; }

header() {
	echo
	echo "${BLD}${CYN}▶ $1${RST}"
	divider
}

section() {
	echo
	echo "${BLD}${MGN}═══════════════════════════════════════════════${RST}"
	echo "${BLD}${MGN}  檢查檔案: $1${RST}"
	echo "${BLD}${MGN}═══════════════════════════════════════════════${RST}"
}

result() {
	if [ "$1" -eq 0 ]; then
		echo "${GRN}✅ $2${RST}"
	else
		echo "${RED}❌ $2（exit=$1）${RST}"
	fi
}

check_one() {
	local file="$1"
	local base bin pass=0 fail=0
	base="$(basename "$file" .c)"
	bin="$(mktemp -t "chk-${base}.XXXXXX")"
	trap 'rm -f "$bin" "$bin".asan' RETURN

	section "$file"

	# ── 1. 基本編譯 ────────────────────────────────────────
	header "1/5 基本編譯  gcc -Wall -Wextra -Werror -std=c11"
	gcc -Wall -Wextra -Werror -std=c11 "$file" -o "$bin"
	local r1=$?
	result "$r1" "基本編譯"
	[ "$r1" -eq 0 ] && pass=$((pass+1)) || fail=$((fail+1))

	# ── 2. 嚴格編譯 ────────────────────────────────────────
	header "2/5 嚴格編譯  +Wpedantic +Wconversion +Wshadow +Wformat=2 ..."
	gcc -Wall -Wextra -Werror -Wpedantic -Wconversion \
	    -Wshadow -Wcast-align -Wcast-qual \
	    -Wstrict-prototypes -Wmissing-prototypes \
	    -Wdouble-promotion -Wnull-dereference \
	    -Wformat=2 -Wwrite-strings \
	    -fstack-protector-strong \
	    -std=c11 "$file" -o "$bin"
	local r2=$?
	result "$r2" "嚴格編譯（這層才會抓到 %p 缺 void* 這類問題）"
	[ "$r2" -eq 0 ] && pass=$((pass+1)) || fail=$((fail+1))

	# ── 3. Sanitizer ──────────────────────────────────────
	header "3/5 AddressSanitizer + UBSan（編譯並執行）"
	if gcc -Wall -Wextra -g -O1 \
	       -fsanitize=address,undefined \
	       -fno-omit-frame-pointer \
	       -std=c11 "$file" -o "$bin".asan; then
		echo "${YLW}-- 執行 $bin.asan --${RST}"
		"$bin".asan
		local r3=$?
		result "$r3" "Sanitizer 執行（看到 ==ERROR== 代表抓到 bug）"
	else
		echo "${RED}Sanitizer build 失敗${RST}"
		local r3=1
	fi
	[ "$r3" -eq 0 ] && pass=$((pass+1)) || fail=$((fail+1))

	# ── 4. Valgrind ───────────────────────────────────────
	header "4/5 Valgrind memcheck"
	if [ "$r1" -ne 0 ]; then
		echo "${YLW}⚠ 跳過（基本編譯失敗，沒 binary 可跑）${RST}"
		local r4=1
	elif ! command -v valgrind >/dev/null 2>&1; then
		echo "${YLW}⚠ 跳過（valgrind 未安裝，sudo apt install valgrind）${RST}"
		local r4=0
	else
		valgrind --error-exitcode=1 --leak-check=full \
		         --show-leak-kinds=all --track-origins=yes \
		         "$bin"
		local r4=$?
		result "$r4" "Valgrind 零 error 零 leak"
	fi
	[ "$r4" -eq 0 ] && pass=$((pass+1)) || fail=$((fail+1))

	# ── 5. checkpatch ─────────────────────────────────────
	header "5/5 scripts/checkpatch.pl（kernel coding style）"
	if [ ! -x "$CHECKPATCH" ]; then
		echo "${YLW}⚠ checkpatch.pl 不可執行（chmod +x $CHECKPATCH）${RST}"
		local r5=0
	else
		"$CHECKPATCH" --no-tree -f "$file"
		local r5=$?
		result "$r5" "Kernel coding style"
	fi
	[ "$r5" -eq 0 ] && pass=$((pass+1)) || fail=$((fail+1))

	# ── Scorecard ─────────────────────────────────────────
	echo
	echo "${BLD}${CYN}┌─ $file 總評 ─┐${RST}"
	printf "  通過: ${GRN}%d${RST}/5   失敗: ${RED}%d${RST}/5\n" "$pass" "$fail"
	if [ "$fail" -eq 0 ]; then
		echo "  ${GRN}${BLD}🎉 全部過關${RST}"
		return 0
	else
		echo "  ${YLW}往上翻看詳細，對照 wiki/guides/c-mistake-pitfall.md 修${RST}"
		return 1
	fi
}

# 主流程 ─────────────────────────────────────────────────
TARGET="$1"
TOTAL_FAIL=0

if [ -f "$TARGET" ]; then
	check_one "$TARGET" || TOTAL_FAIL=$((TOTAL_FAIL+1))
elif [ -d "$TARGET" ]; then
	shopt -s nullglob
	files=("$TARGET"/*.c)
	if [ ${#files[@]} -eq 0 ]; then
		echo "${RED}找不到 .c 檔案於: $TARGET${RST}"
		exit 2
	fi
	for f in "${files[@]}"; do
		check_one "$f" || TOTAL_FAIL=$((TOTAL_FAIL+1))
	done
	echo
	echo "${BLD}${CYN}══════════ 批次總結 ══════════${RST}"
	echo "  共檢查 ${#files[@]} 個檔案，失敗 ${RED}${TOTAL_FAIL}${RST} 個"
else
	echo "${RED}找不到: $TARGET${RST}"
	exit 2
fi

[ "$TOTAL_FAIL" -eq 0 ]
