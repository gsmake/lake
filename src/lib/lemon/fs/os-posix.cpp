#include <lemon/fs/os.hpp>

#ifndef WIN32

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

namespace lemon { namespace fs{

    std::string current_directory(std::error_code &e)
    {
        char *dir = getcwd(NULL,0);

        std::string current = dir;

        free(dir);

        return filepath::path(current).compress().string();
    }


    void set_current_directory(const std::string& path, std::error_code &err)
    {
        if (-1 == chdir(path.c_str()))
        {
            err = std::make_error_code((std::errc)errno);
        }
    }


    bool exists(const std::string & path)
    {
        struct stat info = {0};

        return stat(path.c_str(), &info) == 0;

    }

    bool is_directory(const std::string & path)
    {
        struct stat info = {0};

        if (stat(path.c_str(), &info) == 0) {

            if(S_ISDIR(info.st_mode)) {
                return true;
            }
        }

        return false;
    }

    void create_directory(const std::string& path, std::error_code &err)
    {
        if(mkdir(path.c_str(), ACCESSPERMS) != 0)
        {
            err = std::make_error_code((std::errc)errno);
        }
    }

    void remove_file(const std::string & path ,std::error_code &err)
    {
        if (0 != remove(path.c_str()))
        {
            err = std::make_error_code((std::errc)errno);
        }
    }


    void create_symlink(const std::string & source,const std::string & target, std::error_code &err)
    {
        if( 0 != symlink(source.c_str(),target.c_str()))
        {
            err = std::make_error_code((std::errc)errno);
        }
    }

    std::vector<std::string> read_directory(const std::string& path, std::error_code &err)
    {
        auto dir = opendir(path.c_str());

        if(dir == nullptr)
        {
            err = std::error_code(errno,std::system_category());

            return {};
        }

        dirent *entry;

        std::vector<std::string> entries;

        while((entry = readdir(dir)) != nullptr)
        {
            entries.push_back(std::string(entry->d_name,entry->d_name + entry->d_namlen));
        }

        closedir(dir);

        return entries;
    }
}}

#endif //WIN32