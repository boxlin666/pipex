#include"pipex.h"
#include"libft.h"

void free_args(char **args)
{
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

char *find_executable(char *cmd, char **envp)
{
    char **paths = NULL;
    char  *path_env = NULL;
    char  *fullpath = NULL;
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        else
            return NULL;  // 127: 找不到或无法执行
    }
    for (int i = 0; envp[i]; i++)
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
    if (!path_env)
        return NULL;

    paths = ft_split(path_env, ':');
    for (int i = 0; paths[i]; i++)
    {
        // 拼接目录 + "/" + cmd
        fullpath = ft_strjoin(paths[i], "/");
        fullpath = ft_strjoin(fullpath, cmd);
        if (access(fullpath, X_OK) == 0)
        {
            // 找到可执行文件
            // 释放其他 paths 内存后返回 fullpath
            free_args(paths);
            return fullpath;
        }
        free(fullpath);
    }
    free_args(paths);
    return NULL;
}
