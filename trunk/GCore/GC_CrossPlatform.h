#ifndef GC_CROSSPLATFORM_H
#define GC_CROSSPLATFORM_H
#pragma once

/*
	Cross-platform definitions.
*/

// Target platforms :
#define GC_PLATFORM_WIN32	1		///< Win32 Architecture (Windows98, WindowsXP, Vista)
#define GC_PLATFORM_APPLE	2		///< Apple OS (OSX)
#define	GC_PLATFORM_LINUX	3		///< Linux based OS (various, Ubuntu and Fedora as first target?)

// Find wich platform it is.
#if defined( WIN32 ) || defined( _WIN32 )	

	/// We are on Win32 operating system (windows98, windowsXP, Vista).
	#define GC_PLATFORM		GC_PLATFORM_WIN32

#elif defined( __APPLE_CC__)

	/// We are on an APPLE operating system (OSX)
	#define GC_PLATFORM		GC_PLATFORM_APPLE

#else 
	
	/// We are on a LINUX based operating system.
	#define GC_PLATFORM		GC_PLATFORM_LINUX

#endif

// Target os architectures
#define GC_OS_ARCHITECTURE_32		///< 32bit Architecture Operating System
#define GC_OS_ARCHITECTURE_64		///< 64bit Architecture Operating System

// Find wich architecture it is
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
	
	/// We are on a 64bit Architecture Operating System 
	#define GC_OS_ARCHITECTURE	GC_OS_ARCHITECTURE_64	

#else
	
	/// We are on a 32bit Architecture Operating System
	#define GC_OS_ARCHITECTURE	GC_OS_ARCHITECTURE_32

#endif


#if GC_PLATFORM == GC_PLATFORM_WIN32 //windows specific
	
	///Specific Win32 DLL Import macro :
	#define GC_DllImport   __declspec( dllimport )
	///Specific Win32 DLL Export macro :
	#define GC_DllExport   __declspec( dllexport )

#else

	//Non-Windows specifics.

	///Specific Win32 DLL Import macro :
	#define DllImport 
	///Specific Win32 DLL Export macro :
	#define DllExport 

#endif




/************************************/
   


#endif