#pragma once
#include "lockqueue.h"
#include <string>

enum LogLevel
{
    INFO,   // 普通信息
    ERROR,  // 错误信息
};

// Mprpc框架提供的日志系统
class Logger 
{
public:
    static Logger& GetInstance();

    void SetLogLevel(int level);
    // 写日志
    void Log(const std::string &msg);
private:
    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
private:
    int m_logLevel; // 日志级别
    LockQueue<std::string> m_lckQue; // 异步写日志的日志队列
};

// 定义宏
#define LOG_INFO(logmsgformat, ...) \
    do \
    { \
        Logger& logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(buf); \
    } while (0)

#define LOG_ERR(logmsgformat, ...) \
    do \
    { \
        Logger& logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(buf); \
    } while (0)