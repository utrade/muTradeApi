#ifndef MUTRADEAPI_LOGGER_H
#define MUTRADEAPI_LOGGER_H

////////////////////
// system headers //
////////////////////

#include <string>
#include <sstream>
#include "../config.h"

#ifdef __GNUC__
#include <syslog.h>
#else
#define _WINSOCKAPI_
#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif
#include <windows.h>
#endif

/////////////////
// our headers //
/////////////////



namespace mutrade {

  /**
   * Various Logging levels supported
   */
  enum LogLevel {
    INFO,
    WARN,
    ERROR_,
    FATAL
  };

  /**
   * \class Logger
   * \brief Abstract Logger class
   */
  class AbstractLogger
  {
  public:
    virtual void log(LogLevel level,
                     const std::string& message) = 0;
    virtual int getLevel(LogLevel level) = 0;

    void setLogLevel(LogLevel level)
    {
      _maxLevel = level;
    }

    int getLogLevel()
    {
      return _maxLevel;
    }

  private:
    LogLevel _maxLevel;

  };

#ifdef __GNUC__

  // ignore the warning which comes from syslog function
#pragma GCC diagnostic ignored "-Wformat-security"

  /**
   * \class SyslogLogger
   * \brief Logger class for POSIX systems
   *
   * Logs are written to syslog facility LOG_LOCAL0
   */
  class SyslogLogger : public AbstractLogger
  {
  public:
    SyslogLogger()
      : _facility()
    {
      openlog(PROJECT_NAME, LOG_CONS | LOG_PID | LOG_PERROR, LOG_LOCAL0);
    }

    void log(LogLevel level,
             const std::string& message)
    {
      if (level >= getLogLevel())
        syslog(getLevel(level) | _facility, message.c_str());
    }

    int getLevel(LogLevel level)
    {
      switch(level)
      {
      case WARN:
        return LOG_WARNING;
      case INFO:
        return LOG_INFO;
      case ERROR_:
        return LOG_ERR;
      case FATAL:
        return LOG_CRIT;
      default:
        return LOG_INFO;
      }
    }

  private:
    int _facility;
  };
#endif /* if GNUC */

#if defined(_WIN32) || defined(_WIN64)
  /**
   * \class WinLogger
   * \brief Logger class for Windows
   */
  class WinLogger : public AbstractLogger
  {
  public:
    WinLogger()
    {
      _eventHandle = RegisterEventSource(NULL, PROJECT_NAME);
    }

    void log(LogLevel level, const std::string& message)
    {
      if (level >= getLogLevel())
	  {
		LPCSTR ptr(message.c_str());
        ReportEvent(_eventHandle,
                    getLevel(level),
                    0,
                    0,
                    NULL,
                    1,
                    0,
                    &ptr,
                    NULL);
	  }
    }

    int getLevel(LogLevel level)
    {
      switch(level)
      {
      case WARN:
        return EVENTLOG_WARNING_TYPE;
      case INFO:
        return EVENTLOG_INFORMATION_TYPE;
      case ERROR_:
      case FATAL:
      default:
        return EVENTLOG_ERROR_TYPE;
      }
    }

  private:
    HANDLE _eventHandle;
  };
#endif /* if WINDOWS */

  /**
   * \class Logger
   * \brief Singleton Logging class
   *
   * This is the class which should be used in code
   * to use the logging functionality. The parameters
   * to be used in the log function, must have stream
   * operators available.
   */

  class Logger
  {
  public:
    static Logger* getInstance()
    {
      static Logger logger;
      return &logger;
    }

    void setLogLevel(LogLevel level)
    {
      _loggerObj->setLogLevel(level);
    }

    void log(LogLevel level,
             const std::string& message)
    {
      std::stringstream ss;
      ss << getLevelDescription(level)
         << message;
      _loggerObj->log(level, ss.str());
    }

    template<typename T>
    void log(LogLevel level,
             const std::string& message,
             const T& param)
    {
      std::stringstream ss;
      ss << getLevelDescription(level)
         << message
         << param;
      _loggerObj->log(level, ss.str());
    }

    template<typename T1,
             typename T2>
    void log(LogLevel level,
             const std::string& message,
             const T1& param1,
             const T2& param2)
    {
      std::stringstream ss;
      ss << getLevelDescription(level)
         << message
         << param1
         << param2;
      _loggerObj->log(level, ss.str());
    }

    template<typename T1,
             typename T2,
             typename T3>
    void log(LogLevel level,
             const std::string& message,
             const T1& param1,
             const T2& param2,
             const T3& param3)
    {
      std::stringstream ss;
      ss << getLevelDescription(level)
         << message
         << param1
         << param2
         << param3;
      _loggerObj->log(level, ss.str());
    }

    template<typename T1,
             typename T2,
             typename T3,
             typename T4>
    void log(LogLevel level,
             const std::string& message,
             const T1& param1,
             const T2& param2,
             const T3& param3,
             const T4& param4)
    {
      std::stringstream ss;
      ss << getLevelDescription(level)
         << message
         << param1
         << param2
         << param3
         << param4;
      _loggerObj->log(level, ss.str());
    }

    template<typename T1,
             typename T2,
             typename T3,
             typename T4,
             typename T5>
    void log(LogLevel level,
             const std::string& message,
             const T1& param1,
             const T2& param2,
             const T3& param3,
             const T4& param4,
             const T5& param5)
    {
      std::stringstream ss;
      ss << getLevelDescription(level)
         << message
         << param1
         << param2
         << param3
         << param4
         << param5;
      _loggerObj->log(level, ss.str());
    }

  private:
    Logger()
      : _loggerObj(0)
    {
#if defined(_WIN32) || defined(_WIN64)
      _loggerObj = new WinLogger();
#else
      _loggerObj = new SyslogLogger();
#endif
    }

    Logger(const Logger& );

    std::string getLevelDescription(LogLevel level)
    {
      switch(level)
      {
      case ERROR_:
        return "ERROR: ";
      case WARN:
        return "WARN : ";
      case FATAL:
        return "FATAL: ";
      case INFO:
      default:
        return "INFO : ";
      }
    }

    AbstractLogger* _loggerObj;
  };
}

#define LOG_0(level, message)                             \
  mutrade::Logger::getInstance()->log(level, message);

#define LOG_1(level, message, param1)                           \
  mutrade::Logger::getInstance()->log(level, message, param1);

#define LOG_2(level, message, param1, param2)                           \
  mutrade::Logger::getInstance()->log(level, message, param1, param2);

#define LOG_3(level, message, param1, param2, param3)                   \
  mutrade::Logger::getInstance()->log(level, message, param1, param2, param3);

#define LOG_4(level, message, param1, param2, param3, param4)           \
  mutrade::Logger::getInstance()->log(level, message, param1, param2, param3, param4);

#define LOG_5(level, message, param1, param2, param3, param4, param5)   \
  mutrade::Logger::getInstance()->log(level, message, param1, param2, param3, param4, param5);

#endif
