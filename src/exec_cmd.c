#include"pipex.h"
#include"libft.h"
#include <errno.h>

// void exec_cmd(char *cmd_str, char **envp)
// {
//     char    **argv;
//     char    *cmd_path;

//     // 1. 拆分命令字符串
//     argv = ft_split(cmd_str, ' ');
//     if (!argv || !argv[0])
//         ERROR_EXIT("failed to parse command");

//     // 2. 查找可执行文件
//     cmd_path = find_executable(argv[0], envp);
//     if (!cmd_path)
//     {
//         free_args(argv);
//         perror("command not found");
//         exit(127); 
//     }

//     execve(cmd_path, argv, envp);
    
//     free(cmd_path);
//     free_args(argv);
//     perror("execve failed");
//     if (errno == ENOENT)
//         exit(127);         // 文件不存在或路径错误
//     else if (errno == EACCES)
//         exit(126);         // 找到文件但无执行权限
//     else
//         exit(126);         // 其他 execve 错误，也可设为 1 或 126
// }

void exec_cmd(char *cmd_str, char **envp)
{
    char *argv[] = { "sh", "-c", cmd_str, NULL };

    execve("/bin/sh", argv, envp);

    // execve 失败才会到这里
    perror("execve failed");

    // 常见错误返回码
    if (errno == ENOENT)
        exit(127); // command not found
    else if (errno == EACCES)
        exit(126); // permission denied
    else
        exit(1);   // other error
}