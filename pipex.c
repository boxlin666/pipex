/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:46:26 by helin             #+#    #+#             */
/*   Updated: 2025/05/04 16:03:00 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex.h"

int pipex(int argc, char **argv)
{
    int (*pipes)[2];       // 声明一个指向二维数组的指针
    int NUM_PROCESSES = argc - 3;
    pid_t pid;
    // 动态分配内存：分配 (NUM_PROCESSES - 1) 个 [2] 的数组
    pipes = malloc((NUM_PROCESSES - 1) * sizeof(int[2]));
    // 创建管道
    for (int i = 0; i < NUM_PROCESSES - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe failed");
            exit(1);
        }
    }

    // 创建子进程链
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {  // 子进程逻辑
            // 关闭不相关的管道端
            for (int j = 0; j < NUM_PROCESSES - 1; j++) {
                if (j != i - 1) close(pipes[j][0]);  // 关闭不需要的读端
                if (j != i) close(pipes[j][1]);     // 关闭不需要的写端
            }

            // 接收上一个进程的数据（如果不是第一个子进程）
            if (i > 0) {
                char buffer[128];
                read(pipes[i - 1][0], buffer, sizeof(buffer));
                close(pipes[i - 1][0]);  // 关闭当前管道的读端

                // 对数据进行处理（这里简单地追加进程编号）
                printf("Process %d received: %s\n", i, buffer);
                char new_buffer[128];
                snprintf(new_buffer, sizeof(new_buffer), "%s -> Process %d", buffer, i);

                // 将数据传递给下一个进程（如果不是最后一个子进程）
                if (i < NUM_PROCESSES - 1) {
                    write(pipes[i][1], new_buffer, strlen(new_buffer) + 1);
                    close(pipes[i][1]);  // 关闭当前管道的写端
                }
            }

            exit(0);  // 子进程退出
        }
    }

    // 父进程逻辑
    // 关闭不相关的管道端
    for (int i = 1; i < NUM_PROCESSES - 1; i++) {
        close(pipes[i][0]);  // 父进程不需要读后续管道
        close(pipes[i][1]);  // 父进程不需要写后续管道
    }

    // 父进程写入数据到第一个管道
    char msg[] = "Hello from parent";
    write(pipes[0][1], msg, strlen(msg) + 1);
    close(pipes[0][1]);  

    // 父进程等待所有子进程完成
    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }
    return 0;
}
