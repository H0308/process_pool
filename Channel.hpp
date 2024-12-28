#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <iostream>
#include <string>
#include <unistd.h>

// 定义管道类
class Channel
{
public:
    Channel(pid_t pid, int wfd)
        : _pid(pid), _wfd(wfd)
    {
        // 设置管道名称为：Channel-编号-写端文件描述符-对应的子进程
        _name = "Channel-" + std::to_string(_count++) + "-wfd" + std::to_string(_wfd) + "-pid=" + std::to_string(_pid);
    }

    std::string getName() const
    {
        return _name;
    }

    void sendTask(int task_num)
    {
        write(_wfd, &task_num, sizeof(int));
    }

    pid_t getPid() const
    {
        return _pid;
    }

    void closeChannel()
    {
        // 父进程关闭管道写端
        close(_wfd);
    }

private:
    std::string _name; // 管道名称
    pid_t _pid;        // 管道对应的子进程
    int _wfd;          // 管道的写端文件描述符
    static int _count; // 管道的编号
};
int Channel::_count = 0;

#endif