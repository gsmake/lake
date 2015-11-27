#include <lemon/os/exec.hpp>

#ifndef WIN32
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <lemon/fs/os.hpp>


namespace lemon{ namespace exec{

    class posix_process : public os::process
    {
    public:
        posix_process(
            const std::string &path,
            std::vector<std::string> args,
            const std::string workdir,
            io::reader* _stdin,
            io::writer* _stdout,
            io::writer* _stderr,
            const std::vector<std::string> &env)
            :_stdin(nullptr),_stdout(nullptr),_stderr(nullptr)
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
                    exec(path,workdir,args,env);
                }
                catch(const std::exception e)
                {
                    std::cout << "catche err :" << e.what() << std::endl;
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
            const std::string workdir,
            std::vector<std::string> args,
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

            std::vector<const char *> argv;

            argv.push_back(path.c_str());

            for(auto arg : args)
            {
                argv.push_back(arg.c_str());
            }

            argv.push_back(NULL);

            std::vector<const char *> envp;

            for(auto e : env)
            {
                envp.push_back((char*const)e.c_str());
            }

            envp.push_back(NULL);

            if (-1 == execve(
                path.c_str(),
                (char*const*)&argv[0],
                (char*const*)&envp[0]))
            {
                throw  std::system_error(errno,std::system_category(),path);
            }
        }

        int wait()
        {
            int status = 0;

            if (::waitpid(_pid,&status,0) == -1)
            {
                if (errno != ECHILD)
                {
                    std::cout << errno << std::endl;

                    throw  std::system_error(errno,std::system_category());
                }
            }

            return WEXITSTATUS(status);
        }

    private:
        pid_t               _pid;
        int                 _pipeIn[2];
        int                 _pipeOut[2];
        int                 _pipeErr[2];
        io::reader*         _stdin;
        io::writer*         _stdout;
        io::writer*         _stderr;
    };


    command & command::start(std::vector<std::string> args)
    {
        this->_process = new posix_process(_path, args, _workpath,_stdin, _stdout, _stderr, _env);

        return *this;
    }
}}

#endif //!WIN32