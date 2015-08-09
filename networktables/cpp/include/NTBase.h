//this file is used for hacking

#ifndef NT_BASE_H_
#define NT_BASE_H_

#if defined(_MSC_VER)
	//this is a windows flag
	//can be found in xkeycheck.h
	#define _ALLOW_KEYWORD_MACROS
	#define noexcept throw()
#endif

#endif