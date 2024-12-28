#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unistd.h>
#include <sys/wait.h>
#include "Task.hpp"
#include "Channel.hpp"
#include "ProcessPool.hpp"

void Debug(const std::vector<Channel> &channels)
{
    // 查看已经创建好的进程以及对应的管道文件描述符
    for (auto &chn : channels)
        std::cout << chn.getName() << std::endl;
}

int main(int argc, char *argv[])
{
    // 如果命令行参数个数不为两个，说明使用方法不对
    if (argc != 2)
    {
        std::cout << "正确使用方法：" << argv[0] << "子进程个数" << std::endl;
        return usageError;
    }

    // 根据指定个数创建子进程
    int process_num = atoi(argv[1]);

    // 创建进程池对象
    ProcessPool pp(process_num, main_work);

    // 1. 创建进程池
    int ret = pp.initProcessPool();
    if (ret)
        std::cerr << ret << std::endl;

    // Debug(channels);

    // sleep(20);

    // 2. 父进程分派任务
    pp.dispatchTask();

    // 3. 父进程关闭进程池
    pp.closeProcessPool();

    return 0;
}