#ifndef LEMON_FS_PATH_HPP
#define LEMON_FS_PATH_HPP

#include <string>
#include <sstream>
#include <vector>
#include <lemon/fs/os.hpp>

namespace lemon{ namespace filepath{

    /**
     * the filepath representation class
     */
    template <typename CharType> class basic_path
    {

    public:

        typedef std::basic_string<CharType> string_type;

        typedef std::vector<string_type> nodes_type;

#ifndef WIN32
        const static char separator = '/';
#else
        const static char separator = '\\';
#endif

        basic_path(const string_type & path)
        {
            parse(path);
        }

        basic_path(const string_type && path)
        {
            parse(path);
        }

        basic_path(const std::string & vol,const std::vector<string_type> & nodes)
            :_volume(vol),_nodes(nodes)
        {

        }

        void clear()
        {
            _volume = "";
            _nodes.clear();
        }

        basic_path<CharType> parent() const
        {
            auto nodes = _nodes;

            if(nodes.size() == 1 && nodes[0] == string_type(1,CharType('.')))
            {
                return basic_path(_volume,nodes);
            }

            if(!nodes.empty()) nodes.pop_back();

            return basic_path(_volume,nodes);
        }

        bool abs() const
        {
            if(_nodes.empty())
            {
                return true;
            }

            if(_nodes[0] != string_type(1,CharType('.')))
            {
                return true;
            }

            return false;
        }

        string_type leaf() const
        {
            if(_nodes.empty())
            {
                if (_volume.empty())
                {
                    return string_type(1,CharType('.'));
                }
                else
                {
                    return _volume;
                }

            }

            return *_nodes.rbegin();
        }

        /**
         * returns the shortest path name equivalent to path by purely lexical processing
         */
        inline basic_path& compress()
        {
            std::vector<string_type> nodes;

            const static string_type key = { CharType('.'),CharType('.') };

            for(auto node : _nodes)
            {
                if(node == string_type(1,CharType('.')) && !nodes.empty())
                {
                    continue;
                }

                if( node == key )
                {
                    if(!nodes.empty()) nodes.pop_back();

                    continue;
                }

                nodes.push_back(node);
            }

            _nodes = nodes;

            return *this;
        }

        bool operator == (const basic_path & rhs) const
        {
            return string() == rhs.string();
        }

        bool operator == (const string_type & rhs) const
        {
            return string() == basic_path(rhs).string();
        }

        /**
         * parse input path string
         */
        inline void parse(const string_type & source)
        {
            clear();

            parseVolume(source);

            auto body = source.substr(_volume.length());

            int start = 0,curr = 0;

            for(auto & c : body)
            {


                if (c == CharType('\\') || c == CharType('/'))
                {
                    auto node  = body.substr(start,curr - start);

                    if (!node.empty())
                    {
                        _nodes.push_back(node);
                    }

                    start = curr + 1;
                }

                curr ++;

            }

            if(start < body.length())
            {
                _nodes.push_back(body.substr(start));
            }
        }

        /**
         * get filepath string using provide separator
         */
        const std::string string(char s)
        {
            std::basic_stringstream<CharType> stream;

            stream << _volume;

            if(_nodes.empty())
            {
                stream << CharType(s);

                return stream.str();
            }

            if(_nodes[0] != string_type(1,CharType('.')))
            {
                stream << CharType(s);
            }

            for(auto node : _nodes)
            {
                stream  << node << CharType(s);
            }

            return stream.str();
        }

        /**
         * get filepath string using slash as separator character
         */
        const std::string slash()
        {
            return string('/');
        }

        const std::string string()
        {
            return string(separator);
        }
    private:
        static inline bool isslash(CharType c)
        {
            return c == CharType('\\') || c == CharType('/');
        }

        inline void parseVolume(const std::string & source)
        {
            if(source.length() < 2)
            {
               return;
            }

            auto label = source[0];

            if (source[1] == CharType(':') &&
                ((CharType('a') <= label && label <= CharType('z')) || (CharType('A') <= label && label <= CharType('Z'))))
            {
                _volume = source.substr(0,2);
                return;
            }
            else if(source.length() > 5) //UNC
            {
                if(isslash(source[0]) && isslash(source[1]) && !isslash(source[2]) && source[2] != CharType('.'))
                {
                    for(int i = 4; i < source.length(); i ++)
                    {
                        if (isslash(source[i]))
                        {
                            i ++;

                            if(!isslash(source[i]))
                            {
                                if(source[i] == CharType('.'))
                                {
                                    return;
                                }

                                for(int j = i; j < source.length(); j ++)
                                {
                                    if(isslash(source[j]))
                                    {
                                        _volume = source.substr(0,j);

                                        return;
                                    }
                                }

                                _volume = source;

                                return;
                            }


                        }
                    }
                }
            }
        }

    private:

        string_type                            _volume;

        nodes_type                             _nodes;
    };


    typedef basic_path<char>                    path;
}}

#endif  // LEMON_FS_PATH_HPP