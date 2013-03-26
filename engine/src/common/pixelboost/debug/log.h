#pragma once

#include <mutex>
#include <string>
#include <vector>

namespace pb
{
    class File;
    
    enum LogLevel
    {
        kLogLevelDebug,
        kLogLevelInfo,
        kLogLevelWarn,
        kLogLevelError,
        kLogLevelNone,
    };
    
    class LogSubscriber
    {
    public:
        LogSubscriber();
        virtual ~LogSubscriber();
        
        void SetFilter(LogLevel logLevel = kLogLevelDebug, const std::string& systemFilter = "");
        
        LogLevel GetLogLevel();
        std::string GetSystemFilter();
        
        virtual void OnLogMessage(LogLevel logLevel, const std::string& system, const std::string& message) = 0;
        
    private:
        LogLevel _Level;
        std::string _Filter;
    };
    
    class LogSubscriberConsole : public LogSubscriber
    {
    public:
        virtual void OnLogMessage(LogLevel logLevel, const std::string& system, const std::string& message);
    };
    
    class LogSubscriberFile : public LogSubscriber
    {
    public:
        LogSubscriberFile(const std::string& filename);
        ~LogSubscriberFile();
        
        virtual void OnLogMessage(LogLevel logLevel, const std::string& system, const std::string& message);
        
    private:
        std::shared_ptr<File> _File;
    };
    
    class LogSystem
    {
    public:
        LogSystem();
        ~LogSystem();
        
        static LogSystem* Instance();
        
    public:
        void AddSubscriber(LogSubscriber* subscriber);
        void Log(LogLevel logLevel, const std::string& system, const std::string& message);
        
    private:
        std::vector<LogSubscriber*> _Subscribers;
        std::mutex _Mutex;
    };
}

#if defined(PIXELBOOST_BUILD_MASTER)
	#define PbLogDebug(system, format, ...) do {} while(0)
    #define PbLogError(system, format, ...) do {} while(0)
    #define PbLogInfo(system, format, ...) do {} while(0)
    #define PbLogWarn(system, format, ...) do {} while(0)
#else
    #define PbLogDebug(system, format, ...) do { char message[1024]; snprintf(message, 1024, format, ## __VA_ARGS__); pb::LogSystem::Instance()->Log(pb::kLogLevelDebug, system, message); } while(false)
	#define PbLogError(system, format, ...) do { char message[1024]; snprintf(message, 1024, format, ## __VA_ARGS__); pb::LogSystem::Instance()->Log(pb::kLogLevelError, system, message); } while(false)
	#define PbLogInfo(system, format, ...) do { char message[1024]; snprintf(message, 1024, format, ## __VA_ARGS__); pb::LogSystem::Instance()->Log(pb::kLogLevelInfo, system, message); } while(false)
    #define PbLogWarn(system, format, ...) do { char message[1024]; snprintf(message, 1024, format, ## __VA_ARGS__); pb::LogSystem::Instance()->Log(pb::kLogLevelWarn, system, message); } while(false)
#endif
