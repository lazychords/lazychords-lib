#ifndef LOG_HPP_INCLUDED
#define LOG_HPP_INCLUDED

#define assert(X) Log::reportError(X, __FILE__, __LINE__); assert(X);


class LogImpl;

class Log
{
    static std::unique_ptr<LogImpl> l;
public :
    Log() = delete;
    static void reportError(const std::string& errorMessage, std::string& file, unsigned line)
    {
        f(file + ":" + toString(line) + ":0:"+errorMessage);
    }
};


#endif // LOG_HPP_INCLUDED
