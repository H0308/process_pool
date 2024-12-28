#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <iostream>
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using sub_task = std::function<void()>;

// 子任务
// 下载
void download()
{
    std::cout << "下载任务-" << "执行的进程为：" << getpid() << std::endl;
}
// 上传
void upload()
{
    std::cout << "上传任务-" << "执行的进程为：" << getpid() << std::endl;
}
// 修改
void modify()
{
    std::cout << "移除任务-" << "执行的进程为：" << getpid() << std::endl;
}

class TaskManager
{
public:
    TaskManager()
    {
        // 随机数种子
        srand(time(NULL));
        insertTask(download);
        insertTask(upload);
        insertTask(modify);
    }

    // 添加任务和编号的映射
    void insertTask(sub_task sub)
    {
        task_count[_number++] = sub;
    }

    // 获取任务编号
    int getTaskNumber()
    {
        // 任务编号范围[0, _number]
        return rand() % _number;
    }

    // 执行任务
    void executeTask(int number)
    {
        // 先判断任务是否存在于哈希表中
        if (!task_count.count(number))
        {
            std::cout << "不存在编号为：" << number << "的任务";
            return;
        }

        task_count[number]();
    }

private:
    static int _number;
    std::unordered_map<int, sub_task> task_count;
};
int TaskManager::_number = 0;

// 创建TaskManager类对象
TaskManager tmg;

#endif