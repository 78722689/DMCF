// Author:ggdou@163.com
#ifndef _LOGER_H_
#define _LOGER_H_

#include <iostream>
#include <string>
#include <sstream>

enum LogSeverityLevel
{
    ELogSeverityLevel_Debug = 0,
    ELogSeverityLevel_Info,
    ELogSeverityLevel_Warning,
    ELogSeverityLevel_Error
};

namespace utility
{

template <unsigned C>
class BaseLoger;

template <unsigned C, LogSeverityLevel Severity>
class Flusher
{
public:
    Flusher(BaseLoger<C>* logger);
    ~Flusher();
    
    template<typename T>
    Flusher& operator<<(const T& data);
    
private:
    BaseLoger<C>* logger_;
};

template <unsigned C>
class BaseLoger
{
public:
    BaseLoger()
    {}
    
    explicit BaseLoger(const std::string& prefix):prefix_("["+prefix+"] ")
    {}
    ~BaseLoger()
    {}

    inline Flusher<C, ELogSeverityLevel_Debug> operator << (Flusher<C, ELogSeverityLevel_Debug> (*manipulator)(BaseLoger&))
    {
        return manipulator(*this);
    }
    
    inline Flusher<C, ELogSeverityLevel_Info> operator << (Flusher<C, ELogSeverityLevel_Info> (*manipulator)(BaseLoger&))
    {
        return manipulator(*this);
    }
    
    inline Flusher<C, ELogSeverityLevel_Warning> operator << (Flusher<C, ELogSeverityLevel_Warning> (*manipulator)(BaseLoger&))
    {
        return manipulator(*this);
    }
    
    inline Flusher<C, ELogSeverityLevel_Error> operator << (Flusher<C, ELogSeverityLevel_Error> (*manipulator)(BaseLoger&))
    {
        return manipulator(*this);
    }
    
private:
    template<LogSeverityLevel Severity>
    friend class utility::Flusher;

    std::string stream_;
    const std::string prefix_;
};

#if defined(LOG_DISABLE_ALL_PRINTS)
template<unsigned C, LogSeverityLevel Severity>
Flusher<C, Severity>::Flusher(BaseLoger<C>* logger) : logger_(logger)
{}

template<unsigned C, LogSeverityLevel Severity>
Flusher<C, Severity>::~Flusher()
{}

template<unsigned C, LogSeverityLevel Severity>    
template<typename T>
Flusher<C, Severity>& Flusher<Severity>::operator<<(const T& data)
{   
    return *this;
}
#else
template<unsigned C, LogSeverityLevel Severity>
Flusher<C, Severity>::Flusher(BaseLoger<C>* logger) : logger_(logger)
{}

template<unsigned C, LogSeverityLevel Severity>
Flusher<C, Severity>::~Flusher()
{
    // TODO: add your inteface to output in where the log should be wroten.
    std::cout << "FLAG[" << C << "] "
		<< logger_->prefix_ << logger_->stream_ << std::endl;
    logger_->stream_.clear();
}

template<unsigned C, LogSeverityLevel Severity>    
template<typename T>
Flusher<C, Severity>& Flusher<C, Severity>::operator<<(const T& data)
{   
     
    std::stringstream ss;
    std::string temp;
    //ss.unsetf(std::ios::skipws);
    ss << data;
    std::getline(ss, temp);
    logger_->stream_ += temp;
    return *this;
}
#endif  // LOG_DISABLE_ALL_PRINTS

#if defined(LOG_DISABLE_DEBUG_PRINTS)
template<unsigned C>
class Flusher<C, ELogSeverityLevel_Debug>
{
public:
    Flusher(BaseLoger* logger){}
    ~Flusher(){}
    
    template<typename T>
    inline Flusher& operator<<(const T& data){return *this;}
};
#endif // LOG_DISABLE_DEBUG_PRINTS

}; // end utility

template <unsigned C>
utility::Flusher<C, ELogSeverityLevel_Debug> debug(utility::BaseLoger<C>& log)
{
    return utility::Flusher<C, ELogSeverityLevel_Debug>(&log);
}
template <unsigned C>
utility::Flusher<C, ELogSeverityLevel_Info> info(utility::BaseLoger<C>& log)
{
    return utility::Flusher<C, ELogSeverityLevel_Info>(&log);
}
template <unsigned C>
utility::Flusher<C, ELogSeverityLevel_Warning> warning(utility::BaseLoger<C>& log)
{
    return utility::Flusher<C, ELogSeverityLevel_Warning>(&log);
}
template <unsigned C>
utility::Flusher<C, ELogSeverityLevel_Error> error(utility::BaseLoger<C>& log)
{
    return utility::Flusher<C, ELogSeverityLevel_Error>(&log);
}

#endif // _LOGER_H_