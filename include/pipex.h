#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

// 普通模式命令数
# define CMD_COUNT 2
#define ERROR_EXIT(msg)      \
    do {                     \
        perror(msg);         \
        exit(EXIT_FAILURE);  \
    } while (0)

// 错误处理
void    error_exit(const char *msg);
void    error_cmd_not_found(const char *cmd);

// 执行流程
void    exec_cmd(char *cmd_str, char **envp);
void    run_pipex(char *infile, char *cmd1, char *cmd2, char *outfile, char **envp);
void    free_args(char **args);
char    *find_executable(char *cmd, char **envp);

#endif
