#ifndef __PROCESSPOOL_HPP__
#define __PROCESSPOOL_HPP__

#include <iostream>
#include <vector>
#include <functional>
#include <unistd.h>
#include "sys/wait.h"
#include "Channel.hpp"
#include "Task.hpp"

enum
{
    normal,
    usageError,
    pipeError,
    forkError,
};

using main_task = std::function<void()>;

// 主任务
void main_work()
{
    while (true)
    {
        int task_num = 0;
        ssize_t n = read(0, &task_num, sizeof(int));

        if (n == sizeof(int)) // 读取正常，执行任务
            tmg.executeTask(task_num);
        else if (!n) // 读取到0，说明父进程关闭写端，退出主任务
            break;
    }
}

class ProcessPool
{
public:
    ProcessPool(int pn, main_task mt)
        : process_num(pn), work(mt)
    {
    }

    // 初始化进程池
    int initProcessPool()
    {
        for (int i = 0; i < process_num; i++)
        {
            int pipe_arr[2] = {0};
            // 创建管道
            int ret_p = pipe(pipe_arr);
            // 错误处理
            if (ret_p)
                return pipeError;

            // 创建子进程
            int pid = fork();
            // 错误处理
            if (pid < 0)
                return forkError;

            // 子进程执行任务
            if (pid == 0)
            {
                // 关闭写端
                close(pipe_arr[1]);
                // 关闭当前子进程所有的写端
                for (auto &ch : channels)
                    ch.closeChannel();
                // 重定向
                dup2(pipe_arr[0], 0);
                // 任务代码
                work();

                exit(0);
            }

            // 父进程关闭读端
            close(pipe_arr[0]);

            // 使用已有的值构建Channel对象
            Channel ch(pid, pipe_arr[1]);
            // 组织
            channels.push_back(ch);
        }

        return normal;
    }

    // 分派任务
    void dispatchTask()
    {
        int child = 0;
        int count_task = 10;
        while (count_task--)
        {
            // 获取到任务编号
            int task_num = tmg.getTaskNumber();

            // 父进程轮询发送任务
            Channel &chn = channels[child++];
            child %= int(channels.size());

            chn.sendTask(task_num);

            sleep(1);
        }
    }

    // 销毁进程池——版本1
    // void closeProcessPool()
    // {
    //     for (auto &chn : channels)
    //         chn.closeChannel();

    //     for (auto &chn : channels)
    //     {
    //         pid_t rid = waitpid(chn.getPid(), NULL, 0);
    //         if (rid)
    //             std::cout << "已回收进程：" << rid << std::endl;
    //     }
    // }

    // 销毁进程池——版本2
    void closeProcessPool()
    {
        for (auto &chn : channels)
        {
            chn.closeChannel();
            pid_t rid = waitpid(chn.getPid(), NULL, 0);
            if (rid)
                std::cout << "已回收进程：" << rid << std::endl;
        }
    }

private:
    int process_num;               // 子进程个数
    std::vector<Channel> channels; // 管理管道数组
    main_task work;                // 主任务
};

#endif