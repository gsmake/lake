#ifndef LEMON_FS_OS_HPP
#define LEMON_FS_OS_HPP

#include <string>
#include <system_error>
#include <lemon/fs/filepath.hpp>

namespace lemon { namespace fs {


    /**
     * return program current work directory
     */
    std::string current_directory(std::error_code &e);

    inline std::string current_directory()
    {
        std::error_code ec;
        auto curr = current_directory(ec);

        if (ec)
        {
            throw std::system_error(ec);
        }

        return curr;
    }

    /**
     * set new program working directory
     */
    void set_current_directory(const std::string& path, std::error_code &e);

    inline void set_current_directory(const std::string& path)
    {
        std::error_code ec;

        set_current_directory(path,ec);

        if (ec)
        {
            throw std::system_error(ec);
        }
    }

    /**
     * return a bool variable indicate whether the target path exists
     */
    bool exists(const std::string & path);

    /**
     * return a bool variable whether the target pah is a directory
     */
    bool is_directory(const std::string & path);

    /**
     * create new directory with path
     */
    void create_directory(const std::string& path, std::error_code &e);

    inline void create_directory(const std::string& path)
    {
        std::error_code ec;

        create_directory(path,ec);

        if (ec)
        {
            throw std::system_error(ec);
        }
    }

    /**
     * remove target file, the target file maybe a directory
     */
    void remove_file(const std::string & path ,std::error_code &e);

    inline void remove_file(const std::string& path)
    {
        std::error_code ec;

        remove_file(path,ec);

        if (ec)
        {
            throw std::system_error(ec);
        }
    }

    /**
     * create new symlink, this method may throw error, if underground filesystem not support symlink
     * @source the symlink source fs node path
     * @target the symlink target fs node path
     */
    void create_symlink(const std::string & source,const std::string & target, std::error_code &e);


    inline void create_symlink(const std::string & source,const std::string & target)
    {
        std::error_code ec;

        create_symlink(source,target,ec);

        if (ec)
        {
            throw std::system_error(ec);
        }
    }

    inline std::string abs(const std::string & source)
    {
        if(filepath::path(source).abs())
        {
            return source;
        }

        return filepath::path(current_directory() + "/" + source).compress().string();
    }
}}

#endif //LEMON_FS_OS_HPP