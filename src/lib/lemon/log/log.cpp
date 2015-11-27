#include <iostream>
#include <lemon/fs/filepath.hpp>
#include <lemon/log/log.hpp>

namespace lemon{ namespace log{

    factory::factory():_level(level::VERBOSE),_exit(false)
    {
        _workthread = std::thread(&factory::loop,this);
    }

    void factory::loop()
    {
        while(!_exit)
        {
            std::unique_lock<std::mutex> lock(_mutex);

            if(_queue.empty() && !_exit)
            {
                _cond.wait(lock);
                continue;
            }

            std::vector<message> messages = std::move(_queue);

            _queue.clear();

            {
                std::unique_lock<std::mutex> unlock(_mutex,std::defer_lock);

                for(auto msg : messages)
                {
                    _sink->write(msg);
                }
            }
        }

        _condexit.notify_one();
    }

    void factory::setlevel(level val)
    {
        _level = val;
    }

    void factory::setsink(sink * val)
    {
        _sink = val;
    }

    void factory::write(const message & msg)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        _queue.push_back(msg);

        _cond.notify_one();
    }

    logger* factory::get(const std::string & name)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if(_sources.count(name) == 0)
        {
            _sources[name] = new logger(name,*this);
        }

        return _sources[name];
    }

    void factory::stop()
    {
        std::unique_lock<std::mutex> lock(_mutex);

        if(!_exit){
            _exit = true;
            _cond.notify_one();
           _condexit.wait(lock);
        }
    }


    void logger::write(level l,const std::string && content,const std::string && file,int lines)
    {
        _factory.write({l,std::chrono::system_clock::now(),_name,file,lines,content});
    }


    void console::write(const message &msg)
    {
        switch (msg.LEVEL)
        {
        case level::ERROR:
            std::cout << "\e[31m";
            break;
        case level::WARN:
            std::cout << "\e[35m";
            break;
        case level::INFO:
            std::cout << "\e[37m";
            break;
        case level::DEBUG:
            std::cout << "\e[36m";
            break;
        case level::TRACE:
            std::cout << "\e[32m";
            break;
        case level::VERBOSE:
            std::cout << "\e[33m";
            break;
        }

        std::time_t ts = std::chrono::system_clock::to_time_t(msg.TS);

        auto tm = std::gmtime(&ts);



        std::cout << " " << tm->tm_year + 1900 << "-" << tm->tm_mon << "-" << tm->tm_mday << " "

            << tm->tm_hour  << ":" << tm->tm_min << ":" << tm->tm_sec << " "

            << msg.Source << " (" << filepath::path(msg.File).leaf() << ") " << msg.Content << std::endl;
    }


    namespace {
        std::once_flag flag;

        factory *_factory;
    }

    logger* get(const std::string name)
    {
        std::call_once(flag,[]{
            _factory = new factory();
            _factory->setsink(new console());
        });

        return _factory->get(name);

    }

    void stop()
    {
        std::call_once(flag,[]{
            _factory = new factory();
            _factory->setsink(new console());
        });

        _factory->stop();
    }
}}