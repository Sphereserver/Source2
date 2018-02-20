// Current libev version: 4.24

#ifndef _LIBEV

#include "wrapper_ev.h"

// libev produces many warnings which isn't really appropriate for us to
// address since it is 3rd party code that could be overwritten at any time
// with a new version
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4013 4068 4100 4101 4127 4133 4189 4244 4245 4456 4457 4706 4996)
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wcomment"
	#pragma GCC diagnostic ignored "-Wold-style-declaration"
	#pragma GCC diagnostic ignored "-Wparentheses"
	#pragma GCC diagnostic ignored "-Wsign-compare"
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wunused-value"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wunused-result"
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
	#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

	#ifdef __MINGW__
		#include <time.h>
		struct timespec
		{
			time_t tv_sec;
			long int tv_nsec;
		};
	#endif  // __MINGW__
#endif


#include "ev.c"

#ifdef _MSC_VER
	#pragma warning(pop)
#else
	#pragma GCC diagnostic pop
#endif

#endif // _LIBEV