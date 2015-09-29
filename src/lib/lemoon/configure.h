#ifndef LEMOON_CONFIGURE_H
#define LEMOON_CONFIGURE_H

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
#	define LEMOON_EXPORT 	__declspec(dllexport)
#	define LEMOON_IMPORT 	__declspec(dllimport)
#	define LEMOON_PRIVATE
#else
#	define LEMOON_EXPORT 	__attribute__ ((visibility ("default")))
#	define LEMOON_IMPORT		__attribute__ ((visibility ("default")))
#	define LEMOON_PRIVATE	__attribute__ ((visibility ("hidden")))
#endif //WIN32

#ifdef LEMOON_BUILD_AS_DLL

#ifdef LEMOON_BUILD
#	define LEMOON_API 	EXTERN_C LEMOON_EXPORT
#	define LEMOON_CLASS 	LEMOON_EXPORT
#else
#	define LEMOON_API 	EXTERN_C LEMOON_IMPORT
#	define LEMOON_CLASS 	LEMOON_IMPORT
#endif //LEMOON_BUILD

#else

#	define LEMOON_API 	EXTERN_C
#	define LEMOON_CLASS

#endif //LEMOON_BUILD_AS_DLL


/* #undef LEMOON_KQUEUE_H */
/* #undef LEMOON_HAS_EPOLL_H */

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


#endif //LEMOON_CONFIGURE_H
