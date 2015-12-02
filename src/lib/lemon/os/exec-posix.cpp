#include <lemon/os/exec.hpp>

#ifndef WIN32

#include <thread>
#include <iostream>


#include <lemon/fs/os.hpp>
#include <lemon/log/log.hpp>


namespace lemon{ namespace exec{

    class posix_process : public os::process
    {
    public:
        posix_process(
            const std::string &path,
            const std::vector<std::string> &args,
            const std::string workdir,
            io::reader_close* _stdin,
            io::writer_close* _stdout,
            io::writer_close* _stderr,
            const std::vector<std::string> &env)
            :_path(path),_stdin(nullptr),_stdout(nullptr),_stderr(nullptr),_logger(lemon::log::get("exec"))
        {


//            this->_stdin = _stdin;
//            this->_stdout = _stdout;
//            this->_stderr = _stderr;


            if(_stdin != nullptr)
            {
                if(0 != pipe(_pipeIn))
                {
                    throw std::system_error(errno,std::system_category(),"create stdin pipe error");
                }
            }

            if(_stdout != nullptr)
            {
                if(0 != pipe(_pipeOut))
                {
                    throw std::system_error(errno,std::system_category(),"create stdout pipe error");
                }
            }

            if(_stderr != nullptr)
            {
                if(0 != pipe(_pipeErr))
                {
                    throw std::system_error(errno,std::system_category(),"create stderr pipe error");
                }
            }


            _pid = fork();

            switch (_pid)
            {
            case -1:
                throw std::system_error(errno,std::system_category(),"create stderr pipe error");
            case 0:
                // child process
                try
                {
                    exec(path,args,workdir,env);
                }
                catch(const std::exception e)
                {
                    lemonE(_logger,"catch error :%s",e.what());
                    exit(1);
                }

                break;
            default:
                // parent process
                handlePipe();
                break;
            }
        }
    private:

        void handlePipe()
        {

        }

        void exec(
            const std::string &path,
            const std::vector<std::string>& args,
            const std::string &workdir,
            const std::vector<std::string> &env)
        {

            if(_stdin != nullptr)
            {
                if(-1 == dup2(_pipeIn[0], STDIN_FILENO))
                {
                    throw  std::system_error(errno,std::system_category(),"replace stdin error");
                }

                close(_pipeIn[0]);
                close(_pipeIn[1]);
            }

            if(_stdout != nullptr)
            {
                if(-1 == dup2(_pipeOut[1], STDOUT_FILENO))
                {

                    throw  std::system_error(errno,std::system_category(),"replace stdout error");
                }

                close(_pipeOut[0]);
                close(_pipeOut[1]);
            }

            if(_stderr != nullptr)
            {
                if(-1 == dup2(_pipeErr[1], STDERR_FILENO))
                {
                    throw  std::system_error(errno,std::system_category(),"replace stderr error");
                }

                close(_pipeErr[0]);
                close(_pipeErr[1]);
            }

            if(!workdir.empty())
            {
                fs::set_current_directory(workdir);
            }

            const char ** argv = new const char*[args.size() + 2];

            argv[0] = path.c_str();

            int i = 1;

            for(auto &arg : args)
            {
                argv[i] = arg.c_str();

                i ++;
            }

            argv[i] = NULL;

            std::vector<const char *> envp;

            for(auto e : env)
            {
                envp.push_back((char*const)e.c_str());
            }

            envp.push_back(NULL);

            if (-1 == execve(
                path.c_str(),
                (char*const*)argv,
                (char*const*)&envp[0]))
            {
                throw  std::system_error(errno,std::system_category(),path);
            }
        }

        int wait()
        {
            int status = 0;

            pid_t ret;

            do{

               ret = ::waitpid(_pid, &status, 0);

            } while ((ret == -1 && errno == EINTR) || (ret != -1 && !WIFEXITED(status)));

            if (ret == -1 && errno != ECHILD)
            {
                auto err = std::error_code(errno,std::system_category());
                lemonE(_logger,"catch error :%s", err.message().c_str())
                throw  std::system_error(err);
            }

            return WEXITSTATUS(status);
        }

    private:
        std::string                 _path;
        pid_t                       _pid;
        int                         _pipeIn[2];
        int                         _pipeOut[2];
        int                         _pipeErr[2];
        io::reader_close*           _stdin;
        io::writer_close*           _stdout;
        io::writer_close*           _stderr;
        const lemon::log::logger    &_logger;
    };


    command & command::start(std::vector<std::string> args)
    {
        this->_process = new posix_process(_path, args, _workpath,_stdin, _stdout, _stderr, _env);

        return *this;
    }
}}

#endif //!WIN32