#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

#ifdef _DEBUG
    #define LOG_TRACE(...) Logger::GetLogger()->trace(__VA_ARGS__)
    #define LOG_INFO(...)  Logger::GetLogger()->info(__VA_ARGS__)
    #define LOG_WARN(...)  Logger::GetLogger()->warn(__VA_ARGS__)
    #define LOG_ERR(...)   Logger::GetLogger()->error(__VA_ARGS__)
    #define LOG_CRIT(...)  Logger::GetLogger()->critical(__VA_ARGS__)

    #define ASSERT_LOGGER() Logger::GetAssertLogger()

    #define FUNCTION() { \
                    SPDLOG_LOGGER_TRACE(Logger::GetAssertLogger(), ""); \
                }
#else
    #define LOG_TRACE(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERR(...)
    #define LOG_CRIT(...)
    #define FUNCTION()
#endif

class Logger
{
private:
	static std::shared_ptr<spdlog::logger> logger;
	static std::shared_ptr<spdlog::logger> assertLogger;

public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetLogger()
    {
        return logger;
    }
    static std::shared_ptr<spdlog::logger>& GetAssertLogger()
    {
        return assertLogger;
    }
};

#endif // !LOGGER_H
