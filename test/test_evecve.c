#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // 检查是否有额外的命令行参数
    if (argc < 2) {
        printf("Usage: %s [ls options]\n", argv[0]);
        printf("Example: %s -l -a\n", argv[0]);
        return 1;
    }

    // 构造 execve 的参数数组
    char *ls_argv[argc + 1];  // +1 用于 NULL 结束符
    ls_argv[0] = "/bin/ls";   // 第一个参数是程序路径
    for (int i = 1; i < argc; i++) {
        ls_argv[i] = argv[i]; // 把命令行参数传递给 ls
    }
    ls_argv[argc] = NULL;     // 参数数组以 NULL 结束

    // 构造环境变量（这里使用默认环境变量）
    char *envp[] = {NULL};    // 环境变量为空

    // 调用 execve 执行 ls
    if (execve("/bin/ls", ls_argv, envp) == -1) {
        perror("execve failed");
        return 1;
    }

    return 0; // 永远不会执行到这里（如果 execve 成功）
}