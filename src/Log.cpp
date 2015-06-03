
#include "Log.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include "Utilities.hpp"

class LogImpl
{
    friend class Log;
private :
    std::function<void(const std::string& outMessage)> f;
    LogImpl(const std::function<void(const std::string& outMessage)>& func = std::function<void(const std::string& outMessage)>([](const std::string& error){std::cerr<<error<<"\n"; std::cerr.flush();})) {f = func;}
    void addError(const std::string& errorMessage, const std::string& file, unsigned line) const
    {
        f(file + ":" + toString(line) + ":0:"+errorMessage);
    }
};

void Log::reportError(const std::string& errorMessage, const std::string& file, unsigned line)
{
    l->addError(errorMessage, file, line);
}
std::unique_ptr<LogImpl> Log::l(new LogImpl);
