#include "logger.h"
#include <time.h>
#include <iostream>

Logger &Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    // 启动专门的写日志线程
    std::thread writeLogTask([&](){
        while (true)
        {
            // 获取当前的日期，然后去日志信息，写入相应的日志文件当中
            time_t now = time(nullptr);
            tm *localTime = localtime(&now);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", 
                    localTime->tm_year + 1900, 
                    localTime->tm_mon + 1, 
                    localTime->tm_mday);

            FILE *fp = fopen(file_name, "a+");
            if (fp == nullptr)
            {
                std::cout << "logger file : " << file_name << " open failed!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lckQue.Pop();

            char time_buf[64] = {0};
            sprintf(time_buf, "%02d:%02d:%02d [%s] ", 
                    localTime->tm_hour, 
                    localTime->tm_min, 
                    localTime->tm_sec,
                    (m_logLevel == INFO) ? "INFO" : "ERROR"
                    );
            msg.insert(0, std::string(time_buf));
            msg.append("\n");

            fputs(msg.c_str(), fp);
            fclose(fp);
        }
    });
    // 分离线程
    writeLogTask.detach();
}

void Logger::SetLogLevel(int level)
{
    m_logLevel = level;
}
// 写日志
void Logger::Log(const std::string &msg)
{
    m_lckQue.Push(msg);
}