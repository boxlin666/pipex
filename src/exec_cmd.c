#include"pipex.h"
#include"libft.h"

void exec_cmd(char *cmd_str, char **envp)
{
    char    **argv;
    char    *cmd_path;

    // 1. 拆分命令字符串
    argv = ft_split(cmd_str, ' ');
    if (!argv || !argv[0])
        ERROR_EXIT("failed to parse command");

    // 2. 查找可执行文件
    cmd_path = find_executable(argv[0], envp);
    if (!cmd_path)
    {
        free_args(argv);
        ERROR_EXIT("command not found");
    }

    execve(cmd_path, argv, envp);

    ERROR_EXIT("execve");
    free(cmd_path);
    free_args(argv);
}