#!/bin/bash

PIPEX_EXEC=./pipex
TMP_DIR=./pipex_test_tmp
mkdir -p $TMP_DIR

# 用于记录通过/失败数量
PASS=0
FAIL=0

# 测试用例定义：每行一个 [infile|cmd1|cmd2|outfile]
TEST_CASES=(
  "input1.txt|cat|wc -l|out1.txt"
  "input2.txt|grep apple|wc -c|out2.txt"
  "input3.txt|tr a-z A-Z|cat|out3.txt"
  "input4.txt|sort|uniq|out4.txt"
  "input5.txt|head -n 2|tail -n 1|out5.txt"
  "input_missing.txt|cat|wc -l|out_err.txt" # 错误测试
  "input1.txt|nosuchcmd|cat|out_fail.txt"  # 无效命令
)

# 初始化测试输入文件
echo -e "apple\nbanana\napple\norange" > input1.txt
echo -e "apple\napple\napple" > input2.txt
echo -e "hello\nworld" > input3.txt
echo -e "z\nb\na\nb\nc\nc" > input4.txt
echo -e "line1\nline2\nline3\nline4" > input5.txt

run_test() {
  local infile=$1
  local cmd1=$2
  local cmd2=$3
  local outfile=$4

  local outfile_pipex="$TMP_DIR/${outfile}_pipex"
  local outfile_shell="$TMP_DIR/${outfile}_shell"

  # 执行 pipex
  $PIPEX_EXEC "$infile" "$cmd1" "$cmd2" "$outfile_pipex"
  EXIT_PIPEX=$?

  # 执行 shell 等价命令
  bash -c "< $infile $cmd1 | $cmd2 > $outfile_shell"
  EXIT_SHELL=$?

  # 比较输出内容
  if diff -q "$outfile_pipex" "$outfile_shell" >/dev/null && [ $EXIT_PIPEX -eq $EXIT_SHELL ]; then
    echo "✅ Test: \"$cmd1 | $cmd2\" PASSED"
    PASS=$((PASS + 1))
  else
    echo "❌ Test: \"$cmd1 | $cmd2\" FAILED"
    echo "  Exit pipex: $EXIT_PIPEX, shell: $EXIT_SHELL"
    echo "  Diff:"
    diff "$outfile_pipex" "$outfile_shell"
    FAIL=$((FAIL + 1))
  fi
}

# 执行所有测试
for test in "${TEST_CASES[@]}"; do
  IFS="|" read -r infile cmd1 cmd2 outfile <<< "$test"
  run_test "$infile" "$cmd1" "$cmd2" "$outfile"
done

echo
echo "🎯 TOTAL: $((PASS + FAIL)) tests run"
echo "✅ PASSED: $PASS"
echo "❌ FAILED: $FAIL"

# 清理临时文件（如需调试可注释掉）
rm -rf $TMP_DIR
