#include "pipex.h"
#include "libft.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

void run_pipex(char *infile, char *cmd1, char *cmd2, char *outfile, char **envp)
{
    int infile_fd, outfile_fd;
    int pipefd[2];
    char *error_message;
    pid_t pid1, pid2;
    int status1, status2;

    // 1. 打开 infile 和 outfile
    infile_fd = open(infile, O_RDONLY);
    outfile_fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (outfile_fd < 0)
        ERROR_EXIT("open outfile");

    // 2. 创建管道
    if (pipe(pipefd) == -1)
        ERROR_EXIT("pipe");

    // 3. 第一个子进程：执行 cmd1，将 infile -> cmd1 -> 管道
    pid1 = fork();
    if (pid1 < 0)
        ERROR_EXIT("fork");
    if (pid1 == 0)
    {
        if (infile_fd < 0)
        {
            error_message = ft_strjoin(cmd1, ": ");
            error_message = ft_strjoin(error_message, infile);
            ERROR_EXIT(error_message);
        }
        // 重定向 stdin/stdout
        dup2(infile_fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(infile_fd);
        close(outfile_fd);
        close(pipefd[0]);
        close(pipefd[1]);
        exec_cmd(cmd1, envp);
    }

    // 4. 第二个子进程：执行 cmd2，将 管道 -> cmd2 -> outfile
    pid2 = fork();
    if (pid2 < 0)
        ERROR_EXIT("fork");
    if (pid2 == 0)
    {
        if (infile_fd < 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            dup2(outfile_fd, STDOUT_FILENO);
            close(outfile_fd);
            close(pipefd[0]);
        }
        else
        {
            dup2(pipefd[0], STDIN_FILENO);
            dup2(outfile_fd, STDOUT_FILENO);
            close(infile_fd);
            close(outfile_fd);
            close(pipefd[0]);
            close(pipefd[1]);
        }

        exec_cmd(cmd2, envp);
    }

    // 5. 父进程关闭所有 fd 并等待子进程结束
    close(infile_fd);
    close(outfile_fd);
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    if (WIFEXITED(status2))
    {
        exit(WEXITSTATUS(status2));
    }
    // 所有命令成功，返回 0
    exit(0);
}
