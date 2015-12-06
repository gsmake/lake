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
    void create_directory(const std::string& path, std::error_code &e) noexcept ;

    inline void create_directory(const std::string& path)
    {
        std::error_code ec;

        create_directory(path,ec);

        if (ec)
        {
            throw std::system_error(ec);
        }
    }

    inline void create_directories(const std::string &path, std::error_code &err)
    {
        if(filepath::path(path).has_parent())
        {
            auto parent = filepath::path(path).parent().native();

            if(!exists(parent))
            {
                create_directories(parent,err);

                if(err) return;
            }
        }

        create_directory(path,err);
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

    /**
     * read the directory child items
     */
    std::vector<std::string> read_directory(const std::string& path, std::error_code &err);

    inline std::vector<std::string> read_directory(const std::string& path)
    {
        std::error_code err;

        auto entries = read_directory(path,err);

        if (err)
        {
            throw std::system_error(err);
        }

        return entries;
    }


    inline void remove_directories(const std::string & path, std::error_code &err)
    {
        auto entries = read_directory(path,err);

        if(err)
        {
            return;
        }

        for(auto entry : entries)
        {
            if(entry == "." || entry == "..") continue;

            auto child = filepath::path(path + "/" + entry).native();

            if(is_directory(child))
            {
                remove_directories(child,err);

                if(err)
                {
                    return;
                }

                continue;
            }

            remove_file(child,err);

            if(err)
            {
                return;
            }
        }

        remove_file(path,err);
    }
}}

#endif //LEMON_FS_OS_HPP