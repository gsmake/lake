#ifndef LEMON_CONFIGURE_H
#define LEMON_CONFIGURE_H

/*
 * below codes is used by cmake lemon lua extension functions
 * do't modify it by hand.
 */

#ifndef EXTERN_C
#	ifdef __cplusplus
#		define EXTERN_C    extern "C"
#	else
#		define EXTERN_C    extern
#	endif
#endif

#ifdef WIN32
#	define LEMON_EXPORT 	__declspec(dllexport)
#	define LEMON_IMPORT 	__declspec(dllimport)
#	define LEMON_PRIVATE
#else
#	define LEMON_EXPORT 	__attribute__ ((visibility ("default")))
#	define LEMON_IMPORT		__attribute__ ((visibility ("default")))
#	define LEMON_PRIVATE	__attribute__ ((visibility ("hidden")))
#endif //WIN32

#ifdef LEMON_BUILD_AS_DLL

#ifdef LEMON_BUILD
#	define LEMON_API 	EXTERN_C LEMON_EXPORT
#	define LEMON_CLASS 	LEMON_EXPORT
#else
#	define LEMON_API 	EXTERN_C LEMON_IMPORT
#	define LEMON_CLASS 	LEMON_IMPORT
#endif //LEMON_BUILD

#else

#	define LEMON_API 	EXTERN_C
#	define LEMON_CLASS

#endif //LEMON_BUILD_AS_DLL


/* #undef LEMON_KQUEUE_H */
/* #undef LEMON_HAS_EPOLL_H */

#ifdef WIN32
#	define WIN32_WINNT			0x0700
#	define _WIN32_WINDOWS		0x0700
#	define NTDDI_VERSION		NTDDI_WIN7
#	define _WIN32_IE			0x0700
#endif //WIN32

#ifdef WIN32
#	ifdef _DEBUG
#		define _CRTDBG_MAP_ALLOC
#		include <crtdbg.h>
#	endif
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <WinSock2.h>
#	include <Ws2tcpip.h>
#	include <tchar.h>
#	pragma comment(lib,"Ws2_32.lib")
#else
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netdb.h>
#	include <sys/time.h>
#endif //WIN32


#endif //LEMON_CONFIGURE_H
