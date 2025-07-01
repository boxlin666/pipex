#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

typedef struct s_pipex
{
    int pipefd[2];
    int infile_fd;
    int outfile_fd;
    char *infile;
    char *outfile;
    char *cmd1;
    char *cmd2;
    pid_t pid1;
    pid_t pid2;
} t_pipex;

void    error_exit(const char *msg);

void    exec_cmd(char *cmd_str, char **envp);
void    run_pipex(t_pipex *context, char **envp);
void    free_args(char **args);

#endif
