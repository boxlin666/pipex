#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

typedef struct s_pipex_bonus
{
    int     cmd_count;      // 命令数量
    char    **cmds;         // 命令字符串数组，比如 {"ls -l", "grep foo", ...}

    int     **pipes;        // 管道数组，大小是 cmd_count - 1，每个 pipes[i][2] 是一个管道的读写fd
                            // 方便遍历和关闭所有管道

    char    *infile;        // 输入文件路径（如果非 here_doc）
    char    *outfile;       // 输出文件路径

    int     infile_fd;      // 输入文件描述符（或者 here_doc 模拟的输入）
    int     outfile_fd;     // 输出文件描述符

    int     here_doc;       // 1 表示使用 here_doc 模式，0 表示普通模式
    char    *limiter;       // here_doc 限制符字符串（仅 here_doc 模式使用）

    pid_t   *pids;          // 存放 fork 出来的子进程 pid，长度 cmd_count

    char    **envp;         // 环境变量，传给 execve 使用

}   t_pipex_bonus;

void    error_exit(const char *msg);

// 执行流程
void    exec_cmd(char *cmd_str, char **envp);
void    run_pipex(t_pipex_bonus *context, char **envp);
void    free_args(char **args);

#endif