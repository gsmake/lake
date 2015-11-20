#include <lemon/fs/os.hpp>
#include <locale>
#include <lemon/fs/filepath.hpp>
#ifdef WIN32
#include <lemon/config.h>
namespace lemon {
	namespace fs {
	
		typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> convert;

		std::string current_directory(std::error_code &e)
		{
		
			wchar_t buff[MAX_PATH];

			DWORD length = ::GetCurrentDirectoryW(MAX_PATH,buff);

			if(length == 0)
			{
				e = std::error_code(GetLastError(), std::system_category());

				return "";
			}

			return filepath::path(convert().to_bytes(std::wstring(buff, length))).compress().string();
		}


		void set_current_directory(const std::string& path, std::error_code &err)
		{
			if(!::SetCurrentDirectoryW(convert().from_bytes(path).c_str()))
			{
				err = std::error_code(GetLastError(), std::system_category());
			}
		}


		bool exists(const std::string & path)
		{
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(convert().from_bytes(path).c_str())) {
				return false;
			}
		
			return true;
		}

		bool is_directory(const std::string & path)
		{
			auto attrs = GetFileAttributesW(convert().from_bytes(path).c_str());
			if (INVALID_FILE_ATTRIBUTES == attrs) 
			{
				return false;
			}

			if ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				return true;
			}

			return false;
		}

		void create_directory(const std::string& path, std::error_code &err)
		{
			if (0 == CreateDirectoryW(convert().from_bytes(path).c_str(), NULL))
			{
				err = std::error_code(GetLastError(), std::system_category());
			}

		}

		void remove_file(const std::string & path, std::error_code &err)
		{
			auto wpath = convert().from_bytes(path);

			if(!is_directory(path))
			{
				for (;;) 
				{
					if (0 == DeleteFileW(wpath.c_str())) {

						if (GetLastError() == ERROR_ACCESS_DENIED) {
							DWORD attrs = GetFileAttributesW(wpath.c_str());
							attrs &= ~FILE_ATTRIBUTE_READONLY;
							SetFileAttributesW(wpath.c_str(), attrs);
							continue;
						}

						err = std::error_code(GetLastError(), std::system_category());
					}

					break;
				}

				return;
			}

			if (0 == RemoveDirectoryW(wpath.c_str())) {
				err = std::error_code(GetLastError(), std::system_category());
			}
		}


		void create_symlink(const std::string & source, const std::string & target, std::error_code &err)
		{
			convert conv;

			auto flags = is_directory(source) ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0;

			if (0 == CreateSymbolicLinkW( conv.from_bytes(abs(target)).c_str(), conv.from_bytes(abs(source)).c_str(), flags)) {

				err = std::error_code(GetLastError(), std::system_category());
			}
		}
	}
}

#endif //WIN32