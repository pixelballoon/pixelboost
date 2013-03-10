#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.cpp"

#if defined(PIXELBOOST_PLATFORM_ANDROID)
    #include <android/log.h>
#elif defined(PIXELBOOST_PLATFORM_NACL)
    #include "ppapi/cpp/instance.h"
    #include "ppapi/cpp/var.h"
    #include "pixelboost/framework/engine.h"
#endif

using namespace pb;

LogSubscriber::LogSubscriber()
{
    _Level = kLogLevelDebug;
    _Filter = "";
}

LogSubscriber::~LogSubscriber()
{
    
}

void LogSubscriber::SetFilter(LogLevel logLevel, const std::string& systemFilter)
{
    _Level = logLevel;
    _Filter = systemFilter;
}

LogLevel LogSubscriber::GetLogLevel()
{
    return _Level;
}

std::string LogSubscriber::GetSystemFilter()
{
    return _Filter;
}

void LogSubscriberConsole::OnLogMessage(LogLevel logLevel, const std::string& system, const std::string& message)
{
#if defined(PIXELBOOST_PLATFORM_ANDROID)
    switch (logLevel)
    {
        case kLogLevelDebug:
            __android_log_print(ANDROID_LOG_DEBUG, system.c_str(), "%s", message.c_str());
            break;
        case kLogLevelError:
            __android_log_print(ANDROID_LOG_ERROR, system.c_str(), "%s", message.c_str());
            break;
        case kLogLevelInfo:
            __android_log_print(ANDROID_LOG_INFO, system.c_str(), "%s", message.c_str());
            break;
        case kLogLevelWarn:
            __android_log_print(ANDROID_LOG_WARN, system.c_str(), "%s", message.c_str());
            break;
        case kLogLevelNone:
            break;
    }
#elif defined(PIXELBOOST_PLATFORM_NACL)
    static_cast<pp::Instance*>(pb::Engine::Instance()->GetViewController())->PostMessage(pp::Var(status));
#else
    std::string levelString;
    switch (logLevel) {
        case kLogLevelDebug:
            levelString = "[Debug]";
            break;
        case kLogLevelError:
            levelString = "[Error]";
            break;
        case kLogLevelInfo:
            levelString = " [Info]";
            break;
        case kLogLevelWarn:
            levelString = " [Warn]";
            break;
        case kLogLevelNone:
            break;
    }
    printf("%s (%s) %s\n", levelString.c_str(), system.c_str(), message.c_str());
#endif
}

LogSubscriberFile::LogSubscriberFile(const std::string& filename)
{
    _File = pb::FileSystem::Instance()->OpenFile(filename, kFileModeWrite);
}

LogSubscriberFile::~LogSubscriberFile()
{
    delete _File;
}

void LogSubscriberFile::OnLogMessage(LogLevel logLevel, const std::string& system, const std::string& message)
{
    std::string levelString;
    switch (logLevel) {
        case kLogLevelDebug:
            levelString = "[Debug]";
            break;
        case kLogLevelError:
            levelString = "[Error]";
            break;
        case kLogLevelInfo:
            levelString = " [Info]";
            break;
        case kLogLevelWarn:
            levelString = " [Warn]";
            break;
        case kLogLevelNone:
            break;
    }
    char log[2048];
    snprintf(log, 2048, "%s (%s) %s\n", levelString.c_str(), system.c_str(), message.c_str());
    _File->Write(std::string(log));
}

LogSystem::LogSystem()
{
    
}

LogSystem::~LogSystem()
{
    for (auto it = _Subscribers.begin(); it != _Subscribers.end(); ++it)
    {
        delete *it;
    }
}

LogSystem* LogSystem::Instance()
{
    static LogSystem* instance = new LogSystem();
    return instance;
}

void LogSystem::AddSubscriber(LogSubscriber* subscriber)
{
    _Subscribers.push_back(subscriber);
}

void LogSystem::Log(LogLevel logLevel, const std::string& system, const std::string& message)
{
    _Mutex.lock();
    for (auto it = _Subscribers.begin(); it != _Subscribers.end(); ++it)
    {
        if (logLevel >= (*it)->GetLogLevel())
        {
            (*it)->OnLogMessage(logLevel, system, message);
        }
    }
    _Mutex.unlock();
}
