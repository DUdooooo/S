#ifndef __myLogger_H__
#define __myLogger_H__
#include<log4cpp/Category.hh>
#include<string>
using std::string;
using namespace::log4cpp;
class myLogger
{
public:
    class autorelease
    {
    public:
        autorelease()
        {
        }
        ~autorelease()
        {
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
public:
    enum Priority
    {
        ERROR=300,
        WARN,
        INFO,
        DEBUG
    };

    static myLogger * getInstance();
    void setPriority(Priority p);

    template<class... Args>
        void error(const char * msg,Args... args)
        {
            _mycat.error(msg,args...);
        }
    template<class... Args>
        void warn(const char * msg,Args... args)
        {
            _mycat.warn(msg,args...);
        }
    template<class... Args>
        void info(const char * msg,Args... args)
        {
            _mycat.info(msg,args...);
        }
    template<class... Args>
        void debug(const char * msg,Args... args)
        {
            _mycat.debug(msg,args...);
        }
    void warn(const char *msg);
    void error(const char *msg);
    void info(const char *msg);
    void debug(const char *msg);
private:
    myLogger();
    ~myLogger();
private:
    static autorelease _ar;
    static myLogger * _pInstance;

    log4cpp::Category & _mycat;
};
#define prefix(msg) string("[").append(__FILE__).append(":").append(__FUNCTION__).append(":").append(std::to_string(__LINE__)).append("] ").append(msg).c_str()

#define LogError(msg,...) myLogger::getInstance()->error(prefix(msg),##__VA_ARGS__)
#define LogWarn(msg,...) myLogger::getInstance()->warn(prefix(msg),##__VA_ARGS__)
#define LogInfo(msg,...) myLogger::getInstance()->info(prefix(msg),##__VA_ARGS__)
#define LogDebug(msg,...) myLogger::getInstance()->Debug(prefix(msg),##__VA_ARGS__)


#endif
