#ifndef LEMON_LOG_LOG_HPP
#define LEMON_LOG_LOG_HPP

#include <condition_variable>
#include <thread>
#include <string>
#include <cstdarg>
#include <chrono>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <lemon/nocopy.hpp>

namespace lemon{ namespace log{
    // log level enum
    enum class level{
        ERROR = 1, WARN = 2, INFO = 4, DEBUG = 8, TRACE = 16, VERBOSE = 32
    };

    class logger;

    class sink;

    struct message
    {
        level                                     LEVEL;
        std::chrono::system_clock::time_point     TS;
        std::string                               Source;
        std::string                               File;
        int                                       Lines;
        std::string                               Content;
    };

    class factory  :public nocopy
    {
    public:

        factory();

        void setlevel(level val);

        void setsink(sink * val);

        void write(const message & msg);

        logger* get(const std::string & name);

        level levels() const {
            return _level;
        }

        void stop();

    private:

        void loop();

    private:
        bool                                        _exit;
        level                                       _level;
        std::mutex                                  _mutex;
        std::condition_variable                     _cond;
        std::condition_variable                     _condexit;
        std::unordered_map<std::string,logger*>     _sources;
        sink*                                       _sink;
        std::vector<message>                        _queue;
        std::thread                                 _workthread;
    };

    class sink
    {
    public:
        virtual void write(const message & msg) = 0;
    };

    class console : public sink
    {
    public:
        void write(const message & msg) final;
    };


    class logger : public nocopy
    {
    public:
        logger(const std::string & name,factory & val)
            :_name(name),_factory(val)
        {}

        level levels() const
        {
            return _factory.levels();
        }

        void write(level l,const std::string && content,const std::string && file,int lines);

    private:
        std::string     _name;
        factory         &_factory;
    };

    logger* get(const std::string name);

    void stop();

    inline void log(logger *source, level level,const char* file,int lines,const char * message,...)
    {
        char buff[1024] = {0};

        va_list args;

        va_start(args,message);

#ifndef WIN32
        int len = vsprintf(buff,message,args);
#endif
        va_end(args);

        source->write(level,std::string(buff,buff+len),std::string(file),lines);
    }
}}

#define logE(logger,fmt,...) if((logger)->levels() >= lemon::log::level::ERROR) { \
    lemon::log::log((logger),lemon::log::level::ERROR,__FILE__,__LINE__,(fmt),##__VA_ARGS__);\
}

#define logW(logger,fmt,...) if((logger)->levels() >= lemon::log::level::WARN) { \
    lemon::log::log((logger),lemon::log::level::WARN,__FILE__,__LINE__,(fmt),##__VA_ARGS__);\
}

#define logI(logger,fmt,...) if((logger)->levels() >= lemon::log::level::INFO) { \
    lemon::log::log((logger),lemon::log::level::INFO,__FILE__,__LINE__,(fmt),##__VA_ARGS__);\
}

#define logD(logger,fmt,...) if((logger)->levels() >= lemon::log::level::DEBUG) { \
    lemon::log::log((logger),lemon::log::level::DEBUG,__FILE__,__LINE__,(fmt),##__VA_ARGS__);\
}

#define logT(logger,fmt,...) if((logger)->levels() >= lemon::log::level::TRACE) { \
    lemon::log::log((logger),lemon::log::level::TRACE,__FILE__,__LINE__,(fmt),##__VA_ARGS__);\
}

#define logV(logger,fmt,...) if((logger)->levels() >= lemon::log::level::VERBOSE) { \
    lemon::log::log((logger),lemon::log::level::VERBOSE,__FILE__,__LINE__,(fmt),##__VA_ARGS__);\
}



#endif //LEMON_LOG_LOG_HPP